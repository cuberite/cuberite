
#pragma once

#include "Entities/Entity.h"
#include "ChunkData.h"

#include "Simulator/FireSimulator.h"
#include "Simulator/SandSimulator.h"
#include "Simulator/RedstoneSimulator.h"

#include "ChunkMap.h"





class cWorld;
class cClientHandle;
class cPlayer;
class cChunkMap;
class cBeaconEntity;
class cBedEntity;
class cBrewingstandEntity;
class cBoundingBox;
class cChestEntity;
class cChunkDataCallback;
class cCommandBlockEntity;
class cDispenserEntity;
class cFurnaceEntity;
class cNoteEntity;
class cMobHeadEntity;
class cFlowerPotEntity;
class cBlockArea;
class cBlockArea;
class cFluidSimulatorData;
class cMobCensus;
class cMobSpawner;
class cSetChunkData;

typedef std::list<cClientHandle *>                cClientHandleList;

// A convenience macro for calling GetChunkAndRelByAbsolute.
#define PREPARE_REL_AND_CHUNK(Position, OriginalChunk) cChunk * Chunk; Vector3i Rel; bool RelSuccess = (OriginalChunk).GetChunkAndRelByAbsolute(Position, &Chunk, Rel);
#define PREPARE_BLOCKDATA BLOCKTYPE BlockType; NIBBLETYPE BlockMeta;


// This class is not to be used directly
// Instead, call actions on cChunkMap (such as cChunkMap::SetBlock() etc.)
class cChunk :
	public cChunkDef  // The inheritance is "misused" here only to inherit the functions and constants defined in cChunkDef
{
public:

	/** Represents the presence state of the chunk */
	enum ePresence
	{
		cpInvalid,  /**< The chunk is not present at all and is not queued in the loader / generator */
		cpQueued,   /**< The chunk is not present, but is queued for loading / generation */
		cpPresent,  /**< The chunk is present */
	};

	cChunk(
		int a_ChunkX, int a_ChunkZ,   // Chunk coords
		cChunkMap * a_ChunkMap, cWorld * a_World,   // Parent objects
		cChunk * a_NeighborXM, cChunk * a_NeighborXP, cChunk * a_NeighborZM, cChunk * a_NeighborZP,  // Neighbor chunks
		cAllocationPool<cChunkData::sChunkSection> & a_Pool
	);
	cChunk(cChunk & other) = delete;
	~cChunk();

	/** Returns true iff the chunk block data is valid (loaded / generated) */
	bool IsValid(void) const {return (m_Presence == cpPresent); }

	/** Returns true iff the chunk is in the queue for loading / generating */
	bool IsQueued(void) const {return (m_Presence == cpQueued); }

	/** Sets the chunk's presence.
	Wakes up any calls to cChunkMap::GetHeight() when setting to cpPresent. */
	void SetPresence(ePresence a_Presence);

	/** Called to indicate whether the chunk should be queued in the generator if it fails to load. Set by cChunkMap::GetChunk(). */
	void SetShouldGenerateIfLoadFailed(bool a_ShouldGenerateIfLoadFailed);

	/** Marks all clients attached to this chunk as wanting this chunk. Also sets presence to cpQueued. */
	void MarkRegenerating(void);

	/** Returns true iff the chunk has changed since it was last saved. */
	bool IsDirty(void) const {return m_IsDirty; }

	bool CanUnload(void);

	/** Returns true if the chunk could have been unloaded if it weren't dirty */
	bool CanUnloadAfterSaving(void);

	bool IsLightValid(void) const {return m_IsLightValid; }

	/*
	To save a chunk, the WSSchema must:
	1. Mark the chunk as being saved (MarkSaving())
	2. Get the chunk's data using GetAllData()
	3. Mark the chunk as saved (MarkSaved())
	If anywhere inside this sequence another thread mmodifies the chunk, the chunk will not get marked as saved in MarkSaved()
	*/
	void MarkSaving(void);  // Marks the chunk as being saved.
	void MarkSaved(void);  // Marks the chunk as saved, if it didn't change from the last call to MarkSaving()
	void MarkLoaded(void);  // Marks the chunk as freshly loaded. Fails if the chunk is already valid

	/** Marks the chunk as failed to load.
	If m_ShouldGenerateIfLoadFailed is set, queues the chunk for generating. */
	void MarkLoadFailed(void);

	/** Gets all chunk data, calls the a_Callback's methods for each data type */
	void GetAllData(cChunkDataCallback & a_Callback);

	/** Sets all chunk data as either loaded from the storage or generated.
	BlockLight and BlockSkyLight are optional, if not present, chunk will be marked as unlighted.
	Modifies the BlockEntity list in a_SetChunkData - moves the block entities into the chunk. */
	void SetAllData(cSetChunkData & a_SetChunkData);

	void SetLight(
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_SkyLight
	);

	/** Copies m_BlockData into a_BlockTypes, only the block types */
	void GetBlockTypes(BLOCKTYPE  * a_BlockTypes);

	/** Writes the specified cBlockArea at the coords specified. Note that the coords may extend beyond the chunk! */
	void WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes);

	/** Returns true if there is a block entity at the coords specified */
	bool HasBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ);

	/** Sets or resets the internal flag that prevents chunk from being unloaded.
	The flag is cumulative - it can be set multiple times and then needs to be un-set that many times
	before the chunk is unloadable again. */
	void Stay(bool a_Stay = true);

	/** Recence all mobs proximities to players in order to know what to do with them */
	void CollectMobCensus(cMobCensus & toFill);

	/** Try to Spawn Monsters inside chunk */
	void SpawnMobs(cMobSpawner & a_MobSpawner);

	void Tick(std::chrono::milliseconds a_Dt);

	/** Ticks a single block. Used by cWorld::TickQueuedBlocks() to tick the queued blocks */
	void TickBlock(int a_RelX, int a_RelY, int a_RelZ);

	int GetPosX(void) const { return m_PosX; }
	int GetPosZ(void) const { return m_PosZ; }

	cWorld * GetWorld(void) const { return m_World; }

	void SetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_SendToClients = true);
	// SetBlock() does a lot of work (heightmap, tickblocks, blockentities) so a BlockIdx version doesn't make sense
	void SetBlock(Vector3i a_RelBlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) { SetBlock( a_RelBlockPos.x, a_RelBlockPos.y, a_RelBlockPos.z, a_BlockType, a_BlockMeta); }

	/** Queues block for ticking (m_ToTickQueue) */
	void QueueTickBlock(int a_RelX, int a_RelY, int a_RelZ);

	/** Queues all 6 neighbors of the specified block for ticking (m_ToTickQueue). If any are outside the chunk, relays the checking to the proper neighboring chunk */
	void QueueTickBlockNeighbors(int a_RelX, int a_RelY, int a_RelZ);

	void FastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta, bool a_SendToClients = true);  // Doesn't force block updates on neighbors, use for simple changes such as grass growing etc.
	void FastSetBlock(Vector3i a_RelPos, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta, bool a_SendToClients = true)
	{
		FastSetBlock(a_RelPos.x, a_RelPos.y, a_RelPos.z, a_BlockType, a_BlockMeta, a_SendToClients);
	}

	BLOCKTYPE GetBlock(int a_RelX, int a_RelY, int a_RelZ) const { return m_ChunkData.GetBlock({ a_RelX, a_RelY, a_RelZ }); }
	BLOCKTYPE GetBlock(Vector3i a_RelCoords) const { return m_ChunkData.GetBlock(a_RelCoords); }

	void GetBlockTypeMeta(Vector3i a_RelPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const;
	void GetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const
	{
		GetBlockTypeMeta({ a_RelX, a_RelY, a_RelZ }, a_BlockType, a_BlockMeta);
	}

	void GetBlockInfo(Vector3i a_RelPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight);
	void GetBlockInfo(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight)
	{
		GetBlockInfo({ a_RelX, a_RelY, a_RelZ }, a_BlockType, a_Meta, a_SkyLight, a_BlockLight);
	}

	/** Convert absolute coordinates into relative coordinates.
	Returns false on failure to obtain a valid chunk. Returns true otherwise.
	@param a_Position The position you'd like to convert, a_Position need not be in the calling chunk and can safely be out
	of its bounds, but for best performance, it should not be too far from the calling chunk.
	@param a_Chunk Returns the chunk in which a_Position is in. If a_Position is within the calling chunk's bounds,
	returns the calling chunk. For best performance, a_Position shouldn't be too far from the calling chunk.
	@param a_Rel Returns the converted relative position. Note that it is relative to the returned a_Chunk.
	The vector will not be modified if the function returns false. */
	bool GetChunkAndRelByAbsolute(const Vector3d & a_Position, cChunk ** a_Chunk, Vector3i & a_Rel);

	/** Convert absolute coordinates into relative coordinates.
	Returns false on failure to obtain a valid chunk. Returns true otherwise.
	@param a_Position The position you'd like to convert, a_Position need not be in the calling chunk and can safely be out
	of its bounds, but for best performance, it should not be too far from the calling chunk.
	@param a_Chunk Returns the chunk in which a_Position is in. If a_Position is within the calling chunk's bounds,
	returns the calling chunk. For best performance, a_Position shouldn't be too far from the calling chunk.
	@param a_Rel Returns the converted relative position. Note that it is relative to the returned a_Chunk.
	The vector will not be modified if the function returns false. */
	bool GetChunkAndRelByAbsolute(const Vector3i & a_Position, cChunk ** a_Chunk, Vector3i & a_Rel);

	/** Returns the chunk into which the specified block belongs, by walking the neighbors.
	Will return self if appropriate. Returns nullptr if not reachable through neighbors.
	*/
	cChunk * GetNeighborChunk(int a_BlockX, int a_BlockZ);

	/**
	Returns the chunk into which the relatively-specified block belongs, by walking the neighbors.
	Will return self if appropriate. Returns nullptr if not reachable through neighbors.
	*/
	cChunk * GetRelNeighborChunk(int a_RelX, int a_RelZ);

	/**
	Returns the chunk into which the relatively-specified block belongs.
	Also modifies the relative coords from this-relative to return-relative.
	Will return self if appropriate.
	Will try walking the neighbors first; if that fails, will query the chunkmap
	*/
	cChunk * GetRelNeighborChunkAdjustCoords(int & a_RelX, int & a_RelZ) const;

	EMCSBiome GetBiomeAt(int a_RelX, int a_RelZ) const {return cChunkDef::GetBiome(m_BiomeMap, a_RelX, a_RelZ); }

	/** Sets the biome at the specified relative coords.
	Doesn't resend the chunk to clients. */
	void SetBiomeAt(int a_RelX, int a_RelZ, EMCSBiome a_Biome);

	/** Sets the biome in the specified relative coords area. All the coords are inclusive.
	Sends the chunk to all relevant clients. */
	void SetAreaBiome(int a_MinRelX, int a_MaxRelX, int a_MinRelZ, int a_MaxRelZ, EMCSBiome a_Biome);

	void CollectPickupsByPlayer(cPlayer & a_Player);

	/** Sets the sign text. Returns true if successful. Also sends update packets to all clients in the chunk */
	bool SetSignLines(int a_RelX, int a_RelY, int a_RelZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);

	int  GetHeight( int a_X, int a_Z);

	void SendBlockTo(int a_RelX, int a_RelY, int a_RelZ, cClientHandle * a_Client);

	/** Adds a client to the chunk; returns true if added, false if already there */
	bool AddClient(cClientHandle * a_Client);

	/** Removes the specified client from the chunk; ignored if client not in chunk. */
	void RemoveClient(cClientHandle * a_Client);

	/** Returns true if the specified client is present in this chunk. */
	bool HasClient(cClientHandle * a_Client);

	/** Returns true if theres any client in the chunk; false otherwise */
	bool HasAnyClients(void) const;

	void AddEntity(OwnedEntity a_Entity);

	/** Releases ownership of the given entity if it was found in this chunk.
	Returns an owning reference to the found entity. */
	OwnedEntity RemoveEntity(cEntity & a_Entity);

	bool HasEntity(UInt32 a_EntityID);

	/** Calls the callback for each entity; returns true if all entities processed, false if the callback aborted by returning true */
	bool ForEachEntity(cEntityCallback a_Callback);  // Lua-accessible

	/** Calls the callback for each entity that has a nonempty intersection with the specified boundingbox.
	Returns true if all entities processed, false if the callback aborted by returning true. */
	bool ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback a_Callback);  // Lua-accessible

	/** Calls the callback if the entity with the specified ID is found, with the entity object as the callback param. Returns true if entity found. */
	bool DoWithEntityByID(UInt32 a_EntityID, cEntityCallback a_Callback, bool & a_CallbackResult);  // Lua-accessible

	/** Calls the callback for each tyEntity; returns true if all block entities processed, false if the callback aborted by returning true
	tBlocktypes are all blocktypes convertible to tyEntity which are to be called. If no block type is given the callback is called for every block entity
	Accessible only from within Chunk.cpp */
	template <class tyEntity, BLOCKTYPE... tBlocktype>
	bool GenericForEachBlockEntity(cFunctionRef<bool(tyEntity &)> a_Callback);

	/** Calls the callback for each block entity; returns true if all block entities processed, false if the callback aborted by returning true */
	bool ForEachBlockEntity(cBlockEntityCallback a_Callback);  // Lua-accessible

	/** Calls the callback for each brewingstand; returns true if all brewingstands processed, false if the callback aborted by returning true */
	bool ForEachBrewingstand(cBrewingstandCallback a_Callback);  // Lua-accessible

	/** Calls the callback for each chest; returns true if all chests processed, false if the callback aborted by returning true */
	bool ForEachChest(cChestCallback a_Callback);  // Lua-accessible

	/** Calls the callback for each dispenser; returns true if all dispensers processed, false if the callback aborted by returning true */
	bool ForEachDispenser(cDispenserCallback a_Callback);

	/** Calls the callback for each dropper; returns true if all droppers processed, false if the callback aborted by returning true */
	bool ForEachDropper(cDropperCallback a_Callback);

	/** Calls the callback for each dropspenser; returns true if all dropspensers processed, false if the callback aborted by returning true */
	bool ForEachDropSpenser(cDropSpenserCallback a_Callback);

	/** Calls the callback for each furnace; returns true if all furnaces processed, false if the callback aborted by returning true */
	bool ForEachFurnace(cFurnaceCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the tyEntity at the specified coords; returns false if there's no such block entity at those coords, true if found
	tBlocktype is a list of the blocktypes to be called. If no BLOCKTYPE template arguments are given the callback is called for any block entity
	Accessible only from within Chunk.cpp */
	template <class tyEntity, BLOCKTYPE... tBlocktype>
	bool GenericDoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFunctionRef<bool(tyEntity &)> a_Callback);

	/** Calls the callback for the block entity at the specified coords; returns false if there's no block entity at those coords, true if found */
	bool DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the beacon at the specified coords; returns false if there's no beacon at those coords, true if found */
	bool DoWithBeaconAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the brewingstand at the specified coords; returns false if there's no brewingstand at those coords, true if found */
	bool DoWithBrewingstandAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBrewingstandCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the bed at the specified coords; returns false if there's no bed at those coords, true if found */
	bool DoWithBedAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBedCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the chest at the specified coords; returns false if there's no chest at those coords, true if found */
	bool DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the dispenser at the specified coords; returns false if there's no dispenser at those coords or callback returns true, returns true if found */
	bool DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback a_Callback);

	/** Calls the callback for the dispenser at the specified coords; returns false if there's no dropper at those coords or callback returns true, returns true if found */
	bool DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback a_Callback);

	/** Calls the callback for the dispenser at the specified coords; returns false if there's no dropspenser at those coords or callback returns true, returns true if found */
	bool DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback a_Callback);

	/** Calls the callback for the furnace at the specified coords; returns false if there's no furnace at those coords or callback returns true, returns true if found */
	bool DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the noteblock at the specified coords; returns false if there's no noteblock at those coords or callback returns true, returns true if found */
	bool DoWithNoteBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cNoteBlockCallback a_Callback);

	/** Calls the callback for the command block at the specified coords; returns false if there's no command block at those coords or callback returns true, returns true if found */
	bool DoWithCommandBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cCommandBlockCallback a_Callback);

	/** Calls the callback for the mob head block at the specified coords; returns false if there's no mob head block at those coords or callback returns true, returns true if found */
	bool DoWithMobHeadAt(int a_BlockX, int a_BlockY, int a_BlockZ, cMobHeadCallback a_Callback);

	/** Calls the callback for the flower pot at the specified coords; returns false if there's no flower pot at those coords or callback returns true, returns true if found */
	bool DoWithFlowerPotAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFlowerPotCallback a_Callback);

	/** Retrieves the test on the sign at the specified coords; returns false if there's no sign at those coords, true if found */
	bool GetSignLines (int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4);  // Lua-accessible

	/** Use block entity on coordinate.
	returns true if the use was successful, return false to use the block as a "normal" block */
	bool UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z);  // [x, y, z] in world block coords

	void CalculateHeightmap(const BLOCKTYPE * a_BlockTypes);

	// Broadcast various packets to all clients of this chunk:
	// (Please keep these alpha-sorted)
	void BroadcastAttachEntity       (const cEntity & a_Entity, const cEntity & a_Vehicle);
	void BroadcastBlockAction        (Vector3i a_BlockPos, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude = nullptr);
	void BroadcastBlockBreakAnimation(UInt32 a_EntityID, Vector3i a_BlockPos, char a_Stage, const cClientHandle * a_Exclude = nullptr);
	void BroadcastBlockEntity        (Vector3i a_BlockPos, const cClientHandle * a_Exclude = nullptr);
	void BroadcastCollectEntity      (const cEntity & a_Entity, const cPlayer & a_Player, int a_Count, const cClientHandle * a_Exclude = nullptr);
	void BroadcastDestroyEntity      (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastDetachEntity       (const cEntity & a_Entity, const cEntity & a_PreviousVehicle);
	void BroadcastEntityEffect       (const cEntity & a_Entity, int a_EffectID, int a_Amplifier, short a_Duration, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityEquipment    (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityHeadLook     (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityLook         (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityMetadata     (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityRelMove      (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityRelMoveLook  (const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityStatus       (const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityVelocity     (const cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastEntityAnimation    (const cEntity & a_Entity, char a_Animation, const cClientHandle * a_Exclude = nullptr);
	void BroadcastLeashEntity        (const cEntity & a_Entity, const cEntity & a_EntityLeashedTo);
	void BroadcastParticleEffect     (const AString & a_ParticleName, float a_SrcX, float a_SrcY, float a_SrcZ, float a_OffsetX, float a_OffsetY, float a_OffsetZ, float a_ParticleData, int a_ParticleAmount, cClientHandle * a_Exclude = nullptr);
	void BroadcastRemoveEntityEffect (const cEntity & a_Entity, int a_EffectID, const cClientHandle * a_Exclude = nullptr);
	void BroadcastSoundEffect        (const AString & a_SoundName, Vector3d a_Position, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = nullptr);
	void BroadcastSoundParticleEffect(const EffectID a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude = nullptr);
	void BroadcastSpawnEntity        (cEntity & a_Entity, const cClientHandle * a_Exclude = nullptr);
	void BroadcastThunderbolt        (Vector3i a_BlockPos, const cClientHandle * a_Exclude = nullptr);
	void BroadcastUnleashEntity      (const cEntity & a_Entity);
	void BroadcastUseBed             (const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ);

	void SendBlockEntity             (int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client);

	Vector3i PositionToWorldPosition(Vector3i a_RelPos)
	{
		return PositionToWorldPosition(a_RelPos.x, a_RelPos.y, a_RelPos.z);
	}

	void     PositionToWorldPosition(int a_RelX, int a_RelY, int a_RelZ, int & a_BlockX, int & a_BlockY, int & a_BlockZ);
	Vector3i PositionToWorldPosition(int a_RelX, int a_RelY, int a_RelZ);

	inline void MarkDirty(void)
	{
		m_IsDirty = true;
		m_IsSaving = false;
	}

	/** Sets the blockticking to start at the specified block. Only one blocktick may be set, second call overwrites the first call */
	inline void SetNextBlockTick(int a_RelX, int a_RelY, int a_RelZ)
	{
		m_BlockTickX = a_RelX;
		m_BlockTickY = a_RelY;
		m_BlockTickZ = a_RelZ;
	}

	inline NIBBLETYPE GetMeta(int a_RelX, int a_RelY, int a_RelZ) const
	{
		return m_ChunkData.GetMeta({ a_RelX, a_RelY, a_RelZ });
	}

	NIBBLETYPE GetMeta(Vector3i a_RelPos) const { return m_ChunkData.GetMeta(a_RelPos); }

	void SetMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Meta, bool a_ShouldMarkDirty = true, bool a_ShouldInformClients = true)
	{
		SetMeta({ a_RelX, a_RelY, a_RelZ }, a_Meta, a_ShouldMarkDirty, a_ShouldInformClients);
	}

	/** Set a meta value, with the option of not informing the client and / or not marking dirty.
	Used for setting metas that are of little value for saving to disk and / or for sending to the client,
	such as leaf decay flags. */
	inline void SetMeta(Vector3i a_RelPos, NIBBLETYPE a_Meta, bool a_ShouldMarkDirty = true, bool a_ShouldInformClients = true)
	{
		bool hasChanged = m_ChunkData.SetMeta(a_RelPos, a_Meta);
		if (hasChanged)
		{
			if (a_ShouldMarkDirty)
			{
				MarkDirty();
			}
			if (a_ShouldInformClients)
			{
				m_PendingSendBlocks.push_back(sSetBlock(m_PosX, m_PosZ, a_RelPos.x, a_RelPos.y, a_RelPos.z, GetBlock(a_RelPos), a_Meta));
			}
		}
	}

	/** Light alterations based on time */
	NIBBLETYPE GetTimeAlteredLight(NIBBLETYPE a_Skylight) const;

	/** Get the level of artificial light illuminating the block (0 - 15) */
	inline NIBBLETYPE GetBlockLight(Vector3i a_RelPos) const { return m_ChunkData.GetBlockLight(a_RelPos); }
	inline NIBBLETYPE GetBlockLight(int a_RelX, int a_RelY, int a_RelZ) const { return m_ChunkData.GetBlockLight({ a_RelX, a_RelY, a_RelZ }); }

	/** Get the level of sky light illuminating the block (0 - 15) independent of daytime. */
	inline NIBBLETYPE GetSkyLight(Vector3i a_RelPos) const { return m_ChunkData.GetSkyLight(a_RelPos); }
	inline NIBBLETYPE GetSkyLight(int a_RelX, int a_RelY, int a_RelZ) const { return m_ChunkData.GetSkyLight({ a_RelX, a_RelY, a_RelZ }); }

	/** Get the level of sky light illuminating the block (0 - 15), taking daytime into a account. */
	inline NIBBLETYPE GetSkyLightAltered(Vector3i a_RelPos) const { return GetTimeAlteredLight(m_ChunkData.GetSkyLight(a_RelPos)); }
	inline NIBBLETYPE GetSkyLightAltered(int a_RelX, int a_RelY, int a_RelZ) const { return GetSkyLightAltered({ a_RelX, a_RelY, a_RelZ }); }

	/** Same as GetBlock(), but relative coords needn't be in this chunk (uses m_Neighbor-s or m_ChunkMap in such a case); returns true on success */
	bool UnboundedRelGetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta) const;

	/** Same as GetBlockType(), but relative coords needn't be in this chunk (uses m_Neighbor-s or m_ChunkMap in such a case); returns true on success */
	bool UnboundedRelGetBlockType(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType) const;

	/** Same as GetBlockMeta(), but relative coords needn't be in this chunk (uses m_Neighbor-s or m_ChunkMap in such a case); returns true on success */
	bool UnboundedRelGetBlockMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockMeta) const;

	/** Same as GetBlockBlockLight(), but relative coords needn't be in this chunk (uses m_Neighbor-s or m_ChunkMap in such a case); returns true on success */
	bool UnboundedRelGetBlockBlockLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockLight) const;

	/** Same as GetBlockSkyLight(), but relative coords needn't be in this chunk (uses m_Neighbor-s or m_ChunkMap in such a case); returns true on success */
	bool UnboundedRelGetBlockSkyLight(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_SkyLight) const;

	/** Queries both BlockLight and SkyLight, relative coords needn't be in this chunk (uses m_Neighbor-s or m_ChunkMap in such a case); returns true on success */
	bool UnboundedRelGetBlockLights(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE & a_BlockLight, NIBBLETYPE & a_SkyLight) const;

	/** Same as SetBlock(), but relative coords needn't be in this chunk (uses m_Neighbor-s or m_ChunkMap in such a case); returns true on success */
	bool UnboundedRelSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/** Same as FastSetBlock(), but relative coords needn't be in this chunk (uses m_Neighbor-s or m_ChunkMap in such a case); returns true on success */
	bool UnboundedRelFastSetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/** Same as QueueTickBlock(), but relative coords needn't be in this chunk (uses m_Neighbor-s in such a case), ignores unsuccessful attempts */
	void UnboundedQueueTickBlock(int a_RelX, int a_RelY, int a_RelZ);



	// Per-chunk simulator data:
	cFireSimulatorChunkData & GetFireSimulatorData (void) { return m_FireSimulatorData; }
	cFluidSimulatorData *     GetWaterSimulatorData(void) { return m_WaterSimulatorData; }
	cFluidSimulatorData *     GetLavaSimulatorData (void) { return m_LavaSimulatorData; }
	cSandSimulatorChunkData & GetSandSimulatorData (void) { return m_SandSimulatorData; }

	cRedstoneSimulatorChunkData * GetRedstoneSimulatorData(void) { return m_RedstoneSimulatorData; }
	void SetRedstoneSimulatorData(cRedstoneSimulatorChunkData * a_Data) { m_RedstoneSimulatorData = a_Data; }

	cBlockEntity * GetBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ);
	cBlockEntity * GetBlockEntity(const Vector3i & a_BlockPos) { return GetBlockEntity(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z); }

	/** Returns true if the chunk should be ticked in the tick-thread.
	Checks if there are any clients and if the always-tick flag is set */
	bool ShouldBeTicked(void) const;

	/** Increments (a_AlwaysTicked == true) or decrements (false) the m_AlwaysTicked counter.
	If the m_AlwaysTicked counter is greater than zero, the chunk is ticked in the tick-thread regardless of
	whether it has any clients or not. When this is set, the chunk never unloads.
	This function allows nesting and task-concurrency (multiple separate tasks can request ticking and as long
	as at least one requests is active the chunk will be ticked). */
	void SetAlwaysTicked(bool a_AlwaysTicked);

	// Makes a copy of the list
	cClientHandleList GetAllClients(void) const
	{
		return cClientHandleList(m_LoadedByClient.begin(), m_LoadedByClient.end());
	}

private:

	friend class cChunkMap;

	struct sSetBlockQueueItem
	{
		Int64 m_Tick;
		int m_RelX, m_RelY, m_RelZ;
		BLOCKTYPE m_BlockType;
		NIBBLETYPE m_BlockMeta;
		BLOCKTYPE m_PreviousType;

		sSetBlockQueueItem(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Int64 a_Tick, BLOCKTYPE a_PreviousBlockType) :
			m_Tick(a_Tick), m_RelX(a_RelX), m_RelY(a_RelY), m_RelZ(a_RelZ), m_BlockType(a_BlockType), m_BlockMeta(a_BlockMeta), m_PreviousType(a_PreviousBlockType)
		{
		}
	} ;

	typedef std::vector<sSetBlockQueueItem> sSetBlockQueueVector;


	/** Holds the presence status of the chunk - if it is present, or in the loader / generator queue, or unloaded */
	ePresence m_Presence;

	/** If the chunk fails to load, should it be queued in the generator or reset back to invalid? */
	bool m_ShouldGenerateIfLoadFailed;
	bool m_IsLightValid;   // True if the blocklight and skylight are calculated
	bool m_IsDirty;        // True if the chunk has changed since it was last saved
	bool m_IsSaving;       // True if the chunk is being saved
	bool m_HasLoadFailed;  // True if chunk failed to load and hasn't been generated yet since then

	std::vector<Vector3i> m_ToTickBlocks;
	sSetBlockVector       m_PendingSendBlocks;  ///< Blocks that have changed and need to be sent to all clients

	// A critical section is not needed, because all chunk access is protected by its parent ChunkMap's csLayers
	std::vector<cClientHandle *> m_LoadedByClient;
	std::vector<OwnedEntity> m_Entities;
	cBlockEntities               m_BlockEntities;

	/** Number of times the chunk has been requested to stay (by various cChunkStay objects); if zero, the chunk can be unloaded */
	int m_StayCount;

	int m_PosX, m_PosZ;
	cWorld *    m_World;
	cChunkMap * m_ChunkMap;

	cChunkData m_ChunkData;

	cChunkDef::HeightMap m_HeightMap;
	cChunkDef::BiomeMap  m_BiomeMap;

	int m_BlockTickX, m_BlockTickY, m_BlockTickZ;

	cChunk * m_NeighborXM;  // Neighbor at [X - 1, Z]
	cChunk * m_NeighborXP;  // Neighbor at [X + 1, Z]
	cChunk * m_NeighborZM;  // Neighbor at [X,     Z - 1]
	cChunk * m_NeighborZP;  // Neighbor at [X,     Z + 1]

	// Per-chunk simulator data:
	cFireSimulatorChunkData m_FireSimulatorData;
	cFluidSimulatorData *   m_WaterSimulatorData;
	cFluidSimulatorData *   m_LavaSimulatorData;
	cSandSimulatorChunkData m_SandSimulatorData;

	cRedstoneSimulatorChunkData * m_RedstoneSimulatorData;

	/** If greater than zero, the chunk is ticked even if it has no clients.
	Manipulated by the SetAlwaysTicked() function, allows for nested calls of the function.
	This is the support for plugin-accessible chunk tick forcing. */
	int m_AlwaysTicked;


	// Pick up a random block of this chunk
	void GetRandomBlockCoords(int & a_X, int & a_Y, int & a_Z);
	void GetThreeRandomNumbers(int & a_X, int & a_Y, int & a_Z, int a_MaxX, int a_MaxY, int a_MaxZ);

	void RemoveBlockEntity(cBlockEntity * a_BlockEntity);
	void AddBlockEntity   (cBlockEntity * a_BlockEntity);

	/** Add a block entity to the chunk without marking the chunk dirty */
	void AddBlockEntityClean(cBlockEntity * a_BlockEntity);

	/** Creates a block entity for each block that needs a block entity and doesn't have one already */
	void CreateBlockEntities(void);

	/** Wakes up each simulator for its specific blocks; through all the blocks in the chunk */
	void WakeUpSimulators(void);

	/** Sends m_PendingSendBlocks to all clients */
	void BroadcastPendingBlockChanges(void);

	/** Checks the block scheduled for checking in m_ToTickBlocks[] */
	void CheckBlocks();

	/** Ticks several random blocks in the chunk */
	void TickBlocks(void);

	/** Adds snow to the top of snowy biomes and hydrates farmland / fills cauldrons in rainy biomes */
	void ApplyWeatherToTop(void);

	/** Grows sugarcane by the specified number of blocks, but no more than 3 blocks high (used by both bonemeal and ticking); returns the amount of blocks the sugarcane grew inside this call */
	int GrowSugarcane   (int a_RelX, int a_RelY, int a_RelZ, int a_NumBlocks);

	/** Grows cactus by the specified number of blocks, but no more than 3 blocks high (used by both bonemeal and ticking); returns the amount of blocks the cactus grew inside this call */
	int GrowCactus      (int a_RelX, int a_RelY, int a_RelZ, int a_NumBlocks);

	/** Grows a tall grass present at the block specified to a two tall grass; returns true if the grass grew */
	bool GrowTallGrass      (int a_RelX, int a_RelY, int a_RelZ);

	/** Grows a melon or a pumpkin next to the block specified (assumed to be the stem); returns true if the pumpkin or melon sucessfully grew */
	bool GrowMelonPumpkin(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType);

	/** Called by Tick() when an entity moves out of this chunk into a neighbor; moves the entity and sends spawn / despawn packet to clients */
	void MoveEntityToNewChunk(OwnedEntity a_Entity);
};

typedef cChunk * cChunkPtr;

typedef std::list<cChunkPtr> cChunkPtrList;




