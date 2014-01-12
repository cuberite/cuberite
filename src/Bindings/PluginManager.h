
#pragma once

#include "../Item.h"





class cPlugin;

// fwd: World.h
class cWorld;

// fwd: ChunkDesc.h
class cChunkDesc;

// fwd: Entities/Entity.h
class cEntity;

// fwd: Mobs/Monster.h
class cMonster;

// fwd: Player.h
class cPlayer;

// fwd: CraftingRecipes.h
class cCraftingGrid;
class cCraftingRecipe;

// fwd: Pickup.h
class cPickup;

// fwd: Pawn.h
struct TakeDamageInfo;
class cPawn;

// fwd: CommandOutput.h
class cCommandOutputCallback;

// fwd: BlockEntities/HopperEntity.h
class cHopperEntity;

// fwd: BlockEntities/BlockEntityWithItems.h
class cBlockEntityWithItems;





class cPluginManager													// tolua_export
{																		// tolua_export
public:																	// tolua_export

	// Called each tick
	virtual void Tick(float a_Dt);

	// tolua_begin
	enum PluginHook
	{
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
		HOOK_PLAYER_EATING,
		HOOK_PLAYER_FISHED,
		HOOK_PLAYER_FISHING,
		HOOK_PLAYER_JOINED,
		HOOK_PLAYER_LEFT_CLICK,
		HOOK_PLAYER_MOVING,
		HOOK_PLAYER_PLACED_BLOCK,
		HOOK_PLAYER_PLACING_BLOCK,
		HOOK_PLAYER_RIGHT_CLICK,
		HOOK_PLAYER_RIGHT_CLICKING_ENTITY,
		HOOK_PLAYER_SHOOTING,
		HOOK_PLAYER_SPAWNED,
		HOOK_PLAYER_TOSSING_ITEM,
		HOOK_PLAYER_USED_BLOCK,
		HOOK_PLAYER_USED_ITEM,
		HOOK_PLAYER_USING_BLOCK,
		HOOK_PLAYER_USING_ITEM,
		HOOK_PLUGIN_MESSAGE,
		HOOK_PLUGINS_LOADED,
		HOOK_POST_CRAFTING,
		HOOK_PRE_CRAFTING,
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

	/// Used as a callback for enumerating bound commands
	class cCommandEnumCallback
	{
	public:
		/** Called for each command; return true to abort enumeration
		For console commands, a_Permission is not used (set to empty string)
		*/
		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) = 0;
	} ;
	
	/// Returns the instance of the Plugin Manager (there is only ever one)
	static cPluginManager * Get(void);							// tolua_export

	typedef std::map< AString, cPlugin * > PluginMap;
	typedef std::list< cPlugin * > PluginList;
	cPlugin * GetPlugin( const AString & a_Plugin ) const;				// tolua_export
	const PluginMap & GetAllPlugins() const;							// >> EXPORTED IN MANUALBINDINGS <<

	void FindPlugins();													// tolua_export
	void ReloadPlugins();												// tolua_export
	
	/// Adds the plugin to the list of plugins called for the specified hook type. Handles multiple adds as a single add
	void AddHook(cPlugin * a_Plugin, int a_HookType);

	unsigned int GetNumPlugins() const;  // tolua_export
	
	// Calls for individual hooks. Each returns false if the action is to continue or true if the plugin wants to abort
	bool CallHookBlockToPickups           (cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups);
	bool CallHookChat                     (cPlayer * a_Player, AString & a_Message);
	bool CallHookChunkAvailable           (cWorld * a_World, int a_ChunkX, int a_ChunkZ);
	bool CallHookChunkGenerated           (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc);
	bool CallHookChunkGenerating          (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc);
	bool CallHookChunkUnloaded            (cWorld * a_World, int a_ChunkX, int a_ChunkZ);
	bool CallHookChunkUnloading           (cWorld * a_World, int a_ChunkX, int a_ChunkZ);
	bool CallHookCollectingPickup         (cPlayer * a_Player, cPickup & a_Pickup);
	bool CallHookCraftingNoRecipe         (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookDisconnect               (cPlayer * a_Player, const AString & a_Reason);
	bool CallHookExecuteCommand           (cPlayer * a_Player, const AStringVector & a_Split);  // If a_Player == NULL, it is a console cmd
	bool CallHookExploded                 (cWorld & a_World, double a_ExplosionSize,   bool a_CanCauseFire,   double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData);
	bool CallHookExploding                (cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData);
	bool CallHookHandshake                (cClientHandle * a_ClientHandle, const AString & a_Username);
	bool CallHookHopperPullingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum);
	bool CallHookHopperPushingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum);
	bool CallHookKilling                  (cEntity & a_Victim, cEntity * a_Killer);
	bool CallHookLogin                    (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username);
	bool CallHookPlayerAnimation          (cPlayer & a_Player, int a_Animation);
	bool CallHookPlayerBreakingBlock      (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerBrokenBlock        (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerEating             (cPlayer & a_Player);
	bool CallHookPlayerFished             (cPlayer & a_Player, const cItems a_Reward);
	bool CallHookPlayerFishing            (cPlayer & a_Player, cItems a_Reward);
	bool CallHookPlayerJoined             (cPlayer & a_Player);
	bool CallHookPlayerMoving             (cPlayer & a_Player);
	bool CallHookPlayerLeftClick          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status);
	bool CallHookPlayerPlacedBlock        (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerPlacingBlock       (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerRightClick         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	bool CallHookPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity);
	bool CallHookPlayerShooting           (cPlayer & a_Player);
	bool CallHookPlayerSpawned            (cPlayer & a_Player);
	bool CallHookPlayerTossingItem        (cPlayer & a_Player);
	bool CallHookPlayerUsedBlock          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerUsedItem           (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	bool CallHookPlayerUsingBlock         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerUsingItem          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ);
	bool CallHookPluginMessage            (cClientHandle & a_Client, const AString & a_Channel, const AString & a_Message);
	bool CallHookPluginsLoaded            (void);
	bool CallHookPostCrafting             (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookPreCrafting              (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe);
	bool CallHookSpawnedEntity            (cWorld & a_World, cEntity & a_Entity);
	bool CallHookSpawnedMonster           (cWorld & a_World, cMonster & a_Monster);
	bool CallHookSpawningEntity           (cWorld & a_World, cEntity & a_Entity);
	bool CallHookSpawningMonster          (cWorld & a_World, cMonster & a_Monster);
	bool CallHookTakeDamage               (cEntity & a_Receiver, TakeDamageInfo & a_TDI);
	bool CallHookUpdatedSign              (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player);
	bool CallHookUpdatingSign             (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player);
	bool CallHookWeatherChanged           (cWorld & a_World);
	bool CallHookWeatherChanging          (cWorld & a_World, eWeather & a_NewWeather);
	bool CallHookWorldStarted             (cWorld & a_World);
	bool CallHookWorldTick                (cWorld & a_World, float a_Dt, int a_LastTickDurationMSec);
	
	bool DisablePlugin(const AString & a_PluginName);  // tolua_export
	bool LoadPlugin   (const AString & a_PluginName);  // tolua_export

	/// Removes all hooks the specified plugin has registered
	void RemoveHooks(cPlugin * a_Plugin);
	
	/// Removes the plugin from the internal structures and deletes its object.
	void RemovePlugin(cPlugin * a_Plugin);
	
	/// Removes all command bindings that the specified plugin has made
	void RemovePluginCommands(cPlugin * a_Plugin);
	
	/// Binds a command to the specified plugin. Returns true if successful, false if command already bound.
	bool BindCommand(const AString & a_Command, cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString);  // Exported in ManualBindings.cpp, without the a_Plugin param
	
	/// Calls a_Callback for each bound command, returns true if all commands were enumerated
	bool ForEachCommand(cCommandEnumCallback & a_Callback);  // Exported in ManualBindings.cpp
	
	/// Returns true if the command is in the command map
	bool IsCommandBound(const AString & a_Command);  // tolua_export
	
	/// Returns the permission needed for the specified command; empty string if command not found
	AString GetCommandPermission(const AString & a_Command);  // tolua_export
	
	/// Executes the command, as if it was requested by a_Player. Checks permissions first. Returns true if executed.
	bool ExecuteCommand(cPlayer * a_Player, const AString & a_Command);  // tolua_export
	
	/// Executes the command, as if it was requested by a_Player. Permisssions are not checked. Returns true if executed (false if not found)
	bool ForceExecuteCommand(cPlayer * a_Player, const AString & a_Command);  // tolua_export
	
	/// Removes all console command bindings that the specified plugin has made
	void RemovePluginConsoleCommands(cPlugin * a_Plugin);
	
	/// Binds a console command to the specified plugin. Returns true if successful, false if command already bound.
	bool BindConsoleCommand(const AString & a_Command, cPlugin * a_Plugin, const AString & a_HelpString);  // Exported in ManualBindings.cpp, without the a_Plugin param
	
	/// Calls a_Callback for each bound console command, returns true if all commands were enumerated
	bool ForEachConsoleCommand(cCommandEnumCallback & a_Callback);  // Exported in ManualBindings.cpp
	
	/// Returns true if the console command is in the command map
	bool IsConsoleCommandBound(const AString & a_Command);  // tolua_export
	
	/// Executes the command split into a_Split, as if it was given on the console. Returns true if executed. Output is sent to the a_Output callback
	bool ExecuteConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output);
	
	/** Appends all commands beginning with a_Text (case-insensitive) into a_Results.
	If a_Player is not NULL, only commands for which the player has permissions are added.
	*/
	void TabCompleteCommand(const AString & a_Text, AStringVector & a_Results, cPlayer * a_Player);
	
	/// Returns true if the specified hook type is within the allowed range
	static bool IsValidHookType(int a_HookType);
	
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

	/// Reloads all plugins, defaulting to settings.ini for settings location
	void ReloadPluginsNow(void);

	/// Reloads all plugins with a cIniFile object expected to be initialised to settings.ini
	void ReloadPluginsNow(cIniFile & a_SettingsIni);

	/// Unloads all plugins
	void UnloadPluginsNow(void);

	/// Handles writing default plugins if 'Plugins' key not found using a cIniFile object expected to be intialised to settings.ini
	void InsertDefaultPlugins(cIniFile & a_SettingsIni);

	/// Adds the plugin into the internal list of plugins and initializes it. If initialization fails, the plugin is removed again.
	bool AddPlugin(cPlugin * a_Plugin);

	/// Tries to match a_Command to the internal table of commands, if a match is found, the corresponding plugin is called. Returns true if the command is handled.
	bool HandleCommand(cPlayer * a_Player, const AString & a_Command, bool a_ShouldCheckPermissions, bool & a_WasCommandForbidden);	
	bool HandleCommand(cPlayer * a_Player, const AString & a_Command, bool a_ShouldCheckPermissions)
	{
		bool DummyBoolean = false;
		return HandleCommand(a_Player, a_Command, a_ShouldCheckPermissions, DummyBoolean);
	}
} ; // tolua_export




