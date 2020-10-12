
#pragma once


#include "../BlockType.h"
#include "../Defines.h"
#include "../FunctionRef.h"





// fwd:
class cBlockEntityWithItems;
class cBrewingstandEntity;
class cChunkDesc;
class cClientHandle;
class cCommandOutputCallback;
class cCraftingGrid;
class cCraftingRecipe;
class cDropSpenserEntity;
class cEntity;
class cHopperEntity;
class cItems;
class cMonster;
class cPickup;
class cPlayer;
class cPlugin;
class cProjectileEntity;
class cWindow;
class cWorld;
class cSettingsRepositoryInterface;
class cDeadlockDetect;
struct TakeDamageInfo;

typedef std::shared_ptr<cPlugin> cPluginPtr;
typedef std::vector<cPluginPtr> cPluginPtrs;





// tolua_begin
class cPluginManager
{
public:

	enum CommandResult
	{
		crExecuted,
		crUnknownCommand,
		crError,
		crBlocked,
		crNoPermission,
	} ;


	/** Defines the status of a single plugin - whether it is loaded, disabled or errored. */
	enum ePluginStatus
	{
		/** The plugin has been loaded successfully. */
		psLoaded,

		/** The plugin is disabled in settings.ini. */
		psDisabled,

		/** The plugin is enabled in settings.ini but has been unloaded (by a command). */
		psUnloaded,

		/** The plugin is enabled in settings.ini but has failed to load.
		m_LoadError is the description of the error. */
		psError,

		/** The plugin has been loaded before, but after a folder refresh it is no longer present.
		The plugin will be unloaded in the next call to ReloadPlugins(). */
		psNotFound,
	};


	enum PluginHook
	{
		HOOK_BLOCK_SPREAD,
		HOOK_BLOCK_TO_PICKUPS,
		HOOK_BREWING_COMPLETING,
		HOOK_BREWING_COMPLETED,
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
		HOOK_ENTITY_CHANGING_WORLD,
		HOOK_ENTITY_CHANGED_WORLD,
		HOOK_EXECUTE_COMMAND,
		HOOK_EXPLODED,
		HOOK_EXPLODING,
		HOOK_HANDSHAKE,
		HOOK_HOPPER_PULLING_ITEM,
		HOOK_HOPPER_PUSHING_ITEM,
		HOOK_DROPSPENSE,
		HOOK_KILLED,
		HOOK_KILLING,
		HOOK_LOGIN,
		HOOK_LOGIN_FORGE,
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
		HOOK_PLAYER_OPENING_WINDOW,
		HOOK_PLAYER_PLACED_BLOCK,
		HOOK_PLAYER_PLACING_BLOCK,
		HOOK_PLAYER_CROUCHED,
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

		// tolua_end

		// Note that if a hook type is added, it may need processing in cPlugin::CanAddHook() descendants,
		//   and it definitely needs adding in cPluginLua::GetHookFnName() !

		// Keep these two as the last items, they are used for validity checking and get their values automagically
		HOOK_NUM_HOOKS,
		HOOK_MAX = HOOK_NUM_HOOKS - 1,
	} ;  // tolua_export


	/** Defines the deferred actions needed for a plugin */
	enum class PluginAction
	{
		Reload,
		Unload
	};


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


	/** Interface that must be provided by any class that implements a command handler, either in-game or console command. */
	class cCommandHandler
	{
	public:
		// Force a virtual destructor in descendants
		virtual ~cCommandHandler() {}

		/** Executes the specified in-game command.
		a_Split is the command string, split at the spaces.
		a_Player is the player executing the command, nullptr in case of the console.
		a_Command is the entire command string.
		a_Output is the sink into which the additional text returned by the command handler should be sent; only used for console commands. */
		virtual bool ExecuteCommand(
			const AStringVector & a_Split,
			cPlayer * a_Player,
			const AString & a_Command,
			cCommandOutputCallback * a_Output = nullptr
		) = 0;
	};

	typedef std::shared_ptr<cCommandHandler> cCommandHandlerPtr;


	/** The interface used for enumerating and extern-calling plugins */
	using cPluginCallback = cFunctionRef<bool(cPlugin &)>;

	typedef std::list<cPlugin *> PluginList;


	/** Called each tick, calls the plugins' OnTick hook, as well as processes plugin events (addition, removal) */
	void Tick(float a_Dt);

	/** Returns the instance of the Plugin Manager (there is only ever one) */
	static cPluginManager * Get(void);  // tolua_export

	/** Refreshes the m_Plugins list based on the current contents of the Plugins folder.
	If an active plugin's folder is not found anymore, the plugin is set as psNotFound, but not yet unloaded. */
	void RefreshPluginList();  // tolua_export

	/** Schedules a reload of the plugins to happen within the next call to Tick(). */
	void ReloadPlugins();  // tolua_export

	/** Adds the plugin to the list of plugins called for the specified hook type.
	If a plugin adds multiple handlers for a single hook, it is added only once (ignore-duplicates). */
	void AddHook(cPlugin * a_Plugin, int a_HookType);

	/** Returns the number of all plugins in m_Plugins (includes disabled, unloaded and errored plugins). */
	size_t GetNumPlugins() const;  // tolua_export

	/** Returns the number of plugins that are psLoaded. */
	size_t GetNumLoadedPlugins(void) const;  // tolua_export

	// Calls for individual hooks. Each returns false if the action is to continue or true if the plugin wants to abort
	bool CallHookBlockSpread              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source);
	bool CallHookBlockToPickups           (cWorld & a_World, Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool, cItems & a_Pickups);
	bool CallHookBrewingCompleting        (cWorld & a_World, cBrewingstandEntity & a_Brewingstand);
	bool CallHookBrewingCompleted         (cWorld & a_World, cBrewingstandEntity & a_Brewingstand);
	bool CallHookChat                     (cPlayer & a_Player, AString & a_Message);
	bool CallHookChunkAvailable           (cWorld & a_World, int a_ChunkX, int a_ChunkZ);
	bool CallHookChunkGenerated           (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc);
	bool CallHookChunkGenerating          (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc);
	bool CallHookChunkUnloaded            (cWorld & a_World, int a_ChunkX, int a_ChunkZ);
	bool CallHookChunkUnloading           (cWorld & a_World, int a_ChunkX, int a_ChunkZ);
	bool CallHookCollectingPickup         (cPlayer & a_Player, cPickup & a_Pickup);
	bool CallHookCraftingNoRecipe         (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe);
	bool CallHookDisconnect               (cClientHandle & a_Client, const AString & a_Reason);
	bool CallHookEntityAddEffect          (cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier);
	bool CallHookEntityTeleport           (cEntity & a_Entity, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition);
	bool CallHookEntityChangingWorld      (cEntity & a_Entity, cWorld & a_World);
	bool CallHookEntityChangedWorld       (cEntity & a_Entity, cWorld & a_World);
	bool CallHookExecuteCommand           (cPlayer * a_Player, const AStringVector & a_Split, const AString & a_EntireCommand, CommandResult & a_Result);  // If a_Player == nullptr, it is a console cmd
	bool CallHookExploded                 (cWorld & a_World, double a_ExplosionSize,   bool a_CanCauseFire,   double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData);
	bool CallHookExploding                (cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData);
	bool CallHookHandshake                (cClientHandle & a_ClientHandle, const AString & a_Username);
	bool CallHookHopperPullingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum);
	bool CallHookHopperPushingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum);
	bool CallHookDropSpense               (cWorld & a_World, cDropSpenserEntity & a_DropSpenser, int a_SlotNum);
	bool CallHookKilled                   (cEntity & a_Victim, TakeDamageInfo & a_TDI, AString & a_DeathMessage);
	bool CallHookKilling                  (cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI);
	bool CallHookLogin                    (cClientHandle & a_Client, UInt32 a_ProtocolVersion, const AString & a_Username);
	bool CallHookLoginForge               (cClientHandle & a_Client, AStringMap & a_Mods);
	bool CallHookPlayerAnimation          (cPlayer & a_Player, int a_Animation);
	bool CallHookPlayerBreakingBlock      (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerBrokenBlock        (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool CallHookPlayerDestroyed          (cPlayer & a_Player);
	bool CallHookPlayerEating             (cPlayer & a_Player);
	bool CallHookPlayerFished             (cPlayer & a_Player, const cItems & a_Reward);
	bool CallHookPlayerFishing            (cPlayer & a_Player, cItems a_Reward);
	bool CallHookPlayerFoodLevelChange    (cPlayer & a_Player, int a_NewFoodLevel);
	bool CallHookPlayerJoined             (cPlayer & a_Player);
	bool CallHookPlayerLeftClick          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status);
	bool CallHookPlayerMoving             (cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition, bool a_PreviousIsOnGround);
	bool CallHookPlayerOpeningWindow      (cPlayer & a_Player, cWindow & a_Window);
	bool CallHookPlayerPlacedBlock        (cPlayer & a_Player, const sSetBlock & a_BlockChange);
	bool CallHookPlayerPlacingBlock       (cPlayer & a_Player, const sSetBlock & a_BlockChange);
	bool CallHookPlayerCrouched           (cPlayer & a_Player);
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
	bool CallHookPostCrafting             (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe);
	bool CallHookPreCrafting              (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe);
	bool CallHookProjectileHitBlock       (cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos);
	bool CallHookProjectileHitEntity      (cProjectileEntity & a_Projectile, cEntity & a_HitEntity);
	bool CallHookServerPing               (cClientHandle & a_ClientHandle, AString & a_ServerDescription, int & a_OnlinePlayersCount, int & a_MaxPlayersCount, AString & a_Favicon);
	bool CallHookSpawnedEntity            (cWorld & a_World, cEntity & a_Entity);
	bool CallHookSpawnedMonster           (cWorld & a_World, cMonster & a_Monster);
	bool CallHookSpawningEntity           (cWorld & a_World, cEntity & a_Entity);
	bool CallHookSpawningMonster          (cWorld & a_World, cMonster & a_Monster);
	bool CallHookTakeDamage               (cEntity & a_Receiver, TakeDamageInfo & a_TDI);
	bool CallHookUpdatedSign              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player);
	bool CallHookUpdatingSign             (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player);
	bool CallHookWeatherChanged           (cWorld & a_World);
	bool CallHookWeatherChanging          (cWorld & a_World, eWeather & a_NewWeather);
	bool CallHookWorldStarted             (cWorld & a_World);
	bool CallHookWorldTick                (cWorld & a_World, std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec);

	/** Queues the specified plugin to be unloaded in the next call to Tick().
	Note that this function returns before the plugin is unloaded, to avoid deadlocks. */
	void UnloadPlugin(const AString & a_PluginFolder);  // tolua_export

	/** Queues the specified plugin to be reloaded in the next call to Tick().
	Note that this function returns before the plugin is unloaded, to avoid deadlocks. */
	void ReloadPlugin(const AString & a_PluginFolder);  // tolua_export

	/** Loads the plugin from the specified plugin folder.
	Returns true if the plugin was loaded successfully or was already loaded before, false otherwise. */
	bool LoadPlugin(const AString & a_PluginFolder);  // tolua_export

	/** Removes all hooks the specified plugin has registered */
	void RemoveHooks(cPlugin * a_Plugin);

	/** Removes the plugin of the specified name from the internal structures and deletes its object. */
	void RemovePlugin(const AString & a_PluginName);

	/** Removes all command bindings that the specified plugin has made */
	void RemovePluginCommands(cPlugin * a_Plugin);

	/** Returns true if the specified plugin is loaded. */
	bool IsPluginLoaded(const AString & a_PluginName);  // tolua_export

	/** Binds a command to the specified handler.
	Returns true if successful, false if command already bound.
	Exported in ManualBindings.cpp. */
	bool BindCommand(
		const AString & a_Command,
		cPlugin * a_Plugin,
		cCommandHandlerPtr a_Handler,
		const AString & a_Permission,
		const AString & a_HelpString
	);

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

	/** Binds a console command to the specified handler.
	Returns true if successful, false if command already bound.
	Exported in ManualBindings.cpp. */
	bool BindConsoleCommand(
		const AString & a_Command,
		cPlugin * a_Plugin,
		cCommandHandlerPtr a_Handler,
		const AString & a_HelpString
	);

	/** Calls a_Callback for each bound console command, returns true if all commands were enumerated */
	bool ForEachConsoleCommand(cCommandEnumCallback & a_Callback);  // Exported in ManualBindings.cpp

	/** Returns true if the console command is in the command map */
	bool IsConsoleCommandBound(const AString & a_Command);  // tolua_export

	/** Executes the command split into a_Split, as if it was given on the console.
	Returns true if executed. Output is sent to the a_Output callback
	Exported in ManualBindings.cpp with a different signature. */
	bool ExecuteConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output, const AString & a_Command);

	/** Appends all commands beginning with a_Text (case-insensitive) into a_Results.
	If a_Player is not nullptr, only commands for which the player has permissions are added.
	*/
	void TabCompleteCommand(const AString & a_Text, AStringVector & a_Results, cPlayer * a_Player);

	/** Returns true if the specified hook type is within the allowed range */
	static bool IsValidHookType(int a_HookType);

	/** Calls the specified callback with the plugin object of the specified plugin.
	Returns false if plugin not found, otherwise returns the value that the callback has returned. */
	bool DoWithPlugin(const AString & a_PluginName, cPluginCallback a_Callback);

	/** Calls the specified callback for each plugin in m_Plugins.
	Returns true if all plugins have been reported, false if the callback has aborted the enumeration by returning true. */
	bool ForEachPlugin(cPluginCallback a_Callback);

	/** Returns the name of the folder (cPlugin::GetFolderName()) from which the specified plugin was loaded. */
	AString GetPluginFolderName(const AString & a_PluginName);  // tolua_export

	/** Returns the path where individual plugins' folders are expected.
	The path doesn't end in a slash. */
	static AString GetPluginsPath(void) { return "Plugins"; }  // tolua_export

private:
	friend class cRoot;

	class cCommandReg
	{
	public:
		cPlugin * m_Plugin;
		AString   m_Permission;  // Not used for console commands
		AString   m_HelpString;
		cCommandHandlerPtr m_Handler;
	} ;

	typedef std::map<int, cPluginManager::PluginList> HookMap;
	typedef std::map<AString, cCommandReg> CommandMap;


	/** FolderNames of plugins that need an action (unload, reload, ...).
	The plugins will be acted upon within the next call to Tick(), to avoid multithreading issues.
	Protected against multithreaded access by m_CSPluginsNeedAction. */
	std::vector<std::pair<PluginAction, AString>> m_PluginsNeedAction;

	/** Protects m_PluginsToUnload against multithreaded access. */
	mutable cCriticalSection m_CSPluginsNeedAction;

	/** All plugins that have been found in the Plugins folder. */
	cPluginPtrs m_Plugins;

	HookMap    m_Hooks;
	CommandMap m_Commands;
	CommandMap m_ConsoleCommands;

	/** If set to true, all the plugins will be reloaded within the next call to Tick(). */
	bool m_bReloadPlugins;

	/** The deadlock detect in which all plugins should track their CSs. */
	cDeadlockDetect & m_DeadlockDetect;


	cPluginManager(cDeadlockDetect & a_DeadlockDetect);
	virtual ~cPluginManager();

	/** Reloads all plugins, defaulting to settings.ini for settings location */
	void ReloadPluginsNow(void);

	/** Reloads all plugins with a settings repo expected to be initialised to settings.ini */
	void ReloadPluginsNow(cSettingsRepositoryInterface & a_Settings);

	/** Unloads all plugins */
	void UnloadPluginsNow(void);

	/** Handles writing default plugins if 'Plugins' key not found using a settings repo expected to be intialised to settings.ini */
	void InsertDefaultPlugins(cSettingsRepositoryInterface & a_Settings);

	/** Tries to match a_Command to the internal table of commands, if a match is found, the corresponding plugin is called. Returns crExecuted if the command is executed. */
	CommandResult HandleCommand(cPlayer & a_Player, const AString & a_Command, bool a_ShouldCheckPermissions);

	/** Returns the folders that are specified in the settings ini to load plugins from. */
	AStringVector GetFoldersToLoad(cSettingsRepositoryInterface & a_Settings);

	/** Calls a_HookFunction on each plugin registered to the hook HookName.
	Returns false if the action is to continue or true if the plugin wants to abort.
	Accessible only from within PluginManager.cpp */
	template <typename HookFunction>
	bool GenericCallHook(PluginHook a_HookName, HookFunction a_HookFunction);
} ;  // tolua_export



