
// cChunkMap.h

// Interfaces to the cChunkMap class representing the chunk storage for a single world

#pragma once


#include <functional>

#include "ChunkDataCallback.h"
#include "EffectID.h"
#include "FunctionRef.h"




class cWorld;
class cItem;
class cItems;
class cChunkStay;
class cChunk;
class cPlayer;
class cBeaconEntity;
class cBedEntity;
class cBrewingstandEntity;
class cChestEntity;
class cDispenserEntity;
class cDropperEntity;
class cDropSpenserEntity;
class cFurnaceEntity;
class cHopperEntity;
class cNoteEntity;
class cCommandBlockEntity;
class cMobHeadEntity;
class cFlowerPotEntity;
class cBlockArea;
class cMobCensus;
class cMobSpawner;
class cSetChunkData;
class cBoundingBox;
class cDeadlockDetect;

typedef std::list<cClientHandle *> cClientHandleList;
using cEntityCallback       = cFunctionRef<bool(cEntity             &)>;
using cBeaconCallback       = cFunctionRef<bool(cBeaconEntity       &)>;
using cBedCallback          = cFunctionRef<bool(cBedEntity          &)>;
using cBlockEntityCallback  = cFunctionRef<bool(cBlockEntity        &)>;
using cBrewingstandCallback = cFunctionRef<bool(cBrewingstandEntity &)>;
using cChestCallback        = cFunctionRef<bool(cChestEntity        &)>;
using cChunkCallback        = cFunctionRef<bool(cChunk              &)>;
using cDispenserCallback    = cFunctionRef<bool(cDispenserEntity    &)>;
using cDropperCallback      = cFunctionRef<bool(cDropperEntity      &)>;
using cDropSpenserCallback  = cFunctionRef<bool(cDropSpenserEntity  &)>;
using cFurnaceCallback      = cFunctionRef<bool(cFurnaceEntity      &)>;
using cHopperCallback       = cFunctionRef<bool(cHopperEntity       &)>;
using cNoteBlockCallback    = cFunctionRef<bool(cNoteEntity         &)>;
using cCommandBlockCallback = cFunctionRef<bool(cCommandBlockEntity &)>;
using cMobHeadCallback      = cFunctionRef<bool(cMobHeadEntity      &)>;
using cFlowerPotCallback    = cFunctionRef<bool(cFlowerPotEntity    &)>;





class cChunkMap
{
public:

	cChunkMap(cWorld * a_World);
	~cChunkMap();

	/** Sends the block entity, if it is at the coords specified, to a_Client */
	void SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client);

	/** a_Player rclked block entity at the coords specified, handle it
	returns true if the use was successful, return false to use the block as a "normal" block */
	bool UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z);

	/** Calls the callback for the chunk specified, with ChunkMapCS locked; returns false if the chunk doesn't exist, otherwise returns the same value as the callback */
	bool DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback a_Callback);

	/** Calls the callback for the chunk at the block position specified, with ChunkMapCS locked; returns false if the chunk doesn't exist, otherwise returns the same value as the callback */
	bool DoWithChunkAt(Vector3i a_BlockPos, cChunkCallback a_Callback);

	/** Wakes up simulators for the specified block */
	void WakeUpSimulators(Vector3i a_Block);

	// DEPRECATED, use the vector-parametered version instead.
	void WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		LOGWARNING("cChunkMap::WakeUpSimulators(int, int, int) is deprecated, use cChunkMap::WakeUpSimulators(Vector3i) instead.");
		WakeUpSimulators(Vector3i(a_BlockX, a_BlockY, a_BlockZ));
	}

	void MarkChunkDirty     (int a_ChunkX, int a_ChunkZ);
	void MarkChunkSaving    (int a_ChunkX, int a_ChunkZ);
	void MarkChunkSaved     (int a_ChunkX, int a_ChunkZ);

	/** Sets the chunk data as either loaded from the storage or generated.
	BlockLight and BlockSkyLight are optional, if not present, chunk will be marked as unlighted.
	If MarkDirty is set, the chunk is set as dirty (used after generating)
	Modifies the BlockEntity list in a_SetChunkData - moves the block entities into the chunk.
	*/
	void SetChunkData(cSetChunkData & a_SetChunkData);

	void ChunkLighted(
		int a_ChunkX, int a_ChunkZ,
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_SkyLight
	);

	/** Calls the callback with the chunk's data, if available (with ChunkCS locked).
	Returns true if the chunk was reported successfully, false if not (chunk not present or callback failed). */
	bool GetChunkData(cChunkCoords a_Coords, cChunkDataCallback & a_Callback);

	/** Copies the chunk's blocktypes into a_Blocks; returns true if successful */
	bool GetChunkBlockTypes (int a_ChunkX, int a_ChunkZ, BLOCKTYPE * a_Blocks);

	/** Returns true iff the chunk is in the loader / generator queue. */
	bool IsChunkQueued(int a_ChunkX, int a_ChunkZ);

	bool      IsChunkValid       (int a_ChunkX, int a_ChunkZ);
	bool      HasChunkAnyClients (int a_ChunkX, int a_ChunkZ);
	int       GetHeight          (int a_BlockX, int a_BlockZ);  // Waits for the chunk to get loaded / generated
	bool      TryGetHeight       (int a_BlockX, int a_BlockZ, int & a_Height);  // Returns false if chunk not loaded / generated

	/** Sets the block at the specified coords to the specified value.
	The replacement doesn't trigger block updates, nor wake up simulators.
	The replaced blocks aren't checked for block entities (block entity is leaked if it exists at this block).
	If the chunk is invalid, the operation is ignored silently. */
	void FastSetBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/** Makes the specified player collect all the pickups around them. */
	void CollectPickupsByPlayer(cPlayer & a_Player);

	BLOCKTYPE  GetBlock          (Vector3i a_BlockPos);
	NIBBLETYPE GetBlockMeta      (Vector3i a_BlockPos);
	NIBBLETYPE GetBlockSkyLight  (Vector3i a_BlockPos);
	NIBBLETYPE GetBlockBlockLight(Vector3i a_BlockPos);

	/** Sets the meta for the specified block, while keeping the blocktype.
	Ignored if the chunk is invalid. */
	void SetBlockMeta(Vector3i a_BlockPos, NIBBLETYPE a_BlockMeta);

	void       SetBlock          (Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	bool       GetBlockTypeMeta  (Vector3i a_BlockPos, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);
	bool       GetBlockInfo      (Vector3i, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight);

	/** Special function used for growing trees, replaces only blocks that tree may overwrite */
	void      ReplaceTreeBlocks(const sSetBlockVector & a_Blocks);

	/** Returns the biome at the specified coords. Reads the biome from the chunk, if loaded, otherwise uses the world generator to provide the biome value */
	EMCSBiome GetBiomeAt (int a_BlockX, int a_BlockZ);

	/** Sets the biome at the specified coords. Returns true if successful, false if not (chunk not loaded).
	Doesn't resend the chunk to clients. */
	bool SetBiomeAt(int a_BlockX, int a_BlockZ, EMCSBiome a_Biome);

	/** Sets the biome at the area. Returns true if successful, false if any subarea failed (chunk not loaded).
	(Re)sends the chunks to their relevant clients if successful. */
	bool SetAreaBiome(int a_MinX, int a_MaxX, int a_MinZ, int a_MaxZ, EMCSBiome a_Biome);

	/** Retrieves block types and metas of the specified blocks.
	If a chunk is not loaded, doesn't modify the block and consults a_ContinueOnFailure whether to process the rest of the array.
	Returns true if all blocks were read, false if any one failed. */
	bool GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure);

	/** Removes the block at the specified coords and wakes up simulators.
	Returns false if the chunk is not loaded (and the block is not dug).
	Returns true if successful. */
	bool DigBlock(Vector3i a_BlockPos);

	/** Returns all the pickups that would result if the a_Digger dug up the block at a_BlockPos using a_Tool.
	a_Digger is usually a player, but can be nullptr for natural causes.
	a_Tool is an optional item used to dig up the block, used by the handlers (empty hand vs shears produce different pickups from leaves).
	An empty hand is assumed if a_Tool is nullptr.
	Returns an empty cItems object if the chunk is not present. */
	cItems PickupsFromBlock(Vector3i a_BlockPos, const cEntity * a_Digger, const cItem * a_Tool);

	/** Sends the block at the specified coords to the specified player.
	Uses a blockchange packet to send the block.
	If the relevant chunk isn't loaded, doesn't do anything. */
	void SendBlockTo(int a_BlockX, int a_BlockY, int a_BlockZ, cPlayer & a_Player);

	/** Compares clients of two chunks, calls the callback accordingly */
	void CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback);

	/** Compares clients of two chunks, calls the callback accordingly */
	void CompareChunkClients(cChunk * a_Chunk1, cChunk * a_Chunk2, cClientDiffCallback & a_Callback);

	/** Adds client to a chunk, if not already present; returns true if added, false if present */
	bool AddChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);

	/** Removes the client from the chunk */
	void RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);

	/** Removes the client from all chunks it is present in */
	void RemoveClientFromChunks(cClientHandle * a_Client);

	/** Adds the entity to its appropriate chunk, takes ownership of the entity pointer */
	void AddEntity(OwnedEntity a_Entity);

	/** Adds the player to its appropriate chunk, takes ownership of the player pointer */
	void AddPlayer(std::unique_ptr<cPlayer> a_Player);

	/** Returns true if the entity with specified ID is present in the chunks */
	bool HasEntity(UInt32 a_EntityID) const;

	/** Removes the entity from its appropriate chunk
	Returns an owning reference to the found entity. */
	OwnedEntity RemoveEntity(cEntity & a_Entity);

	/** Calls the callback for each entity in the entire world; returns true if all entities processed, false if the callback aborted by returning true */
	bool ForEachEntity(cEntityCallback a_Callback) const;  // Lua-accessible

	/** Calls the callback for each entity in the specified chunk; returns true if all entities processed, false if the callback aborted by returning true */
	bool ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback a_Callback);  // Lua-accessible

	/** Calls the callback for each entity that has a nonempty intersection with the specified boundingbox.
	Returns true if all entities processed, false if the callback aborted by returning true.
	If any chunk in the box is missing, ignores the entities in that chunk silently. */
	bool ForEachEntityInBox(const cBoundingBox & a_Box, cEntityCallback a_Callback);  // Lua-accessible

	/** Calls the callback if the entity with the specified ID is found, with the entity object as the callback param.
	Returns true if entity found and callback returned false. */
	bool DoWithEntityByID(UInt32 a_EntityID, cEntityCallback a_Callback) const;  // Lua-accessible

	/** Calls the callback for each block entity in the specified chunk.
	Returns true if all block entities processed, false if the callback aborted by returning true. */
	bool ForEachBlockEntityInChunk(int a_ChunkX, int a_ChunkZ, cBlockEntityCallback a_Callback);  // Lua-accessible

	/** Calls the callback for brewingstand in the specified chunk.
	Returns true if all brewingstands processed, false if the callback aborted by returning true. */
	bool ForEachBrewingstandInChunk(int a_ChunkX, int a_ChunkZ, cBrewingstandCallback a_Callback);  // Lua-accessible

	/** Calls the callback for each chest in the specified chunk.
	Returns true if all chests processed, false if the callback aborted by returning true. */
	bool ForEachChestInChunk(int a_ChunkX, int a_ChunkZ, cChestCallback a_Callback);  // Lua-accessible

	/** Calls the callback for each dispenser in the specified chunk.
	Returns true if all dispensers processed, false if the callback aborted by returning true. */
	bool ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback a_Callback);

	/** Calls the callback for each dropper in the specified chunk.
	Returns true if all droppers processed, false if the callback aborted by returning true. */
	bool ForEachDropperInChunk(int a_ChunkX, int a_ChunkZ, cDropperCallback a_Callback);

	/** Calls the callback for each dropspenser in the specified chunk.
	Returns true if all dropspensers processed, false if the callback aborted by returning true. */
	bool ForEachDropSpenserInChunk(int a_ChunkX, int a_ChunkZ, cDropSpenserCallback a_Callback);

	/** Calls the callback for each furnace in the specified chunk.
	Returns true if all furnaces processed, false if the callback aborted by returning true. */
	bool ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the block entity at the specified coords.
	Returns false if there's no block entity at those coords, true if found. */
	bool DoWithBlockEntityAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBlockEntityCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the beacon at the specified coords.
	Returns false if there's no beacon at those coords, true if found. */
	bool DoWithBeaconAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBeaconCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the bed at the specified coords.
	Returns false if there's no bed at those coords, true if found. */
	bool DoWithBedAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBedCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the brewingstand at the specified coords; returns false if there's no brewingstand at those coords, true if found */
	bool DoWithBrewingstandAt(int a_BlockX, int a_BlockY, int a_BlockZ, cBrewingstandCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the chest at the specified coords.
	Returns false if there's no chest at those coords, true if found. */
	bool DoWithChestAt(int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback a_Callback);  // Lua-acessible

	/** Calls the callback for the dispenser at the specified coords.
	Returns false if there's no dispenser at those coords or callback returns true, returns true if found. */
	bool DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the dropper at the specified coords.
	Returns false if there's no dropper at those coords or callback returns true, returns true if found. */
	bool DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the dropspenser at the specified coords.
	Returns false if there's no dropspenser at those coords or callback returns true, returns true if found. */
	bool DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the furnace at the specified coords.
	Returns false if there's no furnace at those coords or callback returns true, returns true if found. */
	bool DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the hopper at the specified coords.
	Returns false if there's no hopper at those coords or callback returns true, returns true if found. */
	bool DoWithHopperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cHopperCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the noteblock at the specified coords.
	Returns false if there's no noteblock at those coords or callback returns true, returns true if found. */
	bool DoWithNoteBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cNoteBlockCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the command block at the specified coords.
	Returns false if there's no command block at those coords or callback returns true, returns true if found. */
	bool DoWithCommandBlockAt(int a_BlockX, int a_BlockY, int a_BlockZ, cCommandBlockCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the mob head block at the specified coords.
	Returns false if there's no mob head block at those coords or callback returns true, returns true if found. */
	bool DoWithMobHeadAt(int a_BlockX, int a_BlockY, int a_BlockZ, cMobHeadCallback a_Callback);  // Lua-accessible

	/** Calls the callback for the flower pot at the specified coords.
	Returns false if there's no flower pot at those coords or callback returns true, returns true if found. */
	bool DoWithFlowerPotAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFlowerPotCallback a_Callback);  // Lua-accessible

	/** Retrieves the test on the sign at the specified coords.
	Returns false if there's no sign at those coords, true if found. */
	bool GetSignLines (int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4);  // Lua-accessible

	/** Queues the chunk for preparing - making sure that it's generated and lit.
	The specified chunk is queued to be loaded or generated, and lit if needed.
	The specified callback is called after the chunk has been prepared. If there's no preparation to do, only the callback is called.
	It is legal to call without the callback. */
	void PrepareChunk(int a_ChunkX, int a_ChunkZ, std::unique_ptr<cChunkCoordCallback> a_CallAfter = {});  // Lua-accessible

	/** Queues the chunk for generating.
	First attempts to load the chunk from the storage. If that fails, queues the chunk for generating. */
	void GenerateChunk(int a_ChunkX, int a_ChunkZ);  // Lua-accessible

	/** Marks the chunk as failed-to-load */
	void ChunkLoadFailed(int a_ChunkX, int a_ChunkZ);

	/** Sets the sign text. Returns true if sign text changed. */
	bool SetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);

	/** Marks the chunk as being regenerated - all its clients want that chunk again (used by cWorld::RegenerateChunk()) */
	void MarkChunkRegenerating(int a_ChunkX, int a_ChunkZ);

	bool IsChunkLighted(int a_ChunkX, int a_ChunkZ);

	/** Calls the callback for each chunk in the coords specified (all cords are inclusive). Returns true if all chunks have been processed successfully */
	bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback);

	/** Calls the callback for each loaded chunk. Returns true if all chunks have been processed successfully */
	bool ForEachLoadedChunk(cFunctionRef<bool(int, int)> a_Callback) const;

	/** Writes the block area into the specified coords. Returns true if all chunks have been processed. Prefer cBlockArea::Write() instead. */
	bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes);

	/** Returns the number of valid chunks and the number of dirty chunks */
	void GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty) const;

	/** Grows the plant at the specified position by at most a_NumStages.
	The block's Grow handler is invoked.
	Returns the number of stages the plant has grown, 0 if not a plant. */
	int GrowPlantAt(Vector3i a_BlockPos, int a_NumStages = 1);

	/** Causes the specified block to be ticked on the next Tick() call.
	Plugins can use this via the cWorld:SetNextBlockToTick() API.
	Only one block coord per chunk may be set, a second call overwrites the first call */
	void SetNextBlockToTick(const Vector3i a_BlockPos);

	/** Make a Mob census, of all mobs, their family, their chunk and their distance to closest player */
	void CollectMobCensus(cMobCensus & a_ToFill);

	/** Try to Spawn Monsters inside all Chunks */
	void SpawnMobs(cMobSpawner & a_MobSpawner);

	void Tick(std::chrono::milliseconds a_Dt);

	/** Ticks a single block. Used by cWorld::TickQueuedBlocks() to tick the queued blocks */
	void TickBlock(const Vector3i a_BlockPos);

	void UnloadUnusedChunks(void);
	void SaveAllChunks(void) const;

	cWorld * GetWorld(void) const { return m_World; }

	size_t GetNumChunks(void) const;

	/** Returns the number of unused dirty chunks. Those are chunks that we can save and then unload */
	size_t GetNumUnusedDirtyChunks(void) const;

	void ChunkValidated(void);  // Called by chunks that have become valid

	/** Returns the CS for locking the chunkmap; only cWorld::cLock may use this function! */
	cCriticalSection & GetCS(void) { return m_CSChunks; }

	/** Increments (a_AlwaysTicked == true) or decrements (false) the m_AlwaysTicked counter for the specified chunk.
	If the m_AlwaysTicked counter is greater than zero, the chunk is ticked in the tick-thread regardless of
	whether it has any clients or not.
	This function allows nesting and task-concurrency (multiple separate tasks can request ticking and as long
	as at least one requests is active the chunk will be ticked). */
	void SetChunkAlwaysTicked(int a_ChunkX, int a_ChunkZ, bool a_AlwaysTicked);

	/** Adds this chunkmap's CS to the DeadlockDetect's tracked CSs. */
	void TrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect, const AString & a_WorldName);

	/** Removes this chunkmap's CS from the DeadlockDetect's tracked CSs. */
	void UntrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect);

private:

	// Chunks query their neighbors using FindChunk(), while being ticked
	friend class cChunk;

	// The chunkstay can (de-)register itself using AddChunkStay() and DelChunkStay()
	friend class cChunkStay;


	class cStarvationCallbacks
		: public cAllocationPool<cChunkData::sChunkSection>::cStarvationCallbacks
	{
		virtual void OnStartUsingReserve() override
		{
			LOG("Using backup memory buffer");
		}
		virtual void OnEndUsingReserve() override
		{
			LOG("Stoped using backup memory buffer");
		}
		virtual void OnOutOfReserve() override
		{
			LOG("Out of Memory");
		}
	};

	struct ChunkCoordinate
	{
		struct Comparer
		{
			bool operator() (const ChunkCoordinate & a_Lhs, const ChunkCoordinate & a_Rhs) const
			{
				return ((a_Lhs.ChunkX == a_Rhs.ChunkX) ? (a_Lhs.ChunkZ < a_Rhs.ChunkZ) : (a_Lhs.ChunkX < a_Rhs.ChunkX));
			}
		};

		int ChunkX;
		int ChunkZ;
	};

	typedef std::list<cChunkStay *> cChunkStays;

	mutable cCriticalSection m_CSChunks;

	/** A map of chunk coordinates to chunks.
	Uses a map (as opposed to unordered_map) because sorted maps are apparently faster. */
	std::map<ChunkCoordinate, cChunk, ChunkCoordinate::Comparer> m_Chunks;

	cEvent m_evtChunkValid;  // Set whenever any chunk becomes valid, via ChunkValidated()

	cWorld * m_World;

	/** The cChunkStay descendants that are currently enabled in this chunkmap */
	cChunkStays m_ChunkStays;

	std::unique_ptr<cAllocationPool<cChunkData::sChunkSection> > m_Pool;

	/** Returns or creates and returns a chunk pointer corresponding to the given chunk coordinates.
	Emplaces this chunk in the chunk map. */
	cChunk & ConstructChunk(int a_ChunkX, int a_ChunkZ);

	/** Constructs a chunk and queues it for loading / generating if not valid, returning it */
	cChunk & GetChunk(int a_ChunkX, int a_ChunkZ);

	/** Locates a chunk ptr in the chunkmap; doesn't create it when not found; assumes m_CSChunks is locked. To be called only from cChunkMap. */
	cChunk * FindChunk(int a_ChunkX, int a_ChunkZ);

	/** Adds a new cChunkStay descendant to the internal list of ChunkStays; loads its chunks.
	To be used only by cChunkStay; others should use cChunkStay::Enable() instead */
	void AddChunkStay(cChunkStay & a_ChunkStay);

	/** Removes the specified cChunkStay descendant from the internal list of ChunkStays.
	To be used only by cChunkStay; others should use cChunkStay::Disable() instead */
	void DelChunkStay(cChunkStay & a_ChunkStay);

};
