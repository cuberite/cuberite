
#pragma once

#include "PluginInterface.h"

// fwd:
class cPlugin;






// tolua_begin
class cPluginManager:
	public cPluginInterface
{
public:
	// tolua_end
	
	// Called each tick
	virtual void Tick(float a_Dt);
	
	// tolua_begin
	enum CommandResult
	{
		crExecuted,
		crUnknownCommand,
		crError,
		crBlocked,
		crNoPermission,
	} ;

	enum PluginHook
	{
		HOOK_BLOCK_SPREAD,
		HOOK_BLOCK_TO_PICKUPS,
		HOOK_CHAT,
		HOOK_CHUNK_AVAILABLE,
		HOOK_CHUNK_GENERATED,
		HOOK_CHUNK_GENERATING,
		HOOK_CHUNK_UNLOADED,
		HOOK_CHUNK_UNLOADING,
		HOOK_COLLECTING_PICKUP,
		HOOK_CRAFTING_NO_RECIPE,
		HOOK_DISCONNECT,
		HOOK_PLAYER_ANIMATION,
		HOOK_ENTITY_ADD_EFFECT,
		HOOK_EXECUTE_COMMAND,
		HOOK_EXPLODED,
		HOOK_EXPLODING,
		HOOK_HANDSHAKE,
		HOOK_HOPPER_PULLING_ITEM,
		HOOK_HOPPER_PUSHING_ITEM,
		HOOK_KILLING,
		HOOK_LOGIN,
		HOOK_PLAYER_BREAKING_BLOCK,
		HOOK_PLAYER_BROKEN_BLOCK,
		HOOK_PLAYER_DESTROYED,
		HOOK_PLAYER_EATING,
		HOOK_PLAYER_FISHED,
		HOOK_PLAYER_FISHING,
		HOOK_PLAYER_FOOD_LEVEL_CHANGE,
		HOOK_PLAYER_JOINED,
		HOOK_PLAYER_LEFT_CLICK,
		HOOK_PLAYER_MOVING,
		HOOK_PLAYER_PLACED_BLOCK,
		HOOK_PLAYER_PLACING_BLOCK,
		HOOK_PLAYER_RIGHT_CLICK,
		HOOK_PLAYER_RIGHT_CLICKING_ENTITY,
		HOOK_PLAYER_SHOOTING,
		HOOK_PLAYER_SPAWNED,
		HOOK_ENTITY_TELEPORT,
		HOOK_PLAYER_TOSSING_ITEM,
		HOOK_PLAYER_USED_BLOCK,
		HOOK_PLAYER_USED_ITEM,
		HOOK_PLAYER_USING_BLOCK,
		HOOK_PLAYER_USING_ITEM,
		HOOK_PLUGIN_MESSAGE,
		HOOK_PLUGINS_LOADED,
		HOOK_POST_CRAFTING,
		HOOK_PRE_CRAFTING,
		HOOK_PROJECTILE_HIT_BLOCK,
		HOOK_PROJECTILE_HIT_ENTITY,
		HOOK_SERVER_PING,
		HOOK_SPAWNED_ENTITY,
		HOOK_SPAWNED_MONSTER,
		HOOK_SPAWNING_ENTITY,
		HOOK_SPAWNING_MONSTER,
		HOOK_TAKE_DAMAGE,
		HOOK_TICK,
		HOOK_UPDATED_SIGN,
		HOOK_UPDATING_SIGN,
		HOOK_WEATHER_CHANGED,
		HOOK_WEATHER_CHANGING,
		HOOK_WORLD_STARTED,
		HOOK_WORLD_TICK,
		
		// Note that if a hook type is added, it may need processing in cPlugin::CanAddHook() descendants,
		//   and it definitely needs adding in cPluginLua::GetHookFnName() !
		
		// Keep these two as the last items, they are used for validity checking and get their values automagically
		HOOK_NUM_HOOKS,
		HOOK_MAX = HOOK_NUM_HOOKS - 1,
	} ;
	// tolua_end

	/** Used as a callback for enumerating bound commands */
	class cCommandEnumCallback
	{
	public:
		virtual ~cCommandEnumCallback() {}
		
		/** Called for each command; return true to abort enumeration
		For console commands, a_Permission is not used (set to empty string)
		*/
		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) = 0;
	} ;
	
	/** The interface used for enumerating and extern-calling plugins */
	typedef cItemCallback<cPlugin> cPluginCallback;
	
	
	/** Returns the instance of the Plugin Manager (there is only ever one) */
	static cPluginManager * Get(void);  // tolua_export

	typedef std::map< AString, cPlugin * > PluginMap;
	typedef std::list< cPlugin * > PluginList;
	cPlugin * GetPlugin( const AString & a_Plugin) const;  // tolua_export
	const PluginMap & GetAllPlugins() const;  // >> EXPORTED IN MANUALBINDINGS <<

	// tolua_begin
	void FindPlugins();
	void ReloadPlugins();
	// tolua_end
	
	/** Adds the plugin to the list of plugins called for the specified hook type. Handles multiple adds as a single add */
	void AddHook(cPlugin * a_Plugin, int a_HookType);

	size_t GetNumPlugins() const;  // tolua_export
	
	// Calls for individual hooks. Each returns false if the action is to continue or true if the plugin wants to abort
	virtual bool CallHookBlockSpread              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source) override;
	virtual bool CallHookBlockToPickups           (cWorld & a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) override;
	virtual bool CallHookChat                     (cPlayer & a_Player, AString & a_Message) override;
	virtual bool CallHookChunkAvailable           (cWorld & a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool CallHookChunkGenerated           (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) override;
	virtual bool CallHookChunkGenerating          (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) override;
	virtual bool CallHookChunkUnloaded            (cWorld & a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool CallHookChunkUnloading           (cWorld & a_World, int a_ChunkX, int a_ChunkZ) override;
	virtual bool CallHookCollectingPickup         (cPlayer & a_Player, cPickup & a_Pickup) override;
	virtual bool CallHookCraftingNoRecipe         (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) override;
	virtual bool CallHookDisconnect               (cClientHandle & a_Client, const AString & a_Reason) override;
	virtual bool CallHookEntityAddEffect          (cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier) override;
	virtual bool CallHookEntityTeleport           (cEntity & a_Entity, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition) override;
	virtual bool CallHookExecuteCommand           (cPlayer * a_Player, const AStringVector & a_Split, const AString & a_EntireCommand) override;  // If a_Player == nullptr, it is a console cmd
	virtual bool CallHookExploded                 (cWorld & a_World, double a_ExplosionSize,   bool a_CanCauseFire,   double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) override;
	virtual bool CallHookExploding                (cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) override;
	virtual bool CallHookHandshake                (cClientHandle & a_ClientHandle, const AString & a_Username) override;
	virtual bool CallHookHopperPullingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum) override;
	virtual bool CallHookHopperPushingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum) override;
	virtual bool CallHookKilling                  (cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI) override;
	virtual bool CallHookLogin                    (cClientHandle & a_Client, int a_ProtocolVersion, const AString & a_Username) override;
	virtual bool CallHookPlayerAnimation          (cPlayer & a_Player, int a_Animation) override;
	virtual bool CallHookPlayerBreakingBlock      (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool CallHookPlayerBrokenBlock        (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool CallHookPlayerDestroyed          (cPlayer & a_Player) override;
	virtual bool CallHookPlayerEating             (cPlayer & a_Player) override;
	virtual bool CallHookPlayerFished             (cPlayer & a_Player, const cItems & a_Reward) override;
	virtual bool CallHookPlayerFishing            (cPlayer & a_Player, cItems a_Reward) override;
	virtual bool CallHookPlayerFoodLevelChange    (cPlayer & a_Player, int a_NewFoodLevel) override;
	virtual bool CallHookPlayerJoined             (cPlayer & a_Player) override;
	virtual bool CallHookPlayerLeftClick          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, char a_Status) override;
	virtual bool CallHookPlayerMoving             (cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition) override;
	virtual bool CallHookPlayerPlacedBlock        (cPlayer & a_Player, const sSetBlock & a_BlockChange) override;
	virtual bool CallHookPlayerPlacingBlock       (cPlayer & a_Player, const sSetBlock & a_BlockChange) override;
	virtual bool CallHookPlayerRightClick         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool CallHookPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity) override;
	virtual bool CallHookPlayerShooting           (cPlayer & a_Player) override;
	virtual bool CallHookPlayerSpawned            (cPlayer & a_Player) override;
	virtual bool CallHookPlayerTossingItem        (cPlayer & a_Player) override;
	virtual bool CallHookPlayerUsedBlock          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool CallHookPlayerUsedItem           (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool CallHookPlayerUsingBlock         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual bool CallHookPlayerUsingItem          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;
	virtual bool CallHookPluginMessage            (cClientHandle & a_Client, const AString & a_Channel, const AString & a_Message) override;
	virtual bool CallHookPluginsLoaded            (void) override;
	virtual bool CallHookPostCrafting             (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) override;
	virtual bool CallHookPreCrafting              (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) override;
	virtual bool CallHookProjectileHitBlock       (cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos) override;
	virtual bool CallHookProjectileHitEntity      (cProjectileEntity & a_Projectile, cEntity & a_HitEntity) override;
	virtual bool CallHookServerPing               (cClientHandle & a_ClientHandle, AString & a_ServerDescription, int & a_OnlinePlayersCount, int & a_MaxPlayersCount, AString & a_Favicon) override;
	virtual bool CallHookSpawnedEntity            (cWorld & a_World, cEntity & a_Entity) override;
	virtual bool CallHookSpawnedMonster           (cWorld & a_World, cMonster & a_Monster) override;
	virtual bool CallHookSpawningEntity           (cWorld & a_World, cEntity & a_Entity) override;
	virtual bool CallHookSpawningMonster          (cWorld & a_World, cMonster & a_Monster) override;
	virtual bool CallHookTakeDamage               (cEntity & a_Receiver, TakeDamageInfo & a_TDI) override;
	virtual bool CallHookUpdatedSign              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player) override;
	virtual bool CallHookUpdatingSign             (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player) override;
	virtual bool CallHookWeatherChanged           (cWorld & a_World) override;
	virtual bool CallHookWeatherChanging          (cWorld & a_World, eWeather & a_NewWeather) override;
	virtual bool CallHookWorldStarted             (cWorld & a_World) override;
	virtual bool CallHookWorldTick                (cWorld & a_World, std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec) override;
	
	bool DisablePlugin(const AString & a_PluginName);  // tolua_export
	bool LoadPlugin   (const AString & a_PluginName);  // tolua_export

	/** Removes all hooks the specified plugin has registered */
	void RemoveHooks(cPlugin * a_Plugin);
	
	/** Removes the plugin from the internal structures and deletes its object. */
	void RemovePlugin(cPlugin * a_Plugin);
	
	/** Removes all command bindings that the specified plugin has made */
	void RemovePluginCommands(cPlugin * a_Plugin);
	
	/** Binds a command to the specified plugin. Returns true if successful, false if command already bound. */
	bool BindCommand(const AString & a_Command, cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString);  // Exported in ManualBindings.cpp, without the a_Plugin param
	
	/** Calls a_Callback for each bound command, returns true if all commands were enumerated */
	bool ForEachCommand(cCommandEnumCallback & a_Callback);  // Exported in ManualBindings.cpp
	
	/** Returns true if the command is in the command map */
	bool IsCommandBound(const AString & a_Command);  // tolua_export
	
	/** Returns the permission needed for the specified command; empty string if command not found */
	AString GetCommandPermission(const AString & a_Command);  // tolua_export
	
	/** Executes the command, as if it was requested by a_Player. Checks permissions first. Returns crExecuted if executed. */
	CommandResult ExecuteCommand(cPlayer & a_Player, const AString & a_Command);  // tolua_export
	
	/** Executes the command, as if it was requested by a_Player. Permisssions are not checked. Returns crExecuted if executed. */
	CommandResult ForceExecuteCommand(cPlayer & a_Player, const AString & a_Command);  // tolua_export
	
	/** Removes all console command bindings that the specified plugin has made */
	void RemovePluginConsoleCommands(cPlugin * a_Plugin);
	
	/** Binds a console command to the specified plugin. Returns true if successful, false if command already bound. */
	bool BindConsoleCommand(const AString & a_Command, cPlugin * a_Plugin, const AString & a_HelpString);  // Exported in ManualBindings.cpp, without the a_Plugin param
	
	/** Calls a_Callback for each bound console command, returns true if all commands were enumerated */
	bool ForEachConsoleCommand(cCommandEnumCallback & a_Callback);  // Exported in ManualBindings.cpp
	
	/** Returns true if the console command is in the command map */
	bool IsConsoleCommandBound(const AString & a_Command);  // tolua_export
	
	/** Executes the command split into a_Split, as if it was given on the console. Returns true if executed. Output is sent to the a_Output callback */
	bool ExecuteConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output, const AString & a_Command);
	
	/** Appends all commands beginning with a_Text (case-insensitive) into a_Results.
	If a_Player is not nullptr, only commands for which the player has permissions are added.
	*/
	void TabCompleteCommand(const AString & a_Text, AStringVector & a_Results, cPlayer * a_Player);
	
	/** Returns true if the specified hook type is within the allowed range */
	static bool IsValidHookType(int a_HookType);
	
	/** Calls the specified callback with the plugin object of the specified plugin.
	Returns false if plugin not found, and the value that the callback has returned otherwise. */
	bool DoWithPlugin(const AString & a_PluginName, cPluginCallback & a_Callback);
	
	/** Returns the path where individual plugins' folders are expected.
	The path doesn't end in a slash. */
	static AString GetPluginsPath(void) { return FILE_IO_PREFIX + AString("Plugins"); }  // tolua_export
	
private:
	friend class cRoot;
	
	class cCommandReg
	{
	public:
		cPlugin * m_Plugin;
		AString   m_Permission;  // Not used for console commands
		AString   m_HelpString;
	} ;
	
	typedef std::map<int, cPluginManager::PluginList> HookMap;
	typedef std::map<AString, cCommandReg> CommandMap;

	PluginList m_DisablePluginList;
	PluginMap  m_Plugins;
	HookMap    m_Hooks;
	CommandMap m_Commands;
	CommandMap m_ConsoleCommands;

	bool m_bReloadPlugins;

	cPluginManager();
	virtual ~cPluginManager();

	/** Reloads all plugins, defaulting to settings.ini for settings location */
	void ReloadPluginsNow(void);

	/** Reloads all plugins with a cIniFile object expected to be initialised to settings.ini */
	void ReloadPluginsNow(cIniFile & a_SettingsIni);

	/** Unloads all plugins */
	void UnloadPluginsNow(void);

	/** Handles writing default plugins if 'Plugins' key not found using a cIniFile object expected to be intialised to settings.ini */
	void InsertDefaultPlugins(cIniFile & a_SettingsIni);

	/** Adds the plugin into the internal list of plugins and initializes it. If initialization fails, the plugin is removed again. */
	bool AddPlugin(cPlugin * a_Plugin);

	/** Tries to match a_Command to the internal table of commands, if a match is found, the corresponding plugin is called. Returns crExecuted if the command is executed. */
	CommandResult HandleCommand(cPlayer & a_Player, const AString & a_Command, bool a_ShouldCheckPermissions);
} ;  // tolua_export




