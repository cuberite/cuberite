
#pragma once

#ifndef _WIN32
	#include "BlockID.h"
#else
	enum ENUM_ITEM_ID;
#endif

#define MAX_PLAYERS 65535

#include "Simulator/SimulatorManager.h"
#include "MersenneTwister.h"
#include "ChunkMap.h"
#include "WorldStorage/WorldStorage.h"
#include "Generating/ChunkGenerator.h"
#include "Vector3i.h"
#include "Vector3f.h"
#include "ChunkSender.h"
#include "Defines.h"
#include "LightingThread.h"
#include "Item.h"





class cRedstone;
class cFireSimulator;
class cFluidSimulator;
class cSandSimulator;
class cRedstoneSimulator;
class cItem;
class cPlayer;
class cClientHandle;
class cEntity;
class cBlockEntity;
class cWorldGenerator;  // The generator that actually generates the chunks for a single world
class cChunkGenerator;  // The thread responsible for generating chunks
class cChestEntity;
class cDispenserEntity;
class cFurnaceEntity;

typedef std::list< cPlayer * > cPlayerList;

typedef cItemCallback<cPlayer>          cPlayerListCallback;
typedef cItemCallback<cEntity>          cEntityCallback;
typedef cItemCallback<cChestEntity>     cChestCallback;
typedef cItemCallback<cDispenserEntity> cDispenserCallback;
typedef cItemCallback<cFurnaceEntity>   cFurnaceCallback;






// tolua_begin
class cWorld
{
public:

	// tolua_end
	
	/// A simple RAII locker for the chunkmap - locks the chunkmap in its constructor, unlocks it in the destructor
	class cLock :
		public cCSLock
	{
		typedef cCSLock super;
	public:
		cLock(cWorld & a_World);
	} ;

	// tolua_begin

	static const char * GetClassStatic(void)
	{
		return "cWorld";
	}
	
	/// Return time in seconds
	inline static float GetTime(void)
	{
		LOGWARNING("cWorld:GetTime() is obsolete, use GetWorldAge() or GetTimeOfDay() for a specific world instead.");
		return 0;
	}
	
	int GetTicksUntilWeatherChange(void) const { return m_WeatherInterval; }
	Int64 GetWorldAge(void)  const { return m_WorldAge; }
	Int64 GetTimeOfDay(void) const { return m_TimeOfDay; }
	
	void SetTicksUntilWeatherChange(int a_WeatherInterval)
	{
		m_WeatherInterval = a_WeatherInterval;
	}

	void SetTimeOfDay(Int64 a_TimeOfDay)
	{
		m_TimeOfDay = a_TimeOfDay;
		m_TimeOfDaySecs = (double)a_TimeOfDay / 20.0;
		BroadcastTimeUpdate();
	}
	
	void SetWorldTime(Int64 a_TimeOfDay)
	{
		LOGWARNING("cWorld:SetWorldTime() is obsolete, use SetTimeOfDay() instead");
		SetTimeOfDay(a_TimeOfDay);
	}
	
	/// Returns the current game mode. Partly OBSOLETE, you should use IsGameModeXXX() functions wherever applicable
	eGameMode GetGameMode(void) const { return m_GameMode; }

	/// Returns true if the world is in Creative mode
	bool IsGameModeCreative(void) const { return (m_GameMode == gmCreative); }
	
	/// Returns true if the world is in Survival mode
	bool IsGameModeSurvival(void) const { return (m_GameMode == gmSurvival); }
	
	/// Returns true if the world is in Adventure mode
	bool IsGameModeAdventure(void) const { return (m_GameMode == gmAdventure); }
	
	bool IsPVPEnabled(void) const { return m_bEnabledPVP; }
	bool IsDeepSnowEnabled(void) const { return m_IsDeepSnowEnabled; }
	
	eDimension GetDimension(void) const { return m_Dimension; }

	/// Returns the world height at the specified coords; waits for the chunk to get loaded / generated
	int GetHeight(int a_BlockX, int a_BlockZ);
	
	// tolua_end

	/// Retrieves the world height at the specified coords; returns false if chunk not loaded / generated
	bool TryGetHeight(int a_BlockX, int a_BlockZ, int & a_Height);  // Exported in ManualBindings.cpp

	// Broadcast respective packets to all clients of the chunk where the event is taking place
	// (Please keep these alpha-sorted)
	void BroadcastAttachEntity       (const cEntity & a_Entity, const cEntity * a_Vehicle);
	void BroadcastBlockAction        (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude = NULL);
	void BroadcastBlockBreakAnimation(int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage, const cClientHandle * a_Exclude = NULL);
	void BroadcastBlockEntity    (int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude = NULL);  ///< If there is a block entity at the specified coods, sends it to all clients except a_Exclude
	void BroadcastChat               (const AString & a_Message, const cClientHandle * a_Exclude = NULL);
	void BroadcastChunkData          (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer, const cClientHandle * a_Exclude = NULL);
	void BroadcastCollectPickup      (const cPickup & a_Pickup, const cPlayer & a_Player, const cClientHandle * a_Exclude = NULL);
	void BroadcastDestroyEntity      (const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityEquipment    (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityHeadLook     (const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityLook         (const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityMetadata     (const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityRelMove      (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityRelMoveLook  (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityStatus       (const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityVelocity     (const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastPlayerAnimation    (const cPlayer & a_Player, char a_Animation, const cClientHandle * a_Exclude = NULL);
	void BroadcastPlayerListItem     (const cPlayer & a_Player, bool a_IsOnline, const cClientHandle * a_Exclude = NULL);
	void BroadcastSoundEffect        (const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = NULL);   // a_Src coords are Block * 8
	void BroadcastSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude = NULL);
	void BroadcastSpawnEntity        (cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastTeleportEntity     (const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastThunderbolt        (int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude = NULL);
	void BroadcastTimeUpdate         (const cClientHandle * a_Exclude = NULL);
	void BroadcastUseBed             (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ );
	void BroadcastWeather            (eWeather a_Weather, const cClientHandle * a_Exclude = NULL);
	
	/// If there is a block entity at the specified coords, sends it to the client specified
	void SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client);
	
	void MarkChunkDirty (int a_ChunkX, int a_ChunkZ);
	void MarkChunkSaving(int a_ChunkX, int a_ChunkZ);
	void MarkChunkSaved (int a_ChunkX, int a_ChunkZ);
	
	/** Sets the chunk data as either loaded from the storage or generated.
	a_BlockLight and a_BlockSkyLight are optional, if not present, chunk will be marked as unlighted.
	a_BiomeMap is optional, if not present, biomes will be calculated by the generator
	a_HeightMap is optional, if not present, will be calculated.
	If a_MarkDirty is set, the chunk is set as dirty (used after generating)
	*/
	void SetChunkData(
		int a_ChunkX, int a_ChunkZ,
		const BLOCKTYPE *  a_BlockTypes,
		const NIBBLETYPE * a_BlockMeta,
		const NIBBLETYPE * a_BlockLight,
		const NIBBLETYPE * a_BlockSkyLight,
		const cChunkDef::HeightMap * a_HeightMap,
		const cChunkDef::BiomeMap  * a_BiomeMap,
		cEntityList & a_Entities,
		cBlockEntityList & a_BlockEntities,
		bool a_MarkDirty
	);
	
	void ChunkLighted(
		int a_ChunkX, int a_ChunkZ,
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_SkyLight
	);
	
	bool GetChunkData      (int a_ChunkX, int a_ChunkZ, cChunkDataCallback & a_Callback);
	
	/// Gets the chunk's blocks, only the block types
	bool GetChunkBlockTypes(int a_ChunkX, int a_ChunkZ, BLOCKTYPE * a_BlockTypes);
	
	bool IsChunkValid      (int a_ChunkX, int a_ChunkZ) const;
	bool HasChunkAnyClients(int a_ChunkX, int a_ChunkZ) const;
	
	void UnloadUnusedChunks(void);    // tolua_export
	
	void CollectPickupsByPlayer(cPlayer * a_Player);

	// MOTD
	const AString & GetDescription(void) const {return m_Description; }	// FIXME: This should not be in cWorld

	// Max Players
	unsigned int GetMaxPlayers(void) const {return m_MaxPlayers; }					// tolua_export
	void SetMaxPlayers(int iMax);													// tolua_export

	void AddPlayer( cPlayer* a_Player );
	void RemovePlayer( cPlayer* a_Player );

	/// Calls the callback for each player in the list; returns true if all players processed, false if the callback aborted by returning true
 	bool ForEachPlayer(cPlayerListCallback & a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<
 	
 	/// Calls the callback for the player of the given name; returns true if the player was found and the callback called, false if player not found. Callback return ignored
 	bool DoWithPlayer(const AString & a_PlayerName, cPlayerListCallback & a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	/// Finds a player from a partial or complete player name and calls the callback - case-insensitive
	bool FindAndDoWithPlayer(const AString & a_PlayerNameHint, cPlayerListCallback & a_Callback);	// >> EXPORTED IN MANUALBINDINGS <<
	
	unsigned int GetNumPlayers();													// tolua_export
	
	// TODO: This interface is dangerous - rewrite to DoWithClosestPlayer(pos, sight, action)
	cPlayer * FindClosestPlayer(const Vector3f & a_Pos, float a_SightLimit);
	
	void SendPlayerList(cPlayer * a_DestPlayer);  // Sends playerlist to the player

	/// Adds the entity into its appropriate chunk; takes ownership of the entity ptr
	void AddEntity(cEntity * a_Entity);
	
	bool HasEntity(int a_UniqueID);
	
	/// Removes the entity, the entity ptr ownership is assumed taken by the caller
	void RemoveEntity(cEntity * a_Entity);
	
	/// Calls the callback for each entity in the entire world; returns true if all entities processed, false if the callback aborted by returning true
	bool ForEachEntity(cEntityCallback & a_Callback);  // Exported in ManualBindings.cpp
	
	/// Calls the callback for each entity in the specified chunk; returns true if all entities processed, false if the callback aborted by returning true
	bool ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback & a_Callback);  // Exported in ManualBindings.cpp

	/// Calls the callback if the entity with the specified ID is found, with the entity object as the callback param. Returns true if entity found and callback returned false.
	bool DoWithEntityByID(int a_UniqueID, cEntityCallback & a_Callback);  // Exported in ManualBindings.cpp

	/// Compares clients of two chunks, calls the callback accordingly
	void CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback);
	
	/// Adds client to a chunk, if not already present; returns true if added, false if present
	bool AddChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes client from the chunk specified
	void RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes the client from all chunks it is present in
	void RemoveClientFromChunks(cClientHandle * a_Client);
	
	/// Sends the chunk to the client specified, if the chunk is valid. If not valid, the request is postponed (ChunkSender will send that chunk when it becomes valid+lighted)
	void SendChunkTo(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes client from ChunkSender's queue of chunks to be sent
	void RemoveClientFromChunkSender(cClientHandle * a_Client);
	
	/// Touches the chunk, causing it to be loaded or generated
	void TouchChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunk, if not already loaded. Doesn't generate. Returns true if chunk valid (even if already loaded before)
	bool LoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunks specified. Doesn't report failure, other than chunks being !IsValid()
	void LoadChunks(const cChunkCoordsList & a_Chunks);
	
	/// Marks the chunk as failed-to-load:
	void ChunkLoadFailed(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Sets the sign text, asking plugins for permission first. a_Player is the player who this change belongs to, may be NULL. Returns true if sign text changed. Same as UpdateSign()
	bool SetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player = NULL);  // Exported in ManualBindings.cpp
	
	/// Sets the sign text, asking plugins for permission first. a_Player is the player who this change belongs to, may be NULL. Returns true if sign text changed. Same as SetSignLines()
	bool UpdateSign(int a_X, int a_Y, int a_Z, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player = NULL);	// Exported in ManualBindings.cpp

	/// Marks (a_Stay == true) or unmarks (a_Stay == false) chunks as non-unloadable. To be used only by cChunkStay!
	void ChunksStay(const cChunkCoordsList & a_Chunks, bool a_Stay = true);
	
	/// Regenerate the given chunk:
	void RegenerateChunk(int a_ChunkX, int a_ChunkZ);													// tolua_export
	
	/// Generates the given chunk, if not already generated
	void GenerateChunk(int a_ChunkX, int a_ChunkZ);													// tolua_export
	
	/// Queues a chunk for lighting; a_Callback is called after the chunk is lighted
	void QueueLightChunk(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback = NULL);
	
	bool IsChunkLighted(int a_ChunkX, int a_ChunkZ);
	
	/// Calls the callback for each chunk in the coords specified (all cords are inclusive). Returns true if all chunks have been processed successfully
	bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback);

	// tolua_begin
	void       SetBlock          (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	void       FastSetBlock      (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	BLOCKTYPE  GetBlock          (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetBlockMeta      (int a_BlockX, int a_BlockY, int a_BlockZ);
	void       SetBlockMeta      (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_MetaData);
	NIBBLETYPE GetBlockSkyLight  (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetBlockBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ);
	bool       GetBlockTypeMeta  (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);
	bool       GetBlockInfo      (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight);
	// TODO: NIBBLETYPE GetBlockActualLight(int a_BlockX, int a_BlockY, int a_BlockZ);

	// Vector3i variants:
	void       FastSetBlock(const Vector3i & a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta ) { FastSetBlock( a_Pos.x, a_Pos.y, a_Pos.z, a_BlockType, a_BlockMeta ); }
	BLOCKTYPE  GetBlock    (const Vector3i & a_Pos ) { return GetBlock( a_Pos.x, a_Pos.y, a_Pos.z ); }
	NIBBLETYPE GetBlockMeta(const Vector3i & a_Pos ) { return GetBlockMeta( a_Pos.x, a_Pos.y, a_Pos.z ); }
	void       SetBlockMeta(const Vector3i & a_Pos, NIBBLETYPE a_MetaData ) { SetBlockMeta( a_Pos.x, a_Pos.y, a_Pos.z, a_MetaData ); }
	// tolua_end
	
	/** Writes the block area into the specified coords.
	Returns true if all chunks have been processed.
	Prefer cBlockArea::Write() instead, this is the internal implementation; cBlockArea does error checking, too.
	a_DataTypes is a bitmask of cBlockArea::baXXX constants ORed together.
	*/
	bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes);
	
	// tolua_begin

	/// Spawns item pickups for each item in the list. May compress pickups if too many entities:
	void SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_FlyAwaySpeed = 1.0);
	
	/// Spawns item pickups for each item in the list. May compress pickups if too many entities. All pickups get the speed specified:
	void SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_SpeedX, double a_SpeedY, double a_SpeedZ);
	
	/// Spawns a new primed TNT entity at the specified block coords and specified fuse duration. Initial velocity is given based on the relative coefficient provided
	void SpawnPrimedTNT(double a_X, double a_Y, double a_Z, float a_FuseTimeInSec, double a_InitialVelocityCoeff = 1);

	// tolua_end

	/// Replaces world blocks with a_Blocks, if they are of type a_FilterBlockType
	void ReplaceBlocks(const sSetBlockVector & a_Blocks, BLOCKTYPE a_FilterBlockType);
	
	/// Retrieves block types of the specified blocks. If a chunk is not loaded, doesn't modify the block. Returns true if all blocks were read.
	bool GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure);
	
	// tolua_begin
	bool DigBlock   (int a_X, int a_Y, int a_Z);
	void SendBlockTo(int a_X, int a_Y, int a_Z, cPlayer * a_Player );

	double GetSpawnX(void) const { return m_SpawnX; }
	double GetSpawnY(void) const { return m_SpawnY; }
	double GetSpawnZ(void) const { return m_SpawnZ; }

	/// Wakes up the simulators for the specified block
	void WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Wakes up the simulators for the specified area of blocks
	void WakeUpSimulatorsInArea(int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ);

	// tolua_end

	inline cSimulatorManager * GetSimulatorManager(void) { return m_SimulatorManager; }
	
	inline cFluidSimulator * GetWaterSimulator(void) { return m_WaterSimulator; }
	inline cFluidSimulator * GetLavaSimulator (void) { return m_LavaSimulator; }
	
	/// Calls the callback for each chest in the specified chunk; returns true if all chests processed, false if the callback aborted by returning true
	bool ForEachChestInChunk  (int a_ChunkX, int a_ChunkZ, cChestCallback &   a_Callback);  // Exported in ManualBindings.cpp

	/// Calls the callback for each dispenser in the specified chunk; returns true if all dispensers processed, false if the callback aborted by returning true
	bool ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback & a_Callback);

	/// Calls the callback for each dropper in the specified chunk; returns true if all droppers processed, false if the callback aborted by returning true
	bool ForEachDropperInChunk(int a_ChunkX, int a_ChunkZ, cDropperCallback & a_Callback);

	/// Calls the callback for each dropspenser in the specified chunk; returns true if all dropspensers processed, false if the callback aborted by returning true
	bool ForEachDropSpenserInChunk(int a_ChunkX, int a_ChunkZ, cDropSpenserCallback & a_Callback);

	/// Calls the callback for each furnace in the specified chunk; returns true if all furnaces processed, false if the callback aborted by returning true
	bool ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback & a_Callback);  // Exported in ManualBindings.cpp
	
	/// Does an explosion with the specified strength at the specified coordinate
	void DoExplosiontAt  (float a_ExplosionSzie, int a_BlockX, int a_BlockY, int a_BlockZ);  //

	/// Calls the callback for the chest at the specified coords; returns false if there's no chest at those coords, true if found
	bool DoWithChestAt  (int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback &   a_Callback);  // Exported in ManualBindings.cpp

	/// Calls the callback for the dispenser at the specified coords; returns false if there's no dispenser at those coords or callback returns true, returns true if found
	bool DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback & a_Callback);  // Exported in ManualBindings.cpp

	/// Calls the callback for the dropper at the specified coords; returns false if there's no dropper at those coords or callback returns true, returns true if found
	bool DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback & a_Callback);  // Exported in ManualBindings.cpp

	/// Calls the callback for the dropspenser at the specified coords; returns false if there's no dropspenser at those coords or callback returns true, returns true if found
	bool DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback & a_Callback);  // Exported in ManualBindings.cpp

	/// Calls the callback for the furnace at the specified coords; returns false if there's no furnace at those coords or callback returns true, returns true if found
	bool DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback & a_Callback);  // Exported in ManualBindings.cpp
	
	/// Retrieves the test on the sign at the specified coords; returns false if there's no sign at those coords, true if found
	bool GetSignLines (int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4);  // tolua_export
	
	/// a_Player is using block entity at [x, y, z], handle that:
	void UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) {m_ChunkMap->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ); }
	
	/// Calls the callback for the chunk specified, with ChunkMapCS locked; returns false if the chunk doesn't exist, otherwise returns the same value as the callback
	bool DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback & a_Callback);

	void GrowTree           (int a_BlockX, int a_BlockY, int a_BlockZ);                           // tolua_export
	void GrowTreeFromSapling(int a_BlockX, int a_BlockY, int a_BlockZ, char a_SaplingMeta);       // tolua_export
	void GrowTreeByBiome    (int a_BlockX, int a_BlockY, int a_BlockZ);                           // tolua_export
	
	void GrowTreeImage(const sSetBlockVector & a_Blocks);
	
	/// Grows the plant at the specified block to its ripe stage (bonemeal used); returns false if the block is not growable. If a_IsBonemeal is true, block is not grown if not allowed in world.ini
	bool GrowRipePlant(int a_BlockX, int a_BlockY, int a_BlockZ, bool a_IsByBonemeal = false);    // tolua_export
	
	/// Grows a cactus present at the block specified by the amount of blocks specified, up to the max height specified in the config
	void GrowCactus(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow);  // tolua_export

	/// Grows a melon or a pumpkin next to the block specified (assumed to be the stem)
	void GrowMelonPumpkin(int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockType);    // tolua_export

	/// Grows a sugarcane present at the block specified by the amount of blocks specified, up to the max height specified in the config
	void GrowSugarcane(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow);  // tolua_export
	
	int  GetBiomeAt (int a_BlockX, int a_BlockZ);   // tolua_export

	const AString & GetName(void) const { return m_WorldName; }									// tolua_export
	const AString & GetIniFileName(void) const {return m_IniFileName; }

	inline static void AbsoluteToRelative( int & a_X, int & a_Y, int & a_Z, int & a_ChunkX, int & a_ChunkY, int & a_ChunkZ )
	{
		// TODO: Use floor() instead of weird if statements
		// Also fix Y
		a_ChunkX = a_X/cChunkDef::Width;
		if(a_X < 0 && a_X % cChunkDef::Width != 0) a_ChunkX--;
		a_ChunkY = 0;
		a_ChunkZ = a_Z/cChunkDef::Width;
		if(a_Z < 0 && a_Z % cChunkDef::Width != 0) a_ChunkZ--;

		a_X = a_X - a_ChunkX*cChunkDef::Width;
		a_Y = a_Y - a_ChunkY*cChunkDef::Height;
		a_Z = a_Z - a_ChunkZ*cChunkDef::Width;
	}
	
	inline static void BlockToChunk( int a_X, int a_Y, int a_Z, int & a_ChunkX, int & a_ChunkY, int & a_ChunkZ )
	{
		// TODO: Use floor() instead of weird if statements
		// Also fix Y
		(void)a_Y; // not unused anymore
		a_ChunkX = a_X/cChunkDef::Width;
		if(a_X < 0 && a_X % cChunkDef::Width != 0) a_ChunkX--;
		a_ChunkY = 0;
		a_ChunkZ = a_Z/cChunkDef::Width;
		if(a_Z < 0 && a_Z % cChunkDef::Width != 0) a_ChunkZ--;
	}

	void SaveAllChunks(void);			// tolua_export

	/// Returns the number of chunks loaded	
	int GetNumChunks() const;		// tolua_export

	/// Returns the number of chunks loaded and dirty, and in the lighting queue
	void GetChunkStats(int & a_NumValid, int & a_NumDirty, int & a_NumInLightingQueue);

	// Various queues length queries (cannot be const, they lock their CS):
	inline int GetGeneratorQueueLength  (void) { return m_Generator.GetQueueLength();   }    // tolua_export
	inline int GetLightingQueueLength   (void) { return m_Lighting.GetQueueLength();    }    // tolua_export
	inline int GetStorageLoadQueueLength(void) { return m_Storage.GetLoadQueueLength(); }    // tolua_export
	inline int GetStorageSaveQueueLength(void) { return m_Storage.GetSaveQueueLength(); }    // tolua_export

	void Tick(float a_Dt);

	void InitializeSpawn(void);
	
	/// Stops threads that belong to this world (part of deinit)
	void StopThreads(void);
	void TickQueuedBlocks(float a_Dt);

	struct BlockTickQueueItem
	{
		int X;
		int Y;
		int Z;
		float ToWait;
	};

	void QueueBlockForTick(int a_BlockX, int a_BlockY, int a_BlockZ, float a_TimeToWait);  // tolua_export

	// tolua_begin
	/// Casts a thunderbolt at the specified coords
	void CastThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Sets the specified weather; resets weather interval; asks and notifies plugins of the change
	void SetWeather     (eWeather a_NewWeather);
	
	/// Forces a weather change in the next game tick
	void ChangeWeather  (void);
	
	/// Returns the current weather
	eWeather GetWeather     (void) const { return m_Weather; };
	// tolua_end

	cChunkGenerator & GetGenerator(void) { return m_Generator; }
	cWorldStorage &   GetStorage  (void) { return m_Storage; }
	cChunkMap *       GetChunkMap (void) { return m_ChunkMap; }
		
	/// Sets the blockticking to start at the specified block. Only one blocktick per chunk may be set, second call overwrites the first call
	void SetNextBlockTick(int a_BlockX, int a_BlockY, int a_BlockZ);  // tolua_export
	
	int GetMaxSugarcaneHeight(void) const { return m_MaxSugarcaneHeight; }  // tolua_export
	int GetMaxCactusHeight   (void) const { return m_MaxCactusHeight; }     // tolua_export

	bool IsBlockDirectlyWatered(int a_BlockX, int a_BlockY, int a_BlockZ);  // tolua_export
	
	/// Spawns a mob of the specified entity type. Returns the mob's EntityID if recognized and spawned, <0 otherwise
	int SpawnMob(double a_PosX, double a_PosY, double a_PosZ, int a_EntityType);  // tolua_export
	
	/// Returns a random number from the m_TickRand in range [0 .. a_Range]. To be used only in the tick thread!
	int GetTickRandomNumber(unsigned a_Range) { return (int)(m_TickRand.randInt(a_Range)); }
	
	/// Appends all usernames starting with a_Text (case-insensitive) into Results
	void TabCompleteUserName(const AString & a_Text, AStringVector & a_Results);
	
private:

	friend class cRoot;

	/// The dimension of the world, used by the client to provide correct lighting scheme
	eDimension m_Dimension;
	
	/// This random generator is to be used only in the Tick() method, and thus only in the World-Tick-thread (MTRand is not exactly thread-safe)
	MTRand m_TickRand;

	double m_SpawnX;
	double m_SpawnY;
	double m_SpawnZ;

	double m_WorldAgeSecs;      // World age, in seconds. Is only incremented, cannot be set by plugins.
	double m_TimeOfDaySecs;     // Time of day in seconds. Can be adjusted. Is wrapped to zero each day.
	Int64  m_WorldAge;          // World age in ticks, calculated off of m_WorldAgeSecs
	Int64  m_TimeOfDay;         // Time in ticks, calculated off of m_TimeOfDaySecs
	Int64  m_LastTimeUpdate;    // The tick in which the last time update has been sent.
	Int64  m_LastUnload;        // The last WorldAge (in ticks) in which unloading was triggerred
	Int64  m_LastSave;          // The last WorldAge (in ticks) in which save-all was triggerred
	Int64  m_LastSpawnMonster;  // The last WorldAge (in ticks) in which a monster was spawned

	eGameMode m_GameMode;
	bool m_bEnabledPVP;
	bool m_IsDeepSnowEnabled;

	// The cRedstone class simulates redstone and needs access to m_RSList
	// friend class cRedstone;
	std::vector<int> m_RSList;
	
	std::vector<BlockTickQueueItem *> m_BlockTickQueue;
	std::vector<BlockTickQueueItem *> m_BlockTickQueueCopy;	//Second is for safely removing the objects from the queue

	cSimulatorManager *  m_SimulatorManager;
	cSandSimulator *     m_SandSimulator;
	cFluidSimulator *    m_WaterSimulator;
	cFluidSimulator *    m_LavaSimulator;
	cFireSimulator *     m_FireSimulator;
	cRedstoneSimulator * m_RedstoneSimulator;
	
	cCriticalSection m_CSPlayers;
	cPlayerList      m_Players;

	cWorldStorage     m_Storage;
	
	AString m_Description;
	
	unsigned int m_MaxPlayers;

	cChunkMap * m_ChunkMap;

	bool m_bAnimals;
	Int64 m_SpawnMonsterRate;

	eWeather m_Weather;
	int m_WeatherInterval;
	
	int  m_MaxCactusHeight;
	int  m_MaxSugarcaneHeight;
	bool m_IsCactusBonemealable;
	bool m_IsCarrotsBonemealable;
	bool m_IsCropsBonemealable;
	bool m_IsGrassBonemealable;
	bool m_IsMelonStemBonemealable;
	bool m_IsMelonBonemealable;
	bool m_IsPotatoesBonemealable;
	bool m_IsPumpkinStemBonemealable;
	bool m_IsPumpkinBonemealable;
	bool m_IsSaplingBonemealable;
	bool m_IsSugarcaneBonemealable;
	
	cCriticalSection m_CSFastSetBlock;
	sSetBlockList    m_FastSetBlockQueue;

	cChunkGenerator  m_Generator;
	
	cChunkSender     m_ChunkSender;
	cLightingThread  m_Lighting;

	AString m_WorldName;
	AString m_IniFileName;
	
	cWorld(const AString & a_WorldName);
	~cWorld();

	void TickWeather(float a_Dt);  // Handles weather each tick
	void TickSpawnMobs(float a_Dt);  // Handles mob spawning each tick
	
	/// Creates a new fluid simulator, loads its settings from the inifile (a_FluidName section)
	cFluidSimulator * InitializeFluidSimulator(cIniFile & a_IniFile, const char * a_FluidName, BLOCKTYPE a_SimulateBlock, BLOCKTYPE a_StationaryBlock);
}; // tolua_export




