#pragma once





#include <map>
#include <functional>





// fwd:
class cBlockHandler;
class BlockState;





/** Complete information about a single block type.
The BlockTypeRegistry uses this structure to store the registered information. */
class BlockInfo
{
public:

	/** Callback is used to query block hints dynamically, based on the current BlockState.
	Useful for example for redstone lamps that can be turned on or off. */
	using HintCallback = std::function<AString(const AString & aTypeName, const BlockState & aBlockState)>;


	/** Creates a new instance with the specified BlockTypeName and handler / hints / callbacks.
	aPluginName specifies the name of the plugin to associate with the block type (to allow unload / reload). */
	BlockInfo(
		const AString & aPluginName,
		const AString & aBlockTypeName,
		std::shared_ptr<cBlockHandler> aHandler,
		const std::map<AString, AString> & aHints = std::map<AString, AString>(),
		const std::map<AString, HintCallback> & aHintCallbacks = std::map<AString, HintCallback>()
	);


	/** Retrieves the value associated with the specified hint for this specific BlockTypeName and BlockState.
	Queries hint callbacks first, then static hints if a callback doesn't exist.
	Returns an empty string if hint not found at all. */
	AString hintValue(
		const AString & aHintName,
		const BlockState & aBlockState
	);

	// Simple getters:
	const AString & pluginName() const { return m_PluginName; }
	const AString & blockTypeName() const { return m_BlockTypeName; }
	std::shared_ptr<cBlockHandler> handler() const { return m_Handler; }

	/** Sets (creates or updates) a static hint.
	Hints provided by callbacks are unaffected by this - callbacks are "higher priority", they overwrite anything set here.
	Logs an info message if the hint is already provided by a hint callback. */
	void setHint(const AString & aHintKey, const AString & aHintValue);

	/** Removes a hint.
	Silently ignored if the hint hasn't been previously set. */
	void removeHint(const AString & aHintKey);


private:

	/** The name of the plugin that registered the block. */
	AString m_PluginName;

	/** The name of the block type, such as "minecraft:redstone_lamp" */
	AString m_BlockTypeName;

	/** The callbacks to call for various interaction. */
	std::shared_ptr<cBlockHandler> m_Handler;

	/** Optional static hints for any subsystem to use, such as "IsSnowable" -> "1".
	Hint callbacks are of higher priority than m_Hints - if a hint is provided by a m_HintCallback, its value in m_Hints is ignored. */
	std::map<AString, AString> m_Hints;

	/** The callbacks for dynamic evaluation of hints, such as "LightValue" -> function(BlockTypeName, BlockState).
	Hint callbacks are of higher priority than m_Hints - if a hint is provided by a m_HintCallback, its value in m_Hints is ignored. */
	std::map<AString, HintCallback> m_HintCallbacks;
};





/** Stores information on all known block types.
Can dynamically add and remove block types.
Block types are identified using BlockTypeName.
Supports unregistering and re-registering the same type by the same plugin.
Stores the name of the plugin that registered the type, for better plugin error messages ("already registered in X")
and so that we can unload and reload plugins. */
class BlockTypeRegistry
{
public:
	// fwd:
	class AlreadyRegisteredException;
	class NotRegisteredException;


	/** Creates an empty new instance of the block type registry */
	BlockTypeRegistry() = default;

	/** Registers the specified block type.
	If the block type already exists and the plugin is the same, updates the registration.
	If the block type already exists and the plugin is different, throws an AlreadyRegisteredException. */
	void registerBlockType(
		const AString & aPluginName,
		const AString & aBlockTypeName,
		std::shared_ptr<cBlockHandler> aHandler,
		const std::map<AString, AString> & aHints = std::map<AString, AString>(),
		const std::map<AString, BlockInfo::HintCallback> & aHintCallbacks = std::map<AString, BlockInfo::HintCallback>()
	);

	/** Returns the registration information for the specified BlockTypeName.
	Returns nullptr if BlockTypeName not found. */
	std::shared_ptr<BlockInfo> blockInfo(const AString & aBlockTypeName);

	/** Removes all registrations done by the specified plugin. */
	void removeAllByPlugin(const AString & aPluginName);

	/** Sets (adds or overwrites) a single Hint value for a BlockType.
	Throws NotRegisteredException if the BlockTypeName is not registered. */
	void setBlockTypeHint(
		const AString & aBlockTypeName,
		const AString & aHintKey,
		const AString & aHintValue
	);

	/** Removes a previously registered single Hint value for a BlockType.
	Throws NotRegisteredException if the BlockTypeName is not registered.
	Silently ignored if the Hint hasn't been previously set. */
	void removeBlockTypeHint(
		const AString & aBlockTypeName,
		const AString & aHintKey
	);


private:

	/** The actual block type registry.
	Maps the BlockTypeName to the BlockInfo instance. */
	std::map<AString, std::shared_ptr<BlockInfo>> m_Registry;

	/** The CS that protects m_Registry against multithreaded access. */
	cCriticalSection m_CSRegistry;
};





/** The exception thrown from BlockTypeRegistry::registerBlockType() if the same block type is being registered from a different plugin. */
class BlockTypeRegistry::AlreadyRegisteredException: public std::runtime_error
{
	using Super = std::runtime_error;

public:

	/** Creates a new instance of the exception that provides info on both the original registration and the newly attempted
	registration that caused the failure. */
	AlreadyRegisteredException(
		const std::shared_ptr<BlockInfo> & aPreviousRegistration,
		const std::shared_ptr<BlockInfo> & aNewRegistration
	);

	// Simple getters:
	std::shared_ptr<BlockInfo> previousRegistration() const { return m_PreviousRegistration; }
	std::shared_ptr<BlockInfo> newRegistration()      const { return m_NewRegistration; }


private:

	std::shared_ptr<BlockInfo> m_PreviousRegistration;
	std::shared_ptr<BlockInfo> m_NewRegistration;


	/** Returns the general exception message formatted by the two registrations.
	The output is used when logging. */
	static AString message(
		const std::shared_ptr<BlockInfo> & aPreviousRegistration,
		const std::shared_ptr<BlockInfo> & aNewRegistration
	);
};





/** The exception thrown from BlockTypeRegistry::setBlockTypeHint() if the block type has not been registered before. */
class BlockTypeRegistry::NotRegisteredException: public std::runtime_error
{
	using Super = std::runtime_error;

public:

	/** Creates a new instance of the exception that provides info on both the original registration and the newly attempted
	registration that caused the failure. */
	NotRegisteredException(
		const AString & aBlockTypeName,
		const AString & aHintKey,
		const AString & aHintValue
	);

	// Simple getters:
	const AString & blockTypeName() const { return m_BlockTypeName; }


private:

	const AString m_BlockTypeName;
};
