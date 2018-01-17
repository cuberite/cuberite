
#pragma once

#define MAX_PLAYERS 65535

#include <functional>

#include "Simulator/SimulatorManager.h"
#include "ChunkMap.h"
#include "WorldStorage/WorldStorage.h"
#include "Generating/ChunkGenerator.h"
#include "ChunkSender.h"
#include "Defines.h"
#include "LightingThread.h"
#include "IniFile.h"
#include "Item.h"
#include "Mobs/Monster.h"
#include "Entities/ProjectileEntity.h"
#include "Entities/Boat.h"
#include "ForEachChunkProvider.h"
#include "Scoreboard.h"
#include "MapManager.h"
#include "Blocks/WorldInterface.h"
#include "Blocks/BroadcastInterface.h"
#include "EffectID.h"



class cFireSimulator;
class cFluidSimulator;
class cSandSimulator;
class cRedstoneSimulator;
class cItem;
class cPlayer;
class cClientHandle;
typedef std::shared_ptr<cClientHandle> cClientHandlePtr;
typedef std::list<cClientHandlePtr> cClientHandlePtrs;
typedef std::list<cClientHandle *> cClientHandles;
class cEntity;
class cChunkGenerator;  // The thread responsible for generating chunks
class cBeaconEntity;
class cBrewingstandEntity;
class cChestEntity;
class cCuboid;
class cDispenserEntity;
class cFlowerPotEntity;
class cFurnaceEntity;
class cNoteEntity;
class cMobHeadEntity;
class cCompositeChat;
class cSetChunkData;
class cBroadcaster;
class cDeadlockDetect;
class cUUID;

typedef std::list< cPlayer * > cPlayerList;
typedef std::list< std::pair< std::unique_ptr<cPlayer>, cWorld * > > cAwaitingPlayerList;

typedef std::unique_ptr<cSetChunkData> cSetChunkDataPtr;
typedef std::vector<cSetChunkDataPtr> cSetChunkDataPtrs;





// tolua_begin
class cWorld :
	public cForEachChunkProvider,
	public cWorldInterface,
	public cBroadcastInterface
{
public:

	// tolua_end

	/** A simple RAII locker for the chunkmap - locks the chunkmap in its constructor, unlocks it in the destructor */
	class cLock :
		public cCSLock
	{
		typedef cCSLock super;
	public:
		cLock(cWorld & a_World);
	};

	static const char * GetClassStatic(void)  // Needed for ManualBindings's ForEach templates
	{
		return "cWorld";
	}

	// tolua_begin

	/** Get whether saving chunks is enabled */
	bool IsSavingEnabled(void) const { return m_IsSavingEnabled; }

	/** Set whether saving chunks is enabled */
	void SetSavingEnabled(bool a_IsSavingEnabled) { m_IsSavingEnabled = a_IsSavingEnabled; }

	int GetTicksUntilWeatherChange(void) const { return m_WeatherInterval; }

	/** Is the daylight cycle enabled? */
	virtual bool IsDaylightCycleEnabled(void) const { return m_IsDaylightCycleEnabled; }

	/** Sets the daylight cycle to true / false. */
	virtual void SetDaylightCycleEnabled(bool a_IsDaylightCycleEnabled)
	{
		m_IsDaylightCycleEnabled = a_IsDaylightCycleEnabled;
		BroadcastTimeUpdate();
	}

	virtual Int64 GetWorldAge (void) const override { return std::chrono::duration_cast<cTickTimeLong>(m_WorldAge).count(); }
	virtual int GetTimeOfDay(void) const override { return std::chrono::duration_cast<cTickTime>(m_TimeOfDay).count(); }

	void SetTicksUntilWeatherChange(int a_WeatherInterval)
	{
		m_WeatherInterval = a_WeatherInterval;
	}

	virtual void SetTimeOfDay(int a_TimeOfDay) override
	{
		m_TimeOfDay = cTickTime(a_TimeOfDay);
		UpdateSkyDarkness();
		BroadcastTimeUpdate();
	}

	/** Returns the default weather interval for the specific weather type.
	Returns -1 for any unknown weather. */
	int GetDefaultWeatherInterval(eWeather a_Weather);

	/** Returns the current game mode. Partly OBSOLETE, you should use IsGameModeXXX() functions wherever applicable */
	eGameMode GetGameMode(void) const { return m_GameMode; }

	/** Returns true if the world is in Creative mode */
	bool IsGameModeCreative(void) const { return (m_GameMode == gmCreative); }

	/** Returns true if the world is in Survival mode */
	bool IsGameModeSurvival(void) const { return (m_GameMode == gmSurvival); }

	/** Returns true if the world is in Adventure mode */
	bool IsGameModeAdventure(void) const { return (m_GameMode == gmAdventure); }

	/** Returns true if the world is in Spectator mode */
	bool IsGameModeSpectator(void) const { return (m_GameMode == gmSpectator); }

	bool IsPVPEnabled(void) const { return m_bEnabledPVP; }
	bool IsDeepSnowEnabled(void) const { return m_IsDeepSnowEnabled; }

	bool ShouldLavaSpawnFire(void) const { return m_ShouldLavaSpawnFire; }

	bool VillagersShouldHarvestCrops(void) const { return m_VillagersShouldHarvestCrops; }

	virtual eDimension GetDimension(void) const override { return m_Dimension; }

	/** Returns the world height at the specified coords; waits for the chunk to get loaded / generated */
	virtual int GetHeight(int a_BlockX, int a_BlockZ) override;

	// tolua_end

	/** Retrieves the world height at the specified coords; returns false if chunk not loaded / generated */
	bool TryGetHeight(int a_BlockX, int a_BlockZ, int & a_Height);  // Exported in ManualBindings.cpp

	// Broadcast respective packets to all clients of the chunk where the event is taking place
	// (Please keep these alpha-sorted)
	void BroadcastAttachEntity       (const cEntity & a_Entity, const cEntity & a_Vehicle);
	void BroadcastBlockAction        (Vector3i a_BlockPos, Byte a_Byte1, Byte a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude = nullptr);  // tolua_export
	void BroadcastBlockAction        (int a_BlockX, int a_BlockY, int a_BlockZ, Byte a_Byte1, Byte a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude = nullptr);  // tolua_export
	void BroadcastBlockBreakAnimation(UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage, const cClientHandle * a_Exclude = nullptr);
	void BroadcastBlockEntity        (Vector3i a_BlockPos, const cClientHandle * a_Exclude = nullptr);  ///< If there is a block entity at the specified coods, sends it to all clients except a_Exclude

	// tolua_begin
	void BroadcastChat       (const AString & a_Message, const cClientHandle * a_Exclude = nullptr, eMessageType a_ChatPrefix = mtCustom);
	void BroadcastChatInfo   (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtInformation); }
	void BroadcastChatFailure(const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtFailure); }
	void BroadcastChatSuccess(const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtSuccess); }
	void BroadcastChatWarning(const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtWarning); }
	void BroadcastChatFatal  (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtFailure); }
	void BroadcastChatDeath  (const AString & a_Message, const cClientHandle * a_Exclude = nullptr) { BroadcastChat(a_Message, a_Exclude, mtDeath); }
	void BroadcastChat       (const cCompositeChat & a_Message, const cClientHandle * a_Exclude = nullptr);
	// tolua_end

	void BroadcastCollectEntity              (const cEntity & a_Pickup, const cPlayer & a_Player, int a_Count, const cClientHandle * a_Exclude = nullptr);
	void BroadcastDestroyEntity              (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastDetachEntity               (const cEntity & a_Entity, const cEntity & a_PreviousVehicle);
	void BroadcastEntityEffect               (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityEquipment            (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityHeadLook             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityLook                 (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityMetadata             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityRelMove              (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityRelMoveLook          (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityStatus               (const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityVelocity             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	virtual void BroadcastEntityAnimation    (const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude = nullptr) override;  // tolua_export
	void BroadcastLeashEntity                (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo);
	void BroadcastPlayerListAddPlayer        (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListRemovePlayer     (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListUpdateGameMode   (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListUpdatePing       (const cPlayer & a_Player, const cClientHandle * a_Exclude = nullptr);
	void BroadcastPlayerListUpdateDisplayName(const cPlayer & a_Player, const AString & a_CustomName, const cClientHandle * a_Exclude = nullptr);
	void BroadcastRemoveEntityEffect         (const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude = nullptr);
	void BroadcastScoreboardObjective        (const AString & a_Name, const AString & a_DisplayName, Byte a_Mode);
	void BroadcastScoreUpdate                (const AString & a_Objective, const AString & a_Player, cObjective::Score a_Score, Byte a_Mode);
	void BroadcastDisplayObjective           (const AString & a_Objective, cScoreboard::eDisplaySlot a_Display);
	void BroadcastSoundEffect                (const AString & a_SoundName, Vector3d a_Position, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = nullptr) override;  // tolua_export
	void BroadcastSoundEffect                (const AString & a_SoundName, double a_X, double a_Y, double a_Z, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = nullptr);  // tolua_export
	virtual void BroadcastSoundParticleEffect        (const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude = nullptr) override;  // tolua_export
	void BroadcastSpawnEntity                (cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastTeleportEntity             (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastThunderbolt                (Vector3i a_BlockPos, const cClientHandle * a_Exclude = nullptr);
	void BroadcastTimeUpdate                 (const cClientHandle * a_Exclude = nullptr);
	void BroadcastUnleashEntity              (const cEntity & a_Entity);
	virtual void BroadcastUseBed             (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ) override;
	void BroadcastWeather                    (eWeather a_Weather, const cClientHandle * a_Exclude = nullptr);

	virtual cBroadcastInterface & GetBroadcastManager(void) override
	{
		return *this;
	}

	/** If there is a block entity at the specified coords, sends it to the client specified */
	void SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client);

	void MarkChunkDirty (int a_ChunkX, int a_ChunkZ);
	void MarkChunkSaving(int a_ChunkX, int a_ChunkZ);
	void MarkChunkSaved (int a_ChunkX, int a_ChunkZ);

	/** Puts the chunk data into a queue to be set into the chunkmap in the tick thread.
	If the chunk data doesn't contain valid biomes, the biomes are calculated before adding the data into the queue. */
	void QueueSetChunkData(cSetChunkDataPtr a_SetChunkData);

	void ChunkLighted(
		int a_ChunkX, int a_ChunkZ,
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_SkyLight
	);

	bool GetChunkData      (int a_ChunkX, int a_ChunkZ, cChunkDataCallback & a_Callback);

	/** Gets the chunk's blocks, only the block types */
	bool GetChunkBlockTypes(int a_ChunkX, int a_ChunkZ, BLOCKTYPE * a_BlockTypes);

	/** Returns true iff the chunk is in the loader / generator queue. */
	bool IsChunkQueued(int a_ChunkX, int a_ChunkZ) const;

	/** Returns true iff the chunk is present and valid. */
	bool IsChunkValid(int a_ChunkX, int a_ChunkZ) const;

	bool HasChunkAnyClients(int a_ChunkX, int a_ChunkZ) const;

	/** Queues a task to unload unused chunks onto the tick thread. The prefferred way of unloading. */
	void QueueUnloadUnusedChunks(void);  // tolua_export

	void CollectPickupsByPlayer(cPlayer & a_Player);

	/** Adds the player to the world.
	Uses a queue to store the player object until the Tick thread processes the addition event.
	Also adds the player as an entity in the chunkmap, and the player's ClientHandle, if any, for ticking.
	If a_OldWorld is provided, a corresponding ENTITY_CHANGED_WORLD event is triggerred after the addition. */
	void AddPlayer(std::unique_ptr<cPlayer> a_Player, cWorld * a_OldWorld = nullptr);

	/** Removes the player from the world.
	Removes the player from the addition queue, too, if appropriate.
	If the player has a ClientHandle, the ClientHandle is removed from all chunks in the world and will not be ticked by this world anymore.
	@param a_RemoveFromChunk determines if the entity should be removed from its chunk as well. Should be false when ticking from cChunk.
	@return An owning reference to the given player. */
	std::unique_ptr<cPlayer> RemovePlayer(cPlayer & a_Player, bool a_RemoveFromChunk);

#ifdef _DEBUG
	bool IsPlayerReferencedInWorldOrChunk(cPlayer & a_Player);
#endif

	/** Calls the callback for each player in the list; returns true if all players processed, false if the callback aborted by returning true */
	virtual bool ForEachPlayer(cPlayerListCallback a_Callback) override;  // >> EXPORTED IN MANUALBINDINGS <<

	/** Calls the callback for the player of the given name; returns true if the player was found and the callback called, false if player not found.
	Callback return value is ignored. If there are multiple players of the same name, only (random) one is processed by the callback. */
	bool DoWithPlayer(const AString & a_PlayerName, cPlayerListCallback a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	/** Finds a player from a partial or complete player name and calls the callback - case-insensitive */
	bool FindAndDoWithPlayer(const AString & a_PlayerNameHint, cPlayerListCallback a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	// TODO: This interface is dangerous - rewrite to DoWithClosestPlayer(pos, sight, action)
	cPlayer * FindClosestPlayer(Vector3d a_Pos, float a_SightLimit, bool a_CheckLineOfSight = true);

	/** Finds the player over his uuid and calls the callback */
	bool DoWithPlayerByUUID(const cUUID & a_PlayerUUID, cPlayerListCallback a_Callback);  // >> EXPORTED IN MANUALBINDINGS <<

	void SendPlayerList(cPlayer * a_DestPlayer);  // Sends playerlist to the player

	/** Adds the entity into its appropriate chunk; takes ownership of the entity ptr.
	The entity is added lazily - this function only puts it in a queue that is then processed by the Tick thread. */
	void AddEntity(OwnedEntity a_Entity);

	/** Returns true if an entity with the specified UniqueID exists in the world.
	Note: Only loaded chunks are considered. */
	bool HasEntity(UInt32 a_UniqueID);

	/** Removes the entity from the world.
	Returns an owning reference to the found entity. */
	OwnedEntity RemoveEntity(cEntity & a_Entity);

	/** Calls the callback for each entity in the entire world; returns true if all entities processed, false if the callback aborted by returning true */
	bool ForEachEntity(cEntityCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for each entity in the specified chunk; returns true if all entities processed, false if the callback aborted by returning true */
	bool ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for each entity that has a nonempty intersection with the specified boundingbox.
	Returns true if all entities processed, false if the callback aborted by returning true.
	If any chunk in the box is missing, ignores the entities in that chunk silently. */
	virtual bool ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback a_Callback) override;  // Exported in ManualBindings.cpp

	/** Calls the callback if the entity with the specified ID is found, with the entity object as the callback param.
	Returns true if entity found and callback returned false. */
	bool DoWithEntityByID(UInt32 a_UniqueID, cEntityCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Compares clients of two chunks, calls the callback accordingly */
	void CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback);

	/** Adds client to a chunk, if not already present; returns true if added, false if present */
	bool AddChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);

	/** Removes client from the chunk specified */
	void RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);

	/** Removes the client from all chunks it is present in */
	void RemoveClientFromChunks(cClientHandle * a_Client);

	/** Sends the chunk to the client specified, if the client doesn't have the chunk yet.
	If chunk not valid, the request is postponed (ChunkSender will send that chunk when it becomes valid + lighted). */
	void SendChunkTo(int a_ChunkX, int a_ChunkZ, cChunkSender::eChunkPriority a_Priority, cClientHandle * a_Client);

	/** Sends the chunk to the client specified, even if the client already has the chunk.
	If the chunk's not valid, the request is postponed (ChunkSender will send that chunk when it becomes valid + lighted). */
	void ForceSendChunkTo(int a_ChunkX, int a_ChunkZ, cChunkSender::eChunkPriority a_Priority, cClientHandle * a_Client);

	/** Removes client from ChunkSender's queue of chunks to be sent */
	void RemoveClientFromChunkSender(cClientHandle * a_Client);

	/** Touches the chunk, causing it to be loaded or generated */
	void TouchChunk(int a_ChunkX, int a_ChunkZ);

	/** Queues the chunk for preparing - making sure that it's generated and lit.
	The specified chunk is queued to be loaded or generated, and lit if needed.
	The specified callback is called after the chunk has been prepared. If there's no preparation to do, only the callback is called.
	It is legal to call with no callback. */
	void PrepareChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallAfter = {});

	/** Marks the chunk as failed-to-load: */
	void ChunkLoadFailed(int a_ChunkX, int a_ChunkZ);

	/** Sets the sign text, asking plugins for permission first. a_Player is the player who this change belongs to, may be nullptr. Returns true if sign text changed. */
	bool SetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4, cPlayer * a_Player = nullptr);  // Exported in ManualBindings.cpp

	/** Sets the command block command. Returns true if command changed. */
	bool SetCommandBlockCommand(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Command);  // tolua_export

	/** Is the trapdoor open? Returns false if there is no trapdoor at the specified coords. */
	bool IsTrapdoorOpen(int a_BlockX, int a_BlockY, int a_BlockZ);                                      // tolua_export

	/** Set the state of a trapdoor. Returns true if the trapdoor was updated, false if there was no trapdoor at those coords. */
	bool SetTrapdoorOpen(int a_BlockX, int a_BlockY, int a_BlockZ, bool a_Open);                        // tolua_export

	/** Regenerate the given chunk: */
	void RegenerateChunk(int a_ChunkX, int a_ChunkZ);  // tolua_export

	/** Generates the given chunk */
	void GenerateChunk(int a_ChunkX, int a_ChunkZ);  // tolua_export

	/** Queues a chunk for lighting; a_Callback is called after the chunk is lighted */
	void QueueLightChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_Callback = {});

	bool IsChunkLighted(int a_ChunkX, int a_ChunkZ);

	/** Calls the callback for each chunk in the coords specified (all cords are inclusive). Returns true if all chunks have been processed successfully */
	virtual bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback) override;

	/** Calls the callback for each loaded chunk. Returns true if all chunks have been processed successfully */
	bool ForEachLoadedChunk(cFunctionRef<bool(int, int)> a_Callback);

	// tolua_begin

	/** Sets the block at the specified coords to the specified value.
	Full processing, incl. updating neighbors, is performed.
	*/
	void SetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_SendToClients = true);

	/** Sets the block at the specified coords to the specified value.
	The replacement doesn't trigger block updates.
	The replaced blocks aren't checked for block entities (block entity is leaked if it exists at this block)
	*/
	void FastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		m_ChunkMap->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta);
	}

	BLOCKTYPE  GetBlock          (int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		return m_ChunkMap->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	}

	NIBBLETYPE GetBlockMeta      (int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		return m_ChunkMap->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
	}

	void       SetBlockMeta      (int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_MetaData, bool a_ShouldMarkDirty = true, bool a_ShouldInformClients = true);
	NIBBLETYPE GetBlockSkyLight  (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetBlockBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ);

	// tolua_end

	bool GetBlockTypeMeta  (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);  // TODO: Exported in ManualBindings.cpp
	bool GetBlockInfo      (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight);  // TODO: Exported in ManualBindings.cpp
	// TODO: NIBBLETYPE GetBlockActualLight(int a_BlockX, int a_BlockY, int a_BlockZ);

	// tolua_begin

	// Vector3i variants:
	void       FastSetBlock(Vector3i a_Pos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) { FastSetBlock( a_Pos.x, a_Pos.y, a_Pos.z, a_BlockType, a_BlockMeta); }
	BLOCKTYPE  GetBlock    (Vector3i a_Pos) { return GetBlock( a_Pos.x, a_Pos.y, a_Pos.z); }
	NIBBLETYPE GetBlockMeta(Vector3i a_Pos) { return GetBlockMeta( a_Pos.x, a_Pos.y, a_Pos.z); }
	void       SetBlockMeta(Vector3i a_Pos, NIBBLETYPE a_MetaData) { SetBlockMeta( a_Pos.x, a_Pos.y, a_Pos.z, a_MetaData); }
	NIBBLETYPE GetBlockBlockLight(Vector3i a_Pos) { return GetBlockBlockLight( a_Pos.x, a_Pos.y, a_Pos.z); }
	// tolua_end

	/** Writes the block area into the specified coords.
	Returns true if all chunks have been processed.
	Prefer cBlockArea::Write() instead, this is the internal implementation; cBlockArea does error checking, too.
	a_DataTypes is a bitmask of cBlockArea::baXXX constants ORed together.
	Doesn't wake up simulators, use WakeUpSimulatorsInArea() for that. */
	virtual bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes) override;

	// tolua_begin

	/** Spawns item pickups for each item in the list. May compress pickups if too many entities: */
	virtual void SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_FlyAwaySpeed = 1.0, bool IsPlayerCreated = false) override;

	/** Spawns item pickups for each item in the list. May compress pickups if too many entities. All pickups get the speed specified. */
	virtual void SpawnItemPickups(const cItems & a_Pickups, double a_BlockX, double a_BlockY, double a_BlockZ, double a_SpeedX, double a_SpeedY, double a_SpeedZ, bool IsPlayerCreated = false) override;

	/** Spawns a single pickup containing the specified item. */
	virtual UInt32 SpawnItemPickup(double a_PosX, double a_PosY, double a_PosZ, const cItem & a_Item, float a_SpeedX = 0.f, float a_SpeedY = 0.f, float a_SpeedZ = 0.f, int a_LifetimeTicks = 6000, bool a_CanCombine = true) override;

	/** Spawns an falling block entity at the given position.
	Returns the UniqueID of the spawned falling block, or cEntity::INVALID_ID on failure. */
	UInt32 SpawnFallingBlock(int a_X, int a_Y, int a_Z, BLOCKTYPE BlockType, NIBBLETYPE BlockMeta);

	/** Spawns an minecart at the given coordinates.
	Returns the UniqueID of the spawned minecart, or cEntity::INVALID_ID on failure. */
	UInt32 SpawnMinecart(double a_X, double a_Y, double a_Z, int a_MinecartType, const cItem & a_Content = cItem(), int a_BlockHeight = 1);

	// DEPRECATED, use the vector-parametered version instead.
	UInt32 SpawnBoat(double a_X, double a_Y, double a_Z, cBoat::eMaterial a_Material)
	{
		LOGWARNING("cWorld::SpawnBoat(double, double, double) is deprecated, use cWorld::SpawnBoat(Vector3d) instead.");
		return SpawnBoat({a_X, a_Y, a_Z}, a_Material);
	}

	/** Spawns a boat at the given coordinates.
	Returns the UniqueID of the spawned boat, or cEntity::INVALID_ID on failure. */
	UInt32 SpawnBoat(Vector3d a_Pos, cBoat::eMaterial a_Material);

	/** Spawns an experience orb at the given location with the given reward.
	Returns the UniqueID of the spawned experience orb, or cEntity::INVALID_ID on failure. */
	virtual UInt32 SpawnExperienceOrb(double a_X, double a_Y, double a_Z, int a_Reward) override;

	// DEPRECATED, use the vector-parametered version instead.
	UInt32 SpawnPrimedTNT(double a_X, double a_Y, double a_Z, int a_FuseTimeInSec = 80, double a_InitialVelocityCoeff = 1)
	{
		LOGWARNING("cWorld::SpawnPrimedTNT(double, double, double) is deprecated, use cWorld::SpawnPrimedTNT(Vector3d) instead.");
		return SpawnPrimedTNT({a_X, a_Y, a_Z}, a_FuseTimeInSec, a_InitialVelocityCoeff);
	}

	/** Spawns a new primed TNT entity at the specified block coords and specified fuse duration.
	Initial velocity is given based on the relative coefficient provided.
	Returns the UniqueID of the created entity, or cEntity::INVALID_ID on failure. */
	UInt32 SpawnPrimedTNT(Vector3d a_Pos, int a_FuseTimeInSec = 80, double a_InitialVelocityCoeff = 1);

	// tolua_end

	/** Performs the specified single-block set operations simultaneously, as if SetBlock() was called for each item.
	Is more efficient than calling SetBlock() multiple times.
	If the chunk for any of the blocks is not loaded, the set operation is ignored silently. */
	void SetBlocks(const sSetBlockVector & a_Blocks);

	/** Replaces world blocks with a_Blocks, if they are of type a_FilterBlockType */
	void ReplaceBlocks(const sSetBlockVector & a_Blocks, BLOCKTYPE a_FilterBlockType);

	/** Retrieves block types of the specified blocks. If a chunk is not loaded, doesn't modify the block. Returns true if all blocks were read. */
	bool GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure);

	// tolua_begin
	bool DigBlock   (int a_X, int a_Y, int a_Z);
	virtual void SendBlockTo(int a_X, int a_Y, int a_Z, cPlayer & a_Player) override;

	/** Set default spawn at the given coordinates.
	Returns false if the new spawn couldn't be stored in the INI file. */
	bool SetSpawn(double a_X, double a_Y, double a_Z);

	double GetSpawnX(void) const { return m_SpawnX; }
	double GetSpawnY(void) const { return m_SpawnY; }
	double GetSpawnZ(void) const { return m_SpawnZ; }

	/** Wakes up the simulators for the specified block */
	virtual void WakeUpSimulators(Vector3i a_Block) override;

	/** Wakes up the simulators for the specified area of blocks */
	void WakeUpSimulatorsInArea(const cCuboid & a_Area);

	// DEPRECATED, use vector-parametered version instead
	void WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		LOGWARNING("cWorld::WakeUpSimulators(int, int, int) is deprecated, use cWorld::WakeUpSimulators(Vector3i) instead.");
		WakeUpSimulators({a_BlockX, a_BlockY, a_BlockZ});
	}

	// DEPRECATED, use vector-parametered version instead
	void WakeUpSimulatorsInArea(int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ);

	// tolua_end

	inline cSimulatorManager * GetSimulatorManager(void) { return m_SimulatorManager.get(); }

	inline cFluidSimulator * GetWaterSimulator(void) { return m_WaterSimulator; }
	inline cFluidSimulator * GetLavaSimulator (void) { return m_LavaSimulator; }
	inline cRedstoneSimulator * GetRedstoneSimulator(void) { return m_RedstoneSimulator; }

	/** Calls the callback for each block entity in the specified chunk; returns true if all block entities processed, false if the callback aborted by returning true */
	bool ForEachBlockEntityInChunk(int a_ChunkX, int a_ChunkZ, cBlockEntityCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for each brewingstand in the specified chunk; returns true if all brewingstands processed, false if the callback aborted by returning true */
	bool ForEachBrewingstandInChunk(int a_ChunkX, int a_ChunkZ, cBrewingstandCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for each chest in the specified chunk; returns true if all chests processed, false if the callback aborted by returning true */
	bool ForEachChestInChunk(int a_ChunkX, int a_ChunkZ, cChestCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for each dispenser in the specified chunk; returns true if all dispensers processed, false if the callback aborted by returning true */
	bool ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback a_Callback);

	/** Calls the callback for each dropper in the specified chunk; returns true if all droppers processed, false if the callback aborted by returning true */
	bool ForEachDropperInChunk(int a_ChunkX, int a_ChunkZ, cDropperCallback a_Callback);

	/** Calls the callback for each dropspenser in the specified chunk; returns true if all dropspensers processed, false if the callback aborted by returning true */
	bool ForEachDropSpenserInChunk(int a_ChunkX, int a_ChunkZ, cDropSpenserCallback a_Callback);

	/** Calls the callback for each furnace in the specified chunk; returns true if all furnaces processed, false if the callback aborted by returning true */
	bool ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Does an explosion with the specified strength at the specified coordinates.
	Executes the HOOK_EXPLODING and HOOK_EXPLODED hooks as part of the processing.
	a_SourceData exact type depends on the a_Source, see the declaration of the esXXX constants in BlockID.h for details.
	Exported to Lua manually in ManualBindings_World.cpp in order to support the variable a_SourceData param. */
	virtual void DoExplosionAt(double a_ExplosionSize, double a_BlockX, double a_BlockY, double a_BlockZ, bool a_CanCauseFire, eExplosionSource a_Source, void * a_SourceData) override;

	/** Calls the callback for the block entity at the specified coords; returns false if there's no block entity at those coords, true if found */
	virtual bool DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback a_Callback) override;  // Exported in ManualBindings.cpp

	/** Calls the callback for the beacon at the specified coords; returns false if there's no beacon at those coords, true if found */
	bool DoWithBeaconAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the bed at the specified coords; returns false if there's no bed at those coords, true if found */
	virtual bool DoWithBedAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBedCallback a_Callback) override;  // Exported in ManualBindings.cpp

	/** Calls the callback for the brewingstand at the specified coords; returns false if there's no brewingstand at those coords, true if found */
	bool DoWithBrewingstandAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBrewingstandCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the chest at the specified coords; returns false if there's no chest at those coords, true if found */
	bool DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the dispenser at the specified coords; returns false if there's no dispenser at those coords or callback returns true, returns true if found */
	bool DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the dropper at the specified coords; returns false if there's no dropper at those coords or callback returns true, returns true if found */
	bool DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the dropspenser at the specified coords; returns false if there's no dropspenser at those coords or callback returns true, returns true if found */
	bool DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the furnace at the specified coords; returns false if there's no furnace at those coords or callback returns true, returns true if found */
	bool DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the noteblock at the specified coords; returns false if there's no noteblock at those coords or callback returns true, returns true if found */
	bool DoWithNoteBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cNoteBlockCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the command block at the specified coords; returns false if there's no command block at those coords or callback returns true, returns true if found */
	bool DoWithCommandBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cCommandBlockCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the mob head block at the specified coords; returns false if there's no mob head block at those coords or callback returns true, returns true if found */
	bool DoWithMobHeadAt(int a_BlockX, int a_BlockY, int a_BlockZ, cMobHeadCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Calls the callback for the flower pot at the specified coords; returns false if there's no flower pot at those coords or callback returns true, returns true if found */
	bool DoWithFlowerPotAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFlowerPotCallback a_Callback);  // Exported in ManualBindings.cpp

	/** Retrieves the test on the sign at the specified coords; returns false if there's no sign at those coords, true if found */
	bool GetSignLines (int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4);  // Exported in ManualBindings.cpp

	/** a_Player is using block entity at [x, y, z], handle that: */
	void UseBlockEntity(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) {m_ChunkMap->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ); }  // tolua_export

	/** Calls the callback for the chunk specified, with ChunkMapCS locked.
	Returns false if the chunk doesn't exist, otherwise returns the same value as the callback */
	bool DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback a_Callback);

	/** Calls the callback for the chunk at the block position specified, with ChunkMapCS locked.
	Returns false if the chunk isn't loaded, otherwise returns the same value as the callback */
	bool DoWithChunkAt(Vector3i a_BlockPos, cChunkCallback a_Callback);

	void GrowTreeImage(const sSetBlockVector & a_Blocks);

	// tolua_begin

	/** Grows a tree at the specified coords, either from a sapling there, or based on the biome */
	void GrowTree           (int a_BlockX, int a_BlockY, int a_BlockZ);

	/** Grows a tree at the specified coords, based on the sapling meta provided */
	void GrowTreeFromSapling(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_SaplingMeta);

	/** Grows a tree at the specified coords, based on the biome in the place */
	void GrowTreeByBiome    (int a_BlockX, int a_BlockY, int a_BlockZ);

	/** Grows the plant at the specified block to its ripe stage (bonemeal used); returns false if the block is not growable. If a_IsBonemeal is true, block is not grown if not allowed in world.ini */
	bool GrowRipePlant(int a_BlockX, int a_BlockY, int a_BlockZ, bool a_IsByBonemeal = false);

	/** Grows a cactus present at the block specified by the amount of blocks specified, up to the max height specified in the config; returns the amount of blocks the cactus grew inside this call */
	int GrowCactus(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow);

	/** Grows a melon or a pumpkin next to the block specified (assumed to be the stem); returns true if the pumpkin or melon sucessfully grew. */
	bool GrowMelonPumpkin(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType);

	/** Grows a sugarcane present at the block specified by the amount of blocks specified, up to the max height specified in the config; returns the amount of blocks the sugarcane grew inside this call */
	int GrowSugarcane(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow);

	/** Returns the biome at the specified coords. Reads the biome from the chunk, if loaded, otherwise uses the world generator to provide the biome value */
	EMCSBiome GetBiomeAt(int a_BlockX, int a_BlockZ);

	/** Sets the biome at the specified coords. Returns true if successful, false if not (chunk not loaded).
	Doesn't resend the chunk to clients, use ForceSendChunkTo() for that. */
	bool SetBiomeAt(int a_BlockX, int a_BlockZ, EMCSBiome a_Biome);

	/** Sets the biome at the area. Returns true if successful, false if any subarea failed (chunk not loaded).
	(Re)sends the chunks to their relevant clients if successful. */
	bool SetAreaBiome(int a_MinX, int a_MaxX, int a_MinZ, int a_MaxZ, EMCSBiome a_Biome);

	/** Sets the biome at the area. Returns true if successful, false if any subarea failed (chunk not loaded).
	(Re)sends the chunks to their relevant clients if successful.
	The cuboid needn't be sorted. */
	bool SetAreaBiome(const cCuboid & a_Area, EMCSBiome a_Biome);

	/** Returns the name of the world */
	const AString & GetName(void) const { return m_WorldName; }

	/** Returns the data path to the world data */
	const AString & GetDataPath(void) const { return m_DataPath; }

	/** Returns the name of the world.ini file used by this world */
	const AString & GetIniFileName(void) const {return m_IniFileName; }

	/** Returns the associated scoreboard instance. */
	cScoreboard & GetScoreBoard(void) { return m_Scoreboard; }

	/** Returns the associated map manager instance. */
	cMapManager & GetMapManager(void) { return m_MapManager; }

	bool AreCommandBlocksEnabled(void) const { return m_bCommandBlocksEnabled; }
	void SetCommandBlocksEnabled(bool a_Flag) { m_bCommandBlocksEnabled = a_Flag; }

	eShrapnelLevel GetTNTShrapnelLevel(void) const { return m_TNTShrapnelLevel; }
	void SetTNTShrapnelLevel(eShrapnelLevel a_Flag) { m_TNTShrapnelLevel = a_Flag; }

	int GetMaxViewDistance(void) const { return m_MaxViewDistance; }
	void SetMaxViewDistance(int a_MaxViewDistance);

	bool ShouldUseChatPrefixes(void) const { return m_bUseChatPrefixes; }
	void SetShouldUseChatPrefixes(bool a_Flag) { m_bUseChatPrefixes = a_Flag; }

	bool ShouldBroadcastDeathMessages(void) const { return m_BroadcastDeathMessages; }
	bool ShouldBroadcastAchievementMessages(void) const { return m_BroadcastAchievementMessages; }


	AString GetLinkedNetherWorldName(void) const { return m_LinkedNetherWorldName; }
	void SetLinkedNetherWorldName(const AString & a_Name) { m_LinkedNetherWorldName = a_Name; }

	AString GetLinkedEndWorldName(void) const { return m_LinkedEndWorldName; }
	void SetLinkedEndWorldName(const AString & a_Name) { m_LinkedEndWorldName = a_Name; }

	AString GetLinkedOverworldName(void) const { return m_LinkedOverworldName; }
	void SetLinkedOverworldName(const AString & a_Name) { m_LinkedOverworldName = a_Name; }

	/** Returns or sets the minumim or maximum netherportal width */
	virtual int GetMinNetherPortalWidth(void) const override { return m_MinNetherPortalWidth; }
	virtual int GetMaxNetherPortalWidth(void) const override { return m_MaxNetherPortalWidth; }
	virtual void SetMinNetherPortalWidth(int a_NewMinWidth) override { m_MinNetherPortalWidth = a_NewMinWidth; }
	virtual void SetMaxNetherPortalWidth(int a_NewMaxWidth) override { m_MaxNetherPortalWidth = a_NewMaxWidth; }

	/** Returns or sets the minumim or maximum netherportal height */
	virtual int GetMinNetherPortalHeight(void) const override { return m_MinNetherPortalHeight; }
	virtual int GetMaxNetherPortalHeight(void) const override { return m_MaxNetherPortalHeight; }
	virtual void SetMinNetherPortalHeight(int a_NewMinHeight) override { m_MinNetherPortalHeight = a_NewMinHeight; }
	virtual void SetMaxNetherPortalHeight(int a_NewMaxHeight) override { m_MaxNetherPortalHeight = a_NewMaxHeight; }

	// tolua_end

	/** Saves all chunks immediately. Dangerous interface, may deadlock, use QueueSaveAllChunks() instead */
	void SaveAllChunks(void);

	/** Queues a task to save all chunks onto the tick thread. The prefferred way of saving chunks from external sources */
	void QueueSaveAllChunks(void);  // tolua_export

	/** Queues a task onto the tick thread. The task object will be deleted once the task is finished */
	void QueueTask(std::function<void(cWorld &)> a_Task);  // Exported in ManualBindings.cpp

	/** Queues a lambda task onto the tick thread, with the specified delay. */
	void ScheduleTask(int a_DelayTicks, std::function<void(cWorld &)> a_Task);

	/** Returns the number of chunks loaded	 */
	size_t GetNumChunks() const;  // tolua_export

	/** Returns the number of unused dirty chunks. That's the number of chunks that we can save and then unload. */
	size_t GetNumUnusedDirtyChunks(void) const;  // tolua_export

	/** Returns the number of chunks loaded and dirty, and in the lighting queue */
	void GetChunkStats(int & a_NumValid, int & a_NumDirty, int & a_NumInLightingQueue);

	// Various queues length queries (cannot be const, they lock their CS):
	inline int GetGeneratorQueueLength     (void) { return m_Generator.GetQueueLength();   }    // tolua_export
	inline size_t GetLightingQueueLength   (void) { return m_Lighting.GetQueueLength();    }    // tolua_export
	inline size_t GetStorageLoadQueueLength(void) { return m_Storage.GetLoadQueueLength(); }    // tolua_export
	inline size_t GetStorageSaveQueueLength(void) { return m_Storage.GetSaveQueueLength(); }    // tolua_export

	cLightingThread & GetLightingThread(void) { return m_Lighting; }

	void InitializeSpawn(void);

	/** Starts threads that belong to this world. */
	void Start();

	/** Stops threads that belong to this world (part of deinit).
	a_DeadlockDetect is used for tracking this world's age, detecting a possible deadlock. */
	void Stop(cDeadlockDetect & a_DeadlockDetect);

	/** Processes the blocks queued for ticking with a delay (m_BlockTickQueue[]) */
	void TickQueuedBlocks(void);

	struct BlockTickQueueItem
	{
		int X;
		int Y;
		int Z;
		int TicksToWait;
	};

	/** Queues the block to be ticked after the specified number of game ticks */
	void QueueBlockForTick(int a_BlockX, int a_BlockY, int a_BlockZ, int a_TicksToWait);  // tolua_export

	// tolua_begin
	/** Casts a thunderbolt at the specified coords */
	void CastThunderbolt(Vector3i a_Block);
	void CastThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ);  // DEPRECATED, use vector-parametered version instead

	/** Sets the specified weather; resets weather interval; asks and notifies plugins of the change */
	void SetWeather(eWeather a_NewWeather);

	/** Forces a weather change in the next game tick */
	void ChangeWeather(void);

	/** Returns the current weather. Instead of comparing values directly to the weather constants, use IsWeatherXXX() functions, if possible */
	eWeather GetWeather(void) const { return m_Weather; }

	/** Returns true if the current weather is sunny. */
	bool IsWeatherSunny(void) const { return (m_Weather == wSunny); }

	/** Returns true if it is sunny at the specified location. This takes into account biomes. */
	bool IsWeatherSunnyAt(int a_BlockX, int a_BlockZ)
	{
		return (IsWeatherSunny() || IsBiomeNoDownfall(GetBiomeAt(a_BlockX, a_BlockZ)));
	}

	/** Returns true if the current weather is rainy. */
	bool IsWeatherRain(void) const { return (m_Weather == wRain); }

	/** Returns true if it is raining at the specified location. This takes into account biomes. */
	bool IsWeatherRainAt(int a_BlockX, int a_BlockZ)
	{
		return (IsWeatherRain() && !IsBiomeNoDownfall(GetBiomeAt(a_BlockX, a_BlockZ)));
	}

	/** Returns true if the current weather is stormy. */
	bool IsWeatherStorm(void) const { return (m_Weather == wStorm); }

	/** Returns true if the weather is stormy at the specified location. This takes into account biomes. */
	bool IsWeatherStormAt(int a_BlockX, int a_BlockZ)
	{
		return (IsWeatherStorm() && !IsBiomeNoDownfall(GetBiomeAt(a_BlockX, a_BlockZ)));
	}

	/** Returns true if the world currently has any precipitation - rain, storm or snow. */
	bool IsWeatherWet(void) const { return !IsWeatherSunny(); }

	/** Returns true if it is raining or storming at the specified location.
	This takes into account biomes. */
	virtual bool IsWeatherWetAt(int a_BlockX, int a_BlockZ) override
	{
		auto Biome = GetBiomeAt(a_BlockX, a_BlockZ);
		return (IsWeatherWet() && !IsBiomeNoDownfall(Biome) && !IsBiomeCold(Biome));
	}

	/** Returns true if the specified location has wet weather (rain or storm),
	using the same logic as IsWeatherWetAt, except that any rain-blocking blocks
	above the specified position will block the precipitation and this function
	will return false. */
	virtual bool IsWeatherWetAtXYZ(Vector3i a_Pos);

	/** Returns the seed of the world. */
	int GetSeed(void) { return m_Generator.GetSeed(); }

	// tolua_end

	cChunkGenerator & GetGenerator(void) { return m_Generator; }
	cWorldStorage &   GetStorage  (void) { return m_Storage; }
	cChunkMap *       GetChunkMap (void) { return m_ChunkMap.get(); }

	/** Sets the blockticking to start at the specified block. Only one blocktick per chunk may be set, second call overwrites the first call */
	void SetNextBlockTick(int a_BlockX, int a_BlockY, int a_BlockZ);  // tolua_export

	int GetMaxSugarcaneHeight(void) const { return m_MaxSugarcaneHeight; }  // tolua_export
	int GetMaxCactusHeight   (void) const { return m_MaxCactusHeight; }     // tolua_export

	bool IsBlockDirectlyWatered(int a_BlockX, int a_BlockY, int a_BlockZ);  // tolua_export

	/** Spawns a mob of the specified type. Returns the mob's UniqueID if recognized and spawned, cEntity::INVALID_ID otherwise */
	virtual UInt32 SpawnMob(double a_PosX, double a_PosY, double a_PosZ, eMonsterType a_MonsterType, bool a_Baby = false) override;  // tolua_export

	/** Wraps cEntity::Initialize, doing Monster-specific things before spawning the monster.
	Takes ownership of the given Monster reference. */
	UInt32 SpawnMobFinalize(std::unique_ptr<cMonster> a_Monster);

	/** Creates a projectile of the specified type. Returns the projectile's UniqueID if successful, cEntity::INVALID_ID otherwise
	Item parameter is currently used for Fireworks to correctly set entity metadata based on item metadata. */
	UInt32 CreateProjectile(double a_PosX, double a_PosY, double a_PosZ, cProjectileEntity::eKind a_Kind, cEntity * a_Creator, const cItem * a_Item, const Vector3d * a_Speed = nullptr);  // tolua_export

	/** Returns a random number in range [0 .. a_Range]. */
	int GetTickRandomNumber(int a_Range);

	/** Appends all usernames starting with a_Text (case-insensitive) into Results */
	void TabCompleteUserName(const AString & a_Text, AStringVector & a_Results);

	/** Get the current darkness level based on the time */
	NIBBLETYPE GetSkyDarkness() { return m_SkyDarkness; }

	/** Increments (a_AlwaysTicked == true) or decrements (false) the m_AlwaysTicked counter for the specified chunk.
	If the m_AlwaysTicked counter is greater than zero, the chunk is ticked in the tick-thread regardless of
	whether it has any clients or not.
	This function allows nesting and task-concurrency (multiple separate tasks can request ticking and as long
	as at least one requests is active the chunk will be ticked). */
	void SetChunkAlwaysTicked(int a_ChunkX, int a_ChunkZ, bool a_AlwaysTicked = true);  // tolua_export

	cBroadcaster GetBroadcaster();

private:

	friend class cRoot;

	class cTickThread :
		public cIsThread
	{
		typedef cIsThread super;
	public:
		cTickThread(cWorld & a_World);

	protected:
		cWorld & m_World;

		// cIsThread overrides:
		virtual void Execute(void) override;
	} ;

	/** Implementation of the callbacks that the ChunkGenerator uses to store new chunks and interface to plugins */
	class cChunkGeneratorCallbacks :
		public cChunkGenerator::cChunkSink,
		public cChunkGenerator::cPluginInterface
	{
		cWorld * m_World;

		// cChunkSink overrides:
		virtual void OnChunkGenerated  (cChunkDesc & a_ChunkDesc) override;
		virtual bool IsChunkValid      (int a_ChunkX, int a_ChunkZ) override;
		virtual bool HasChunkAnyClients(int a_ChunkX, int a_ChunkZ) override;
		virtual bool IsChunkQueued     (int a_ChunkX, int a_ChunkZ) override;

		// cPluginInterface overrides:
		virtual void CallHookChunkGenerating(cChunkDesc & a_ChunkDesc) override;
		virtual void CallHookChunkGenerated (cChunkDesc & a_ChunkDesc) override;

	public:
		cChunkGeneratorCallbacks(cWorld & a_World);
	} ;


	/** The maximum number of allowed unused dirty chunks for this world.
	Loaded from config, enforced every 10 seconds by freeing some unused dirty chunks
	if this was exceeded. */
	size_t m_UnusedDirtyChunksCap;

	AString m_WorldName;

	/** The path to the root directory for the world files. Does not including trailing path specifier. */
	AString m_DataPath;

	/** The name of the overworld that portals in this world should link to.
	Only has effect if this world is a Nether or End world. */
	AString m_LinkedOverworldName;

	AString m_IniFileName;

	/** Name of the storage schema used to load and save chunks */
	AString m_StorageSchema;

	int m_StorageCompressionFactor;

	/** Whether or not writing chunks to disk is currently enabled */
	std::atomic<bool> m_IsSavingEnabled;

	/** The dimension of the world, used by the client to provide correct lighting scheme */
	eDimension m_Dimension;

	bool m_IsSpawnExplicitlySet;
	double m_SpawnX;
	double m_SpawnY;
	double m_SpawnZ;

	// Variables defining the minimum and maximum size for a nether portal
	int m_MinNetherPortalWidth;
	int m_MaxNetherPortalWidth;
	int m_MinNetherPortalHeight;
	int m_MaxNetherPortalHeight;

	bool m_BroadcastDeathMessages;
	bool m_BroadcastAchievementMessages;

	bool   m_IsDaylightCycleEnabled;

	/** The age of the world.
	Monotonic, always increasing each game tick, persistent across server restart. */
	std::chrono::milliseconds m_WorldAge;

	std::chrono::milliseconds  m_TimeOfDay;
	cTickTimeLong  m_LastTimeUpdate;    // The tick in which the last time update has been sent.
	cTickTimeLong  m_LastChunkCheck;        // The last WorldAge (in ticks) in which unloading and possibly saving was triggered
	cTickTimeLong  m_LastSave;          // The last WorldAge (in ticks) in which save-all was triggerred
	std::map<cMonster::eFamily, cTickTimeLong> m_LastSpawnMonster;  // The last WorldAge (in ticks) in which a monster was spawned (for each megatype of monster)  // MG TODO : find a way to optimize without creating unmaintenability (if mob IDs are becoming unrowed)

	NIBBLETYPE m_SkyDarkness;

	eGameMode m_GameMode;
	bool m_bEnabledPVP;
	bool m_IsDeepSnowEnabled;
	bool m_ShouldLavaSpawnFire;
	bool m_VillagersShouldHarvestCrops;

	std::vector<BlockTickQueueItem *> m_BlockTickQueue;
	std::vector<BlockTickQueueItem *> m_BlockTickQueueCopy;  // Second is for safely removing the objects from the queue

	std::unique_ptr<cSimulatorManager>   m_SimulatorManager;
	std::unique_ptr<cSandSimulator>      m_SandSimulator;
	cFluidSimulator *                    m_WaterSimulator;
	cFluidSimulator *                    m_LavaSimulator;
	std::unique_ptr<cFireSimulator>      m_FireSimulator;
	cRedstoneSimulator * m_RedstoneSimulator;

	cCriticalSection m_CSPlayers;
	cPlayerList      m_Players;

	cWorldStorage    m_Storage;

	unsigned int m_MaxPlayers;

	std::unique_ptr<cChunkMap> m_ChunkMap;

	bool m_bAnimals;
	std::set<eMonsterType> m_AllowedMobs;

	eWeather m_Weather;
	int m_WeatherInterval;
	int m_MaxSunnyTicks, m_MinSunnyTicks;
	int m_MaxRainTicks,  m_MinRainTicks;
	int m_MaxThunderStormTicks, m_MinThunderStormTicks;

	int  m_MaxCactusHeight;
	int  m_MaxSugarcaneHeight;
	bool m_IsBeetrootsBonemealable;
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
	bool m_IsBigFlowerBonemealable;
	bool m_IsTallGrassBonemealable;

	/** Whether command blocks are enabled or not */
	bool m_bCommandBlocksEnabled;

	/** Whether prefixes such as [INFO] are prepended to SendMessageXXX() / BroadcastChatXXX() functions */
	bool m_bUseChatPrefixes;

	/** The level of DoExplosionAt() projecting random affected blocks as FallingBlock entities
	See the eShrapnelLevel enumeration for details
	*/
	eShrapnelLevel m_TNTShrapnelLevel;

	/** The maximum view distance that a player can have in this world. */
	int m_MaxViewDistance;

	/** Name of the nether world - where Nether portals should teleport.
	Only used when this world is an Overworld. */
	AString m_LinkedNetherWorldName;

	/** Name of the End world - where End portals should teleport.
	Only used when this world is an Overworld. */
	AString m_LinkedEndWorldName;


	cChunkGenerator  m_Generator;

	cScoreboard      m_Scoreboard;
	cMapManager      m_MapManager;

	/** The callbacks that the ChunkGenerator uses to store new chunks and interface to plugins */
	cChunkGeneratorCallbacks m_GeneratorCallbacks;

	cChunkSender     m_ChunkSender;
	cLightingThread  m_Lighting;
	cTickThread      m_TickThread;

	/** Guards the m_Tasks */
	cCriticalSection m_CSTasks;

	/** Tasks that have been queued onto the tick thread, possibly to be executed at target tick in the future; guarded by m_CSTasks */
	std::vector<std::pair<Int64, std::function<void(cWorld &)>>> m_Tasks;

	/** Guards m_Clients */
	cCriticalSection  m_CSClients;

	/** List of clients in this world, these will be ticked by this world */
	cClientHandlePtrs m_Clients;

	/** Clients that are scheduled for removal (ticked in another world), waiting for TickClients() to remove them */
	cClientHandles m_ClientsToRemove;

	/** Clients that are scheduled for adding, waiting for TickClients to add them */
	cClientHandlePtrs m_ClientsToAdd;

	/** Guards m_EntitiesToAdd */
	cCriticalSection m_CSEntitiesToAdd;

	/** List of entities that are scheduled for adding, waiting for the Tick thread to add them. */
	cEntityList m_EntitiesToAdd;

	/** Guards m_PlayersToAdd */
	cCriticalSection m_CSPlayersToAdd;

	/** List of players that are scheduled for adding, waiting for the Tick thread to add them. */
	cAwaitingPlayerList m_PlayersToAdd;

	/** CS protecting m_SetChunkDataQueue. */
	cCriticalSection m_CSSetChunkDataQueue;

	/** Queue for the chunk data to be set into m_ChunkMap by the tick thread. Protected by m_CSSetChunkDataQueue */
	cSetChunkDataPtrs m_SetChunkDataQueue;

	/** Construct the world and read settings from its ini file.
	@param a_DeadlockDetect is used for tracking this world's age, detecting a possible deadlock.
	@param a_WorldNames is a list of all world names, used to validate linked worlds
	*/
	cWorld(
		const AString & a_WorldName, const AString & a_DataPath,
		cDeadlockDetect & a_DeadlockDetect, const AStringVector & a_WorldNames,
		eDimension a_Dimension = dimOverworld, const AString & a_LinkedOverworldName = {}
	);
	virtual ~cWorld() override;

	void Tick(std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec);

	/** Handles the weather in each tick */
	void TickWeather(float a_Dt);

	/** Handles the mob spawning / moving / destroying each tick */
	void TickMobs(std::chrono::milliseconds a_Dt);

	/** Executes all tasks queued onto the tick thread */
	void TickQueuedTasks(void);

	/** Ticks all clients that are in this world */
	void TickClients(float a_Dt);

	/** Unloads all chunks immediately. */
	void UnloadUnusedChunks(void);

	void UpdateSkyDarkness(void);

	/** Generates a random spawnpoint on solid land by walking chunks and finding their biomes */
	void GenerateRandomSpawn(int a_MaxSpawnRadius);

	/** Can the specified coordinates be used as a spawn point?
	Returns true if spawn position is valid and sets a_Y to the valid spawn height */
	bool CanSpawnAt(double a_X, double & a_Y, double a_Z);

	/** Check if player starting point is acceptable */
	bool CheckPlayerSpawnPoint(int a_PosX, int a_PosY, int a_PosZ);

	/** Chooses a reasonable transition from the current weather to a new weather */
	eWeather ChooseNewWeather(void);

	/** Creates a new fluid simulator, loads its settings from the inifile (a_FluidName section) */
	cFluidSimulator * InitializeFluidSimulator(cIniFile & a_IniFile, const char * a_FluidName, BLOCKTYPE a_SimulateBlock, BLOCKTYPE a_StationaryBlock);

	/** Creates a new redstone simulator. */
	cRedstoneSimulator * InitializeRedstoneSimulator(cIniFile & a_IniFile);

	/** Adds the players queued in the m_PlayersToAdd queue into the m_Players list.
	Assumes it is called from the Tick thread. */
	void AddQueuedPlayers(void);

	/** Sets generator values to dimension specific defaults, if those values do not exist */
	void InitialiseGeneratorDefaults(cIniFile & a_IniFile);

	/** Sets mob spawning values if nonexistant to their dimension specific defaults */
	void InitialiseAndLoadMobSpawningValues(cIniFile & a_IniFile);

	/** Sets the specified chunk data into the chunkmap. Called in the tick thread.
	Modifies the a_SetChunkData - moves the entities contained in it into the chunk. */
	void SetChunkData(cSetChunkData & a_SetChunkData);

};  // tolua_export
