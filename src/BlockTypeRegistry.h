#pragma once





#include <map>





// fwd:
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
	Queries callbacks first, then hints if a callback doesn't exist.
	Returns an empty string if hint not found at all. */
	AString hintValue(
		const AString & aHintName,
		const BlockState & aBlockState
	);

	// Simple getters:
	const AString & pluginName() const { return mPluginName; }
	const AString & blockTypeName() const { return mBlockTypeName; }
	std::shared_ptr<cBlockHandler> handler() const { return mHandler; }


private:

	/** The name of the plugin that registered the block. */
	AString mPluginName;

	/** The name of the block type, such as "minecraft:redstone_lamp" */
	AString mBlockTypeName;

	/** The callbacks to call for various interaction. */
	std::shared_ptr<cBlockHandler> mHandler;

	/** Optional hints for any subsystem to use, such as "IsSnowable" -> "1". */
	std::map<AString, AString> mHints;

	/** The callbacks for dynamic evaluation of hints, such as "LightValue" -> function(BlockTypeName, BlockState). */
	std::map<AString, HintCallback> mHintCallbacks;
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


private:

	/** The actual block type registry.
	Maps the BlockTypeName to the BlockInfo instance. */
	std::map<AString, std::shared_ptr<BlockInfo>> mRegistry;

	/** The CS that protects mRegistry against multithreaded access. */
	cCriticalSection mCSRegistry;
};





/** The exception thrown from BlockTypeRegistry::registerBlockType() if the same block type is being registered from a different plugin. */
class BlockTypeRegistry::AlreadyRegisteredException: public std::runtime_error
{
	using Super = std::runtime_error;

public:

	/** Creates a new instance of the exception that provides info on both the original registration and the newly attempted
	registration that caused the failure. */
	AlreadyRegisteredException(
		std::shared_ptr<BlockInfo> aPreviousRegistration,
		std::shared_ptr<BlockInfo> aNewRegistration
	);

	// Simple getters:
	std::shared_ptr<BlockInfo> previousRegistration() const { return mPreviousRegistration; }
	std::shared_ptr<BlockInfo> newRegistration()      const { return mNewRegistration; }


private:

	std::shared_ptr<BlockInfo> mPreviousRegistration;
	std::shared_ptr<BlockInfo> mNewRegistration;


	/** Returns the general exception message formatted by the two registrations.
	The output is used when logging. */
	static AString message(
		std::shared_ptr<BlockInfo> aPreviousRegistration,
		std::shared_ptr<BlockInfo> aNewRegistration
	);
};
