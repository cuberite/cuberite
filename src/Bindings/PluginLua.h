
// PluginLua.h

// Declares the cPluginLua class representing a plugin written in Lua





#pragma once

#include "Plugin.h"
#include "LuaState.h"

// Names for the global variables through which the plugin is identified in its LuaState
#define LUA_PLUGIN_NAME_VAR_NAME     "_CuberiteInternal_PluginName"
#define LUA_PLUGIN_INSTANCE_VAR_NAME "_CuberiteInternal_PluginInstance"




// fwd: "UI/Window.h"
class cWindow;





// tolua_begin
class cPluginLua :
	public cPlugin
{
	typedef cPlugin super;

public:
	// tolua_end

	/** A RAII-style mutex lock for accessing the internal LuaState.
	This will be the only way to retrieve the plugin's LuaState;
	therefore it directly supports accessing the LuaState of the locked plugin.
	Usage:
		cPluginLua::cOperation Op(SomePlugin);
		Op().Call(...)  // Call a function in the plugin's LuaState
	*/
	class cOperation
	{
	public:
		cOperation(cPluginLua & a_Plugin) :
			m_Plugin(a_Plugin),
			m_Lock(a_Plugin.m_CriticalSection)
		{
		}

		cLuaState & operator ()(void) { return m_Plugin.m_LuaState; }

	protected:
		cPluginLua & m_Plugin;

		/** RAII lock for m_Plugin.m_CriticalSection */
		cCSLock m_Lock;
	} ;



	/** A base class that represents something related to a plugin
	The plugin can reset this class so that the instance can continue to exist but will not engage the (possibly non-existent) plugin anymore.
	This is used for scheduled tasks etc., so that they can be queued and reset when the plugin is terminated, without removing them from the queue. */
	class cResettable
	{
	public:
		/** Creates a new instance bound to the specified plugin. */
		cResettable(cPluginLua & a_Plugin);

		// Force a virtual destructor in descendants:
		virtual ~cResettable() {}

		/** Resets the plugin instance stored within.
		The instance will continue to exist, but should not call into the plugin anymore. */
		virtual void Reset(void);

	protected:
		/** The plugin that this instance references.
		If nullptr, the plugin has already unloaded and the instance should bail out any processing.
		Protected against multithreaded access by m_CSPlugin. */
		cPluginLua * m_Plugin;

		/** The mutex protecting m_Plugin against multithreaded access. */
		cCriticalSection m_CSPlugin;
	};

	typedef SharedPtr<cResettable> cResettablePtr;
	typedef std::vector<cResettablePtr> cResettablePtrs;


	cPluginLua(const AString & a_PluginDirectory);
	~cPluginLua();

	virtual void OnDisable(void) override;
	virtual bool Load(void) override;
	virtual void Unload(void) override;

	virtual void Tick(float a_Dt) override;

	virtual bool OnBlockSpread              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source) override;
	virtual bool OnBlockToPickups           (cWorld & a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) override;
	virtual bool OnBrewingCompleting        (cWorld & a_World, cBrewingstandEntity & a_BrewingstandEntity) override;
	virtual bool OnBrewingCompleted         (cWorld & a_World, cBrewingstandEntity & a_BrewingstandEntity) override;
	virtual bool OnChat                     (cPlayer & a_Player, AString & a_Message) override;
	virtual bool OnChunkAvailable           (cWorld & a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnChunkGenerated           (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) override;
	virtual bool OnChunkGenerating          (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) override;
	virtual bool OnChunkUnloaded            (cWorld & a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnChunkUnloading           (cWorld & a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnCollectingPickup         (cPlayer & a_Player, cPickup & a_Pickup) override;
	virtual bool OnCraftingNoRecipe         (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) override;
	virtual bool OnDisconnect               (cClientHandle & a_Client, const AString & a_Reason) override;
	virtual bool OnEntityAddEffect          (cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier) override;
	virtual bool OnEntityChangingWorld      (cEntity & a_Entity, cWorld & a_World) override;
	virtual bool OnEntityChangedWorld       (cEntity & a_Entity, cWorld & a_World) override;
	virtual bool OnExecuteCommand           (cPlayer * a_Player, const AStringVector & a_Split, const AString & a_EntireCommand, cPluginManager::CommandResult & a_Result) override;
	virtual bool OnExploded                 (cWorld & a_World, double a_ExplosionSize,   bool a_CanCauseFire,   double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) override;
	virtual bool OnExploding                (cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) override;
	virtual bool OnHandshake                (cClientHandle & a_Client, const AString & a_Username) override;
	virtual bool OnHopperPullingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum) override;
	virtual bool OnHopperPushingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum) override;
	virtual bool OnKilled                   (cEntity & a_Victim, TakeDamageInfo & a_TDI, AString & a_DeathMessage) override;
	virtual bool OnKilling                  (cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI) override;
	virtual bool OnLogin                    (cClientHandle & a_Client, UInt32 a_ProtocolVersion, const AString & a_Username) override;
	virtual bool OnPlayerAnimation          (cPlayer & a_Player, int a_Animation) override;
	virtual bool OnPlayerBreakingBlock      (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerBrokenBlock        (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerDestroyed          (cPlayer & a_Player) override;
	virtual bool OnPlayerEating             (cPlayer & a_Player) override;
	virtual bool OnPlayerFished             (cPlayer & a_Player, const cItems & a_Reward) override;
	virtual bool OnPlayerFishing            (cPlayer & a_Player, cItems & a_Reward) override;
	virtual bool OnPlayerFoodLevelChange    (cPlayer & a_Player, int a_NewFoodLevel) override;
	virtual bool OnPlayerJoined             (cPlayer & a_Player) override;
	virtual bool OnPlayerLeftClick          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status) override;
	virtual bool OnPlayerMoving             (cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition) override;
	virtual bool OnPlayerPlacedBlock        (cPlayer & a_Player, const sSetBlock & a_BlockChange) override;
	virtual bool OnPlayerPlacingBlock       (cPlayer & a_Player, const sSetBlock & a_BlockChange) override;
	virtual bool OnPlayerRightClick         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool OnPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity) override;
	virtual bool OnPlayerShooting           (cPlayer & a_Player) override;
	virtual bool OnPlayerSpawned            (cPlayer & a_Player) override;
	virtual bool OnEntityTeleport           (cEntity & a_Entity, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition) override;
	virtual bool OnPlayerTossingItem        (cPlayer & a_Player) override;
	virtual bool OnPlayerUsedBlock          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerUsedItem           (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool OnPlayerUsingBlock         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool OnPlayerUsingItem          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool OnPluginMessage            (cClientHandle & a_Client, const AString & a_Channel, const AString & a_Message) override;
	virtual bool OnPluginsLoaded            (void) override;
	virtual bool OnPostCrafting             (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) override;
	virtual bool OnPreCrafting              (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) override;
	virtual bool OnProjectileHitBlock       (cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos) override;
	virtual bool OnProjectileHitEntity      (cProjectileEntity & a_Projectile, cEntity & a_HitEntity) override;
	virtual bool OnServerPing               (cClientHandle & a_ClientHandle, AString & a_ServerDescription, int & a_OnlinePlayersCount, int & a_MaxPlayersCount, AString & a_Favicon) override;
	virtual bool OnSpawnedEntity            (cWorld & a_World, cEntity & a_Entity) override;
	virtual bool OnSpawnedMonster           (cWorld & a_World, cMonster & a_Monster) override;
	virtual bool OnSpawningEntity           (cWorld & a_World, cEntity & a_Entity) override;
	virtual bool OnSpawningMonster          (cWorld & a_World, cMonster & a_Monster) override;
	virtual bool OnTakeDamage               (cEntity & a_Receiver, TakeDamageInfo & a_TakeDamageInfo) override;
	virtual bool OnUpdatedSign              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player) override;
	virtual bool OnUpdatingSign             (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player) override;
	virtual bool OnWeatherChanged           (cWorld & a_World) override;
	virtual bool OnWeatherChanging          (cWorld & a_World, eWeather & a_NewWeather) override;
	virtual bool OnWorldStarted             (cWorld & a_World) override;
	virtual bool OnWorldTick                (cWorld & a_World, std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec) override;

	virtual bool HandleCommand(const AStringVector & a_Split, cPlayer & a_Player, const AString & a_FullCommand) override;

	virtual bool HandleConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output, const AString & a_FullCommand) override;

	virtual void ClearCommands(void) override;

	virtual void ClearConsoleCommands(void) override;

	/** Returns true if the plugin contains the function for the specified hook type, using the old-style registration (#121) */
	bool CanAddOldStyleHook(int a_HookType);

	/** Binds the command to call the function specified by a Lua function reference. Simply adds to CommandMap. */
	void BindCommand(const AString & a_Command, int a_FnRef);

	/** Binds the console command to call the function specified by a Lua function reference. Simply adds to CommandMap. */
	void BindConsoleCommand(const AString & a_Command, int a_FnRef);

	cLuaState & GetLuaState(void) { return m_LuaState; }

	cCriticalSection & GetCriticalSection(void) { return m_CriticalSection; }

	/** Removes a previously referenced object (luaL_unref()) */
	void Unreference(int a_LuaRef);

	/** Calls the plugin-specified "cLuaWindow closing" callback. Returns true only if the callback returned true */
	bool CallbackWindowClosing(int a_FnRef, cWindow & a_Window, cPlayer & a_Player, bool a_CanRefuse);

	/** Calls the plugin-specified "cLuaWindow slot changed" callback. */
	void CallbackWindowSlotChanged(int a_FnRef, cWindow & a_Window, int a_SlotNum);

	/** Returns the name of Lua function that should handle the specified hook type in the older (#121) API */
	static const char * GetHookFnName(int a_HookType);

	/** Adds a Lua function to be called for the specified hook.
	The function has to be on the Lua stack at the specified index a_FnRefIdx
	Returns true if the hook was added successfully.
	*/
	bool AddHookRef(int a_HookType, int a_FnRefIdx);

	/** Calls a function in this plugin's LuaState with parameters copied over from a_ForeignState.
	The values that the function returns are placed onto a_ForeignState.
	Returns the number of values returned, if successful, or negative number on failure. */
	int CallFunctionFromForeignState(
		const AString & a_FunctionName,
		cLuaState & a_ForeignState,
		int a_ParamStart,
		int a_ParamEnd
	);

	/** Call a Lua function residing in the plugin. */
	template <typename FnT, typename... Args>
	bool Call(FnT a_Fn, Args && ... a_Args)
	{
		cCSLock Lock(m_CriticalSection);
		return m_LuaState.Call(a_Fn, a_Args...);
	}

	/** Adds the specified cResettable instance to m_Resettables, so that it is notified when the plugin is being closed. */
	void AddResettable(cResettablePtr a_Resettable);

protected:
	/** Maps command name into Lua function reference */
	typedef std::map<AString, int> CommandMap;

	/** Provides an array of Lua function references */
	typedef std::vector<cLuaState::cRef *> cLuaRefs;

	/** Maps hook types into arrays of Lua function references to call for each hook type */
	typedef std::map<int, cLuaRefs> cHookMap;


	/** The mutex protecting m_LuaState and each of the m_Resettables[] against multithreaded use. */
	cCriticalSection m_CriticalSection;

	/** The plugin's Lua state. */
	cLuaState m_LuaState;

	/** Objects that need notification when the plugin is about to be unloaded. */
	cResettablePtrs m_Resettables;

	/** In-game commands that the plugin has registered. */
	CommandMap m_Commands;

	/** Console commands that the plugin has registered. */
	CommandMap m_ConsoleCommands;

	/** Hooks that the plugin has registered. */
	cHookMap m_HookMap;


	/** Releases all Lua references, notifies and removes all m_Resettables[] and closes the m_LuaState. */
	void Close(void);

	/** Removes all WebTabs currently registered for this plugin from the WebAdmin. */
	void ClearWebTabs(void);
} ;  // tolua_export




