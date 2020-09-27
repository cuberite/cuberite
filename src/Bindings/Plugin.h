
// Plugin.h

// Declares the cPlugin class representing an interface that a plugin implementation needs to expose, with some helping functions





#pragma once

#include "../Defines.h"
#include "PluginManager.h"




// tolua_begin
class cPlugin
{
public:
	// tolua_end

	/** Creates a new instance.
	a_FolderName is the name of the folder (in the Plugins folder) from which the plugin is loaded.
	The plugin's name defaults to the folder name. */
	cPlugin(const AString & a_FolderName);

	virtual ~cPlugin();

	/** Called as the last call into the plugin before it is unloaded. */
	virtual void OnDisable(void) {}

	/** Loads and initializes the plugin. Sets m_Status to psLoaded or psError accordingly.
	Returns true if the initialization succeeded, false otherwise. */
	virtual bool Load(void) = 0;

	/** Unloads the plugin. Sets m_Status to psDisabled.
	The default implementation removes the plugin's associations with cPluginManager, descendants should call it as well. */
	virtual void Unload(void);

	// Called each tick
	virtual void Tick(float a_Dt) = 0;

	/** Calls the specified hook with the params given. Returns the bool that the hook callback returns. */
	virtual bool OnBlockSpread              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source) = 0;
	virtual bool OnBlockToPickups           (cWorld & a_World, Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, const cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool, cItems & a_Pickups) = 0;
	virtual bool OnBrewingCompleting        (cWorld & a_World, cBrewingstandEntity & a_BrewingstandEntity) = 0;
	virtual bool OnBrewingCompleted         (cWorld & a_World, cBrewingstandEntity & a_BrewingstandEntity) = 0;
	virtual bool OnChat                     (cPlayer & a_Player, AString & a_Message) = 0;
	virtual bool OnChunkAvailable           (cWorld & a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool OnChunkGenerated           (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) = 0;
	virtual bool OnChunkGenerating          (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) = 0;
	virtual bool OnChunkUnloaded            (cWorld & a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool OnChunkUnloading           (cWorld & a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool OnCollectingPickup         (cPlayer & a_Player, cPickup & a_Pickup) = 0;
	virtual bool OnCraftingNoRecipe         (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) = 0;
	virtual bool OnDisconnect               (cClientHandle & a_Client, const AString & a_Reason) = 0;
	virtual bool OnEntityAddEffect          (cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier) = 0;
	virtual bool OnEntityTeleport           (cEntity & a_Entity, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition) = 0;
	virtual bool OnEntityChangingWorld      (cEntity & a_Entity, cWorld & a_World) = 0;
	virtual bool OnEntityChangedWorld       (cEntity & a_Entity, cWorld & a_World) = 0;
	virtual bool OnExecuteCommand           (cPlayer * a_Player, const AStringVector & a_Split, const AString & a_EntireCommand, cPluginManager::CommandResult & a_Result) = 0;
	virtual bool OnExploded                 (cWorld & a_World, double a_ExplosionSize,   bool a_CanCauseFire,   double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) = 0;
	virtual bool OnExploding                (cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) = 0;
	virtual bool OnHandshake                (cClientHandle & a_Client, const AString & a_Username) = 0;
	virtual bool OnHopperPullingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum) = 0;
	virtual bool OnHopperPushingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum) = 0;
	virtual bool OnDropSpense               (cWorld & a_World, cDropSpenserEntity & a_DropSpenser, int a_SlotNum) = 0;
	virtual bool OnKilled                   (cEntity & a_Victim, TakeDamageInfo & a_TDI, AString & a_DeathMessage) = 0;
	virtual bool OnKilling                  (cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI) = 0;
	virtual bool OnLogin                    (cClientHandle & a_Client, UInt32 a_ProtocolVersion, const AString & a_Username) = 0;
	virtual bool OnLoginForge               (cClientHandle & a_Client, const AStringMap & a_Mods) = 0;
	virtual bool OnPlayerAnimation          (cPlayer & a_Player, int a_Animation) = 0;
	virtual bool OnPlayerBreakingBlock      (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool OnPlayerBrokenBlock        (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool OnPlayerDestroyed          (cPlayer & a_Player) = 0;
	virtual bool OnPlayerEating             (cPlayer & a_Player) = 0;
	virtual bool OnPlayerFished             (cPlayer & a_Player, const cItems & a_Reward) = 0;
	virtual bool OnPlayerFishing            (cPlayer & a_Player, cItems & a_Reward) = 0;
	virtual bool OnPlayerFoodLevelChange    (cPlayer & a_Player, int a_NewFoodLevel) = 0;
	virtual bool OnPlayerJoined             (cPlayer & a_Player) = 0;
	virtual bool OnPlayerLeftClick          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status) = 0;
	virtual bool OnPlayerMoving             (cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition, bool a_PreviousIsOnGround) = 0;
	virtual bool OnPlayerOpeningWindow      (cPlayer & a_Player, cWindow & a_Window) = 0;
	virtual bool OnPlayerPlacedBlock        (cPlayer & a_Player, const sSetBlock & a_BlockChange) = 0;
	virtual bool OnPlayerPlacingBlock       (cPlayer & a_Player, const sSetBlock & a_BlockChange) = 0;
	virtual bool OnPlayerCrouched           (cPlayer & a_Player) = 0;
	virtual bool OnPlayerRightClick         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) = 0;
	virtual bool OnPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity) = 0;
	virtual bool OnPlayerShooting           (cPlayer & a_Player) = 0;
	virtual bool OnPlayerSpawned            (cPlayer & a_Player) = 0;
	virtual bool OnPlayerTossingItem        (cPlayer & a_Player) = 0;
	virtual bool OnPlayerUsedBlock          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool OnPlayerUsedItem           (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) = 0;
	virtual bool OnPlayerUsingBlock         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool OnPlayerUsingItem          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) = 0;
	virtual bool OnPluginMessage            (cClientHandle & a_Client, const AString & a_Channel, const AString & a_Message) = 0;
	virtual bool OnPluginsLoaded            (void) = 0;
	virtual bool OnPostCrafting             (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) = 0;
	virtual bool OnPreCrafting              (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) = 0;
	virtual bool OnProjectileHitBlock       (cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos) = 0;
	virtual bool OnProjectileHitEntity      (cProjectileEntity & a_Projectile, cEntity & a_HitEntity) = 0;
	virtual bool OnServerPing               (cClientHandle & a_ClientHandle, AString & a_ServerDescription, int & a_OnlinePlayersCount, int & a_MaxPlayersCount, AString & a_Favicon) = 0;
	virtual bool OnSpawnedEntity            (cWorld & a_World, cEntity & a_Entity) = 0;
	virtual bool OnSpawnedMonster           (cWorld & a_World, cMonster & a_Monster) = 0;
	virtual bool OnSpawningEntity           (cWorld & a_World, cEntity & a_Entity) = 0;
	virtual bool OnSpawningMonster          (cWorld & a_World, cMonster & a_Monster) = 0;
	virtual bool OnTakeDamage               (cEntity & a_Receiver, TakeDamageInfo & a_TakeDamageInfo) = 0;
	virtual bool OnUpdatedSign              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player) = 0;
	virtual bool OnUpdatingSign             (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player) = 0;
	virtual bool OnWeatherChanged           (cWorld & a_World) = 0;
	virtual bool OnWeatherChanging          (cWorld & a_World, eWeather & a_NewWeather) = 0;
	virtual bool OnWorldStarted             (cWorld & a_World) = 0;
	virtual bool OnWorldTick                (cWorld & a_World, std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec) = 0;

	// tolua_begin
	const AString & GetName(void) const  { return m_Name; }
	void SetName(const AString & a_Name) { m_Name = a_Name; }

	int GetVersion(void) const     { return m_Version; }
	void SetVersion(int a_Version) { m_Version = a_Version; }

	/** Returns the name of the folder (in the Plugins folder) from which the plugin is loaded. */
	const AString & GetFolderName(void) const {return m_FolderName; }

	/** Returns the folder relative to the MCS Executable, from which the plugin is loaded. */
	AString GetLocalFolder(void) const;

	/** Returns the error encountered while loading the plugin. Only valid if m_Status == psError. */
	const AString & GetLoadError(void) const { return m_LoadError; }

	cPluginManager::ePluginStatus GetStatus(void) const { return m_Status; }

	bool IsLoaded(void) const { return (m_Status == cPluginManager::psLoaded); }
	// tolua_end

	// Needed for ManualBindings' tolua_ForEach<>
	static const char * GetClassStatic(void) { return "cPlugin"; }

protected:
	friend class cPluginManager;

	cPluginManager::ePluginStatus m_Status;

	/** The name of the plugin, used to identify the plugin in the system and for inter-plugin calls. */
	AString m_Name;

	int m_Version;

	/** Name of the folder (in the Plugins folder) from which the plugin is loaded. */
	AString m_FolderName;

	/** The error encountered while loading the plugin.
	Only valid if m_Status == psError. */
	AString m_LoadError;


	/** Sets m_LoadError to the specified string and m_Status to psError. */
	void SetLoadError(const AString & a_LoadError);
};  // tolua_export




