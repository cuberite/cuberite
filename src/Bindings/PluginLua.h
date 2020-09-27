
// PluginLua.h

// Declares the cPluginLua class representing a plugin written in Lua





#pragma once

#include "Plugin.h"
#include "LuaState.h"

// Names for the global variables through which the plugin is identified in its LuaState
#define LUA_PLUGIN_NAME_VAR_NAME     "_CuberiteInternal_PluginName"
#define LUA_PLUGIN_INSTANCE_VAR_NAME "_CuberiteInternal_PluginInstance"




// tolua_begin
class cPluginLua:
	public cPlugin
{
	// tolua_end

	using Super = cPlugin;

public:

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
			m_Lock(a_Plugin.m_LuaState)
		{
		}

		cLuaState & operator ()(void) { return m_Plugin.m_LuaState; }

	protected:
		cPluginLua & m_Plugin;

		/** RAII lock for the Lua state. */
		cLuaState::cLock m_Lock;
	} ;



	cPluginLua(const AString & a_PluginDirectory, cDeadlockDetect & a_DeadlockDetect);
	virtual ~cPluginLua() override;

	virtual void OnDisable(void) override;
	virtual bool Load(void) override;
	virtual void Unload(void) override;

	virtual void Tick(float a_Dt) override;

	virtual bool OnBlockSpread              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source) override;
	virtual bool OnBlockToPickups           (cWorld & a_World, Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool, cItems & a_Pickups) override;
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
	virtual bool OnDropSpense               (cWorld & a_World, cDropSpenserEntity & a_DropSpenser, int a_SlotNum) override;
	virtual bool OnKilled                   (cEntity & a_Victim, TakeDamageInfo & a_TDI, AString & a_DeathMessage) override;
	virtual bool OnKilling                  (cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI) override;
	virtual bool OnLogin                    (cClientHandle & a_Client, UInt32 a_ProtocolVersion, const AString & a_Username) override;
	virtual bool OnLoginForge               (cClientHandle & a_Client, const AStringMap & a_Mods) override;
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
	virtual bool OnPlayerMoving             (cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition, bool a_PreviousIsOnGround) override;
	virtual bool OnPlayerOpeningWindow      (cPlayer & a_Player, cWindow & a_Window) override;
	virtual bool OnPlayerPlacedBlock        (cPlayer & a_Player, const sSetBlock & a_BlockChange) override;
	virtual bool OnPlayerPlacingBlock       (cPlayer & a_Player, const sSetBlock & a_BlockChange) override;
	virtual bool OnPlayerCrouched           (cPlayer & a_Player) override;
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

	/** Returns true if the plugin contains the function for the specified hook type, using the old-style registration (#121) */
	bool CanAddOldStyleHook(int a_HookType);

	/** Returns the name of Lua function that should handle the specified hook type in the older (#121) API */
	static const char * GetHookFnName(int a_HookType);

	/** Adds a Lua callback to be called for the specified hook.
	Returns true if the hook was added successfully. */
	bool AddHookCallback(int a_HookType, cLuaState::cCallbackPtr && a_Callback);

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
		return cOperation(*this)().Call(a_Fn, a_Args...);
	}

protected:
	/** Provides an array of Lua function references */
	typedef std::vector<cLuaState::cCallbackPtr> cLuaCallbacks;

	/** Maps hook types into arrays of Lua function references to call for each hook type */
	typedef std::map<int, cLuaCallbacks> cHookMap;


	/** The plugin's Lua state. */
	cLuaState m_LuaState;

	/** Hooks that the plugin has registered. */
	cHookMap m_HookMap;

	/** The DeadlockDetect object to which the plugin's CS is tracked. */
	cDeadlockDetect & m_DeadlockDetect;


	/** Releases all Lua references, notifies and removes all m_Resettables[] and closes the m_LuaState. */
	void Close(void);

	/** Removes all WebTabs currently registered for this plugin from the WebAdmin. */
	void ClearWebTabs(void);

	/** Calls a hook that has the simple format - single bool return value specifying whether the chain should continue.
	The advanced hook types that need more processing implement a similar loop manually instead.
	Returns true if any of hook calls wants to abort the hook (returned true), false if all hook calls returned false. */
	template <typename... Args>
	bool CallSimpleHooks(int a_HookType, Args && ... a_Args)
	{
		cOperation op(*this);
		auto & hooks = m_HookMap[a_HookType];
		bool res = false;
		for (auto & hook: hooks)
		{
			hook->Call(std::forward<Args>(a_Args)..., cLuaState::Return, res);
			if (res)
			{
				// Hook wants to terminate the chain processing
				return true;
			}
		}
		return false;
	}
};  // tolua_export
