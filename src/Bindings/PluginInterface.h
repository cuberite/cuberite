
// PluginInterface.h

// Declares the cPluginInterface class representing the interface for interacting with plugins
// This interface is the implemented by the cPluginManager





#pragma once

#include "Defines.h"




// fwd:
class cBlockEntityWithItems;
class cChunkDesc;
class cCommandOutputCallback;
class cCraftingGrid;
class cCraftingRecipe;
class cEntity;
class cHopperEntity;
class cItems;
class cMonster;
class cPickup;
class cPlayer;
class cProjectileEntity;
class cWorld;
struct TakeDamageInfo;





class cPluginInterface
{
public:
	// Force a virtual destructor in descendants
	virtual ~cPluginInterface() {}

	// Calls for individual hooks. Each returns false if the action is to continue or true if the plugin wants to abort
	virtual bool CallHookBlockSpread              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source) = 0;
	virtual bool CallHookBlockToPickups           (cWorld & a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups) = 0;
	virtual bool CallHookChat                     (cPlayer & a_Player, AString & a_Message) = 0;
	virtual bool CallHookChunkAvailable           (cWorld & a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool CallHookChunkGenerated           (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) = 0;
	virtual bool CallHookChunkGenerating          (cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc) = 0;
	virtual bool CallHookChunkUnloaded            (cWorld & a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool CallHookChunkUnloading           (cWorld & a_World, int a_ChunkX, int a_ChunkZ) = 0;
	virtual bool CallHookCollectingPickup         (cPlayer & a_Player, cPickup & a_Pickup) = 0;
	virtual bool CallHookCraftingNoRecipe         (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) = 0;
	virtual bool CallHookDisconnect               (cClientHandle & a_Client, const AString & a_Reason) = 0;
	virtual bool CallHookEntityAddEffect          (cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier) = 0;
	virtual bool CallHookEntityTeleport           (cEntity & a_Entity, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition) = 0;
	virtual bool CallHookExecuteCommand           (cPlayer * a_Player, const AStringVector & a_Split, const AString & a_EntireCommand) = 0;  // If a_Player == nullptr, it is a console cmd
	virtual bool CallHookExploded                 (cWorld & a_World, double a_ExplosionSize,   bool a_CanCauseFire,   double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) = 0;
	virtual bool CallHookExploding                (cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData) = 0;
	virtual bool CallHookHandshake                (cClientHandle & a_ClientHandle, const AString & a_Username) = 0;
	virtual bool CallHookHopperPullingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum) = 0;
	virtual bool CallHookHopperPushingItem        (cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum) = 0;
	virtual bool CallHookKilling                  (cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI) = 0;
	virtual bool CallHookLogin                    (cClientHandle & a_Client, int a_ProtocolVersion, const AString & a_Username) = 0;
	virtual bool CallHookPlayerAnimation          (cPlayer & a_Player, int a_Animation) = 0;
	virtual bool CallHookPlayerBreakingBlock      (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool CallHookPlayerBrokenBlock        (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool CallHookPlayerDestroyed          (cPlayer & a_Player) = 0;
	virtual bool CallHookPlayerEating             (cPlayer & a_Player) = 0;
	virtual bool CallHookPlayerFished             (cPlayer & a_Player, const cItems & a_Reward) = 0;
	virtual bool CallHookPlayerFishing            (cPlayer & a_Player, cItems a_Reward) = 0;
	virtual bool CallHookPlayerFoodLevelChange    (cPlayer & a_Player, int a_NewFoodLevel) = 0;
	virtual bool CallHookPlayerJoined             (cPlayer & a_Player) = 0;
	virtual bool CallHookPlayerLeftClick          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, char a_Status) = 0;
	virtual bool CallHookPlayerMoving             (cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition) = 0;
	virtual bool CallHookPlayerPlacedBlock        (cPlayer & a_Player, const sSetBlock & a_BlockChange) = 0;
	virtual bool CallHookPlayerPlacingBlock       (cPlayer & a_Player, const sSetBlock & a_BlockChange) = 0;
	virtual bool CallHookPlayerRightClick         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) = 0;
	virtual bool CallHookPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity) = 0;
	virtual bool CallHookPlayerShooting           (cPlayer & a_Player) = 0;
	virtual bool CallHookPlayerSpawned            (cPlayer & a_Player) = 0;
	virtual bool CallHookPlayerTossingItem        (cPlayer & a_Player) = 0;
	virtual bool CallHookPlayerUsedBlock          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool CallHookPlayerUsedItem           (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) = 0;
	virtual bool CallHookPlayerUsingBlock         (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) = 0;
	virtual bool CallHookPlayerUsingItem          (cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) = 0;
	virtual bool CallHookPluginMessage            (cClientHandle & a_Client, const AString & a_Channel, const AString & a_Message) = 0;
	virtual bool CallHookPluginsLoaded            (void) = 0;
	virtual bool CallHookPostCrafting             (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) = 0;
	virtual bool CallHookPreCrafting              (cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe) = 0;
	virtual bool CallHookProjectileHitBlock       (cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos) = 0;
	virtual bool CallHookProjectileHitEntity      (cProjectileEntity & a_Projectile, cEntity & a_HitEntity) = 0;
	virtual bool CallHookServerPing               (cClientHandle & a_ClientHandle, AString & a_ServerDescription, int & a_OnlinePlayersCount, int & a_MaxPlayersCount, AString & a_Favicon) = 0;
	virtual bool CallHookSpawnedEntity            (cWorld & a_World, cEntity & a_Entity) = 0;
	virtual bool CallHookSpawnedMonster           (cWorld & a_World, cMonster & a_Monster) = 0;
	virtual bool CallHookSpawningEntity           (cWorld & a_World, cEntity & a_Entity) = 0;
	virtual bool CallHookSpawningMonster          (cWorld & a_World, cMonster & a_Monster) = 0;
	virtual bool CallHookTakeDamage               (cEntity & a_Receiver, TakeDamageInfo & a_TDI) = 0;
	virtual bool CallHookUpdatedSign              (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player) = 0;
	virtual bool CallHookUpdatingSign             (cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ,       AString & a_Line1,       AString & a_Line2,       AString & a_Line3,       AString & a_Line4, cPlayer * a_Player) = 0;
	virtual bool CallHookWeatherChanged           (cWorld & a_World) = 0;
	virtual bool CallHookWeatherChanging          (cWorld & a_World, eWeather & a_NewWeather) = 0;
	virtual bool CallHookWorldStarted             (cWorld & a_World) = 0;
	virtual bool CallHookWorldTick                (cWorld & a_World, std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec) = 0;
};




