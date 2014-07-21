
#pragma once

#include "PluginManager.h"





class cClientHandle;
class cPlayer;
class cPickup;
class cItem;
class cEntity;
class cWorld;
class cChunkDesc;
struct TakeDamageInfo;

// fwd: cPlayer.h
class cPlayer;

// fwd: CraftingRecipes.h
class cCraftingGrid;
class cCraftingRecipe;





// tolua_begin
class cPlugin
{
public:
	// tolua_end
	
	cPlugin( const AString & a_PluginDirectory);
	virtual ~cPlugin();
	
	virtual void OnDisable(void) {}
	virtual bool Initialize(void) = 0;

	// Called each tick
	virtual void Tick(float a_Dt) = 0;

	/** Calls the specified hook with the params given. Returns the bool that the hook callback returns.*/
	virtual bool OnBlockSpread              (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source) = 0;
	virtual bool OnBlockToPickups           (cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) = 0;
	virtual bool OnChat                     (cPlayer * a_Player, AString & a_Message) = 0;
	virtual bool OnChunkAvailable           (cWorld * a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool OnChunkGenerated           (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) = 0;
	virtual bool OnChunkGenerating          (cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) = 0;
	virtual bool OnChunkUnloaded            (cWorld * a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool OnChunkUnloading           (cWorld * a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool OnCollectingPickup         (cPlayer * a_Player, cPickup * a_Pickup) = 0;
	virtual bool OnCraftingNoRecipe         (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) = 0;
	virtual bool OnDisconnect               (cClientHandle & a_Client, const AString & a_Reason) = 0;
	virtual bool OnEntityAddEffect          (cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier) = 0;
	virtual bool OnExecuteCommand           (cPlayer * a_Player, const AStringVector & a_Split) = 0;
	virtual bool OnExploded                 (cWorld & a_World, double a_ExplosionSize,   bool a_CanCauseFire,   double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) = 0;
	virtual bool OnExploding                (cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) = 0;
	virtual bool OnHandshake                (cClientHandle * a_Client, const AString & a_Username) = 0;
	virtual bool OnHopperPullingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum) = 0;
	virtual bool OnHopperPushingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum) = 0;
	virtual bool OnKilling                  (cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI) = 0;
	virtual bool OnLogin                    (cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username) = 0;
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
	virtual bool OnPlayerMoved              (cPlayer & a_Player) = 0;
	virtual bool OnPlayerPlacedBlock        (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool OnPlayerPlacingBlock       (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
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
	virtual bool OnPostCrafting             (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) = 0;
	virtual bool OnPreCrafting              (const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe) = 0;
	virtual bool OnProjectileHitBlock       (cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos) = 0;
	virtual bool OnProjectileHitEntity      (cProjectileEntity & a_Projectile, cEntity & a_HitEntity) = 0;
	virtual bool OnSpawnedEntity            (cWorld & a_World, cEntity & a_Entity) = 0;
	virtual bool OnSpawnedMonster           (cWorld & a_World, cMonster & a_Monster) = 0;
	virtual bool OnSpawningEntity           (cWorld & a_World, cEntity & a_Entity) = 0;
	virtual bool OnSpawningMonster          (cWorld & a_World, cMonster & a_Monster) = 0;
	virtual bool OnTakeDamage               (cEntity & a_Receiver, TakeDamageInfo & a_TakeDamageInfo) = 0;
	virtual bool OnUpdatedSign              (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player) = 0;
	virtual bool OnUpdatingSign             (cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player) = 0;
	virtual bool OnWeatherChanged           (cWorld & a_World) = 0;
	virtual bool OnWeatherChanging          (cWorld & a_World, eWeather & a_NewWeather) = 0;
	virtual bool OnWorldStarted             (cWorld & a_World) = 0;
	virtual bool OnWorldTick                (cWorld & a_World, float a_Dt, int a_LastTickDurationMSec) = 0;
	
	/** Handles the command split into a_Split, issued by player a_Player.
	Command permissions have already been checked.
	Returns true if command handled successfully
	*/
	virtual bool HandleCommand(const AStringVector & a_Split, cPlayer * a_Player) = 0;
	
	/** Handles the console command split into a_Split.
	Returns true if command handled successfully. Output is to be sent to the a_Output callback.
	*/
	virtual bool HandleConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output) = 0;
	
	/// All bound commands are to be removed, do any language-dependent cleanup here
	virtual void ClearCommands(void) {} ;
	
	/// All bound console commands are to be removed, do any language-dependent cleanup here
	virtual void ClearConsoleCommands(void) {} ;
	
	// tolua_begin
	const AString & GetName(void) const  { return m_Name; }
	void SetName(const AString & a_Name) { m_Name = a_Name; }

	int GetVersion(void) const     { return m_Version; }
	void SetVersion(int a_Version) { m_Version = a_Version; }

	const AString & GetDirectory(void) const {return m_Directory; }
	AString GetLocalDirectory(void) const {return GetLocalFolder(); }  // OBSOLETE, use GetLocalFolder() instead
	AString GetLocalFolder(void) const;
	// tolua_end
	

	/* This should not be exposed to scripting languages */
	enum PluginLanguage
	{
		E_CPP,
		E_LUA,
		E_SQUIRREL,  // OBSOLETE, but kept in place to remind us of the horrors lurking in the history
	};
	PluginLanguage GetLanguage() { return m_Language; }
	void SetLanguage( PluginLanguage a_Language) { m_Language = a_Language; }

private:
	PluginLanguage m_Language;
	AString m_Name;
	int m_Version;

	AString m_Directory;
};  // tolua_export




