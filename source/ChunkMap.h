
// cChunkMap.h

// Interfaces to the cChunkMap class representing the chunk storage for a single world

#pragma once

#include "ChunkDef.h"





class cWorld;
class cItem;
class MTRand;
class cChunkStay;
class cChunk;
class cPlayer;
class cChestEntity;
class cDispenserEntity;
class cDropperEntity;
class cDropSpenserEntity;
class cFurnaceEntity;
class cPawn;
class cPickup;
class cChunkDataSerializer;
class cBlockArea;

typedef std::list<cClientHandle *>  cClientHandleList;
typedef cChunk * cChunkPtr;
typedef cItemCallback<cEntity>            cEntityCallback;
typedef cItemCallback<cChestEntity>       cChestCallback;
typedef cItemCallback<cDispenserEntity>   cDispenserCallback;
typedef cItemCallback<cDropperEntity>     cDropperCallback;
typedef cItemCallback<cDropSpenserEntity> cDropSpenserCallback;
typedef cItemCallback<cFurnaceEntity>     cFurnaceCallback;
typedef cItemCallback<cChunk>             cChunkCallback;





class cChunkMap
{
public:

	static const int LAYER_SIZE = 32;

	cChunkMap(cWorld* a_World );
	~cChunkMap();

	// Broadcast respective packets to all clients of the chunk where the event is taking place
	// (Please keep these alpha-sorted)
	void BroadcastAttachEntity(const cEntity & a_Entity, const cEntity * a_Vehicle);
	void BroadcastBlockAction(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType, const cClientHandle * a_Exclude = NULL);
	void BroadcastBlockBreakAnimation(int a_entityID, int a_blockX, int a_blockY, int a_blockZ, char a_stage, const cClientHandle * a_Exclude = NULL);
	void BroadcastBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude);
	void BroadcastChunkData(int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer, const cClientHandle * a_Exclude = NULL);
	void BroadcastCollectPickup(const cPickup & a_Pickup, const cPlayer & a_Player, const cClientHandle * a_Exclude = NULL);
	void BroadcastDestroyEntity(const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityEquipment(const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityHeadLook(const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityLook(const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityMetadata(const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityRelMove(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityRelMoveLook(const cEntity & a_Entity, char a_RelX, char a_RelY, char a_RelZ, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityStatus(const cEntity & a_Entity, char a_Status, const cClientHandle * a_Exclude = NULL);
	void BroadcastEntityVelocity(const cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastPlayerAnimation(const cPlayer & a_Player, char a_Animation, const cClientHandle * a_Exclude = NULL);
	void BroadcastSoundEffect(const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch, const cClientHandle * a_Exclude = NULL);   // a_Src coords are Block * 8
	void BroadcastSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data, const cClientHandle * a_Exclude = NULL);
	void BroadcastSpawnEntity(cEntity & a_Entity, const cClientHandle * a_Exclude = NULL);
	void BroadcastThunderbolt(int a_BlockX, int a_BlockY, int a_BlockZ, const cClientHandle * a_Exclude = NULL);
	void BroadcastUseBed(const cEntity & a_Entity, int a_BlockX, int a_BlockY, int a_BlockZ );
	
	/// Sends the block entity, if it is at the coords specified, to a_Client
	void SendBlockEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cClientHandle & a_Client);
	
	/// a_Player rclked block entity at the coords specified, handle it
	void UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z);
	
	/// Calls the callback for the chunk specified, with ChunkMapCS locked; returns false if the chunk doesn't exist, otherwise returns the same value as the callback
	bool DoWithChunk(int a_ChunkX, int a_ChunkZ, cChunkCallback & a_Callback);

	/// Wakes up simulators for the specified block
	void WakeUpSimulators(int a_BlockX, int a_BlockY, int a_BlockZ);

	/// Wakes up the simulators for the specified area of blocks
	void WakeUpSimulatorsInArea(int a_MinBlockX, int a_MaxBlockX, int a_MinBlockY, int a_MaxBlockY, int a_MinBlockZ, int a_MaxBlockZ);

	void MarkChunkDirty     (int a_ChunkX, int a_ChunkZ);
	void MarkChunkSaving    (int a_ChunkX, int a_ChunkZ);
	void MarkChunkSaved     (int a_ChunkX, int a_ChunkZ);
	
	/** Sets the chunk data as either loaded from the storage or generated.
	a_BlockLight and a_BlockSkyLight are optional, if not present, chunk will be marked as unlighted.
	a_BiomeMap is optional, if not present, biomes will be calculated by the generator
	a_HeightMap is optional, if not present, will be calculated.
	If a_MarkDirty is set, the chunk is set as dirty (used after generating)
	*/
	void SetChunkData(
		int a_ChunkX, int a_ChunkZ, 
		const BLOCKTYPE * a_BlockTypes,
		const NIBBLETYPE * a_BlockMeta,
		const NIBBLETYPE * a_BlockLight,
		const NIBBLETYPE * a_BlockSkyLight,
		const cChunkDef::HeightMap * a_HeightMap,
		const cChunkDef::BiomeMap &  a_BiomeMap,
		cBlockEntityList & a_BlockEntities,
		bool a_MarkDirty
	);
	
	void ChunkLighted(
		int a_ChunkX, int a_ChunkZ,
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_SkyLight
	);
	
	bool GetChunkData       (int a_ChunkX, int a_ChunkZ, cChunkDataCallback & a_Callback);
	
	/// Copies the chunk's blocktypes into a_Blocks; returns true if successful
	bool GetChunkBlockTypes (int a_ChunkX, int a_ChunkZ, BLOCKTYPE * a_Blocks);
	
	bool      IsChunkValid       (int a_ChunkX, int a_ChunkZ);
	bool      HasChunkAnyClients (int a_ChunkX, int a_ChunkZ);
	int       GetHeight          (int a_BlockX, int a_BlockZ);  // Waits for the chunk to get loaded / generated
	bool      TryGetHeight       (int a_BlockX, int a_BlockZ, int & a_Height);  // Returns false if chunk not loaded / generated
	void      FastSetBlocks      (sSetBlockList & a_BlockList);
	void      CollectPickupsByPlayer(cPlayer * a_Player);
	
	BLOCKTYPE  GetBlock          (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetBlockMeta      (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetBlockSkyLight  (int a_BlockX, int a_BlockY, int a_BlockZ);
	NIBBLETYPE GetBlockBlockLight(int a_BlockX, int a_BlockY, int a_BlockZ);
	void       SetBlockMeta      (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockMeta);
	void       SetBlock          (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta);
	bool       GetBlockTypeMeta  (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);
	bool       GetBlockInfo      (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_Meta, NIBBLETYPE & a_SkyLight, NIBBLETYPE & a_BlockLight);

	/// Replaces world blocks with a_Blocks, if they are of type a_FilterBlockType
	void      ReplaceBlocks(const sSetBlockVector & a_Blocks, BLOCKTYPE a_FilterBlockType);
	
	/// Special function used for growing trees, replaces only blocks that tree may overwrite
	void      ReplaceTreeBlocks(const sSetBlockVector & a_Blocks);
	
	EMCSBiome GetBiomeAt (int a_BlockX, int a_BlockZ);
	
	/// Retrieves block types of the specified blocks. If a chunk is not loaded, doesn't modify the block. Returns true if all blocks were read.
	bool GetBlocks(sSetBlockVector & a_Blocks, bool a_ContinueOnFailure);

	bool DigBlock   (int a_X, int a_Y, int a_Z);
	void SendBlockTo(int a_X, int a_Y, int a_Z, cPlayer * a_Player);
	
	/// Compares clients of two chunks, calls the callback accordingly
	void CompareChunkClients(int a_ChunkX1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkZ2, cClientDiffCallback & a_Callback);
	
	/// Compares clients of two chunks, calls the callback accordingly
	void CompareChunkClients(cChunk * a_Chunk1, cChunk * a_Chunk2, cClientDiffCallback & a_Callback);

	/// Adds client to a chunk, if not already present; returns true if added, false if present
	bool AddChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);

	/// Removes the client from the chunk
	void RemoveChunkClient(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes the client from all chunks it is present in
	void RemoveClientFromChunks(cClientHandle * a_Client);

	/// Adds the entity to its appropriate chunk, takes ownership of the entity pointer
	void AddEntity(cEntity * a_Entity);
	
	/// Returns true if the entity with specified ID is present in the chunks
	bool HasEntity(int a_EntityID);
	
	/// Removes the entity from its appropriate chunk
	void RemoveEntity(cEntity * a_Entity);
	
	/// Calls the callback for each entity in the entire world; returns true if all entities processed, false if the callback aborted by returning true
	bool ForEachEntity(cEntityCallback & a_Callback);  // Lua-accessible

	/// Calls the callback for each entity in the specified chunk; returns true if all entities processed, false if the callback aborted by returning true
	bool ForEachEntityInChunk(int a_ChunkX, int a_ChunkZ, cEntityCallback & a_Callback);  // Lua-accessible

	/// Destroys and returns a list of blocks destroyed in the explosion at the specified coordinates
	void DoExplosiontAt(float a_ExplosionSize, int a_BlockX, int a_BlockY, int a_BlockZ, cVector3iArray & a_BlockAffected);
	
	/// Calls the callback if the entity with the specified ID is found, with the entity object as the callback param. Returns true if entity found and callback returned false.
	bool DoWithEntityByID(int a_UniqueID, cEntityCallback & a_Callback);  // Lua-accessible

	/// Calls the callback for each chest in the specified chunk; returns true if all chests processed, false if the callback aborted by returning true
	bool ForEachChestInChunk  (int a_ChunkX, int a_ChunkZ, cChestCallback &   a_Callback);  // Lua-accessible

	/// Calls the callback for each dispenser in the specified chunk; returns true if all dispensers processed, false if the callback aborted by returning true
	bool ForEachDispenserInChunk(int a_ChunkX, int a_ChunkZ, cDispenserCallback & a_Callback);

	/// Calls the callback for each dropper in the specified chunk; returns true if all droppers processed, false if the callback aborted by returning true
	bool ForEachDropperInChunk(int a_ChunkX, int a_ChunkZ, cDropperCallback & a_Callback);

	/// Calls the callback for each dropspenser in the specified chunk; returns true if all dropspensers processed, false if the callback aborted by returning true
	bool ForEachDropSpenserInChunk(int a_ChunkX, int a_ChunkZ, cDropSpenserCallback & a_Callback);

	/// Calls the callback for each furnace in the specified chunk; returns true if all furnaces processed, false if the callback aborted by returning true
	bool ForEachFurnaceInChunk(int a_ChunkX, int a_ChunkZ, cFurnaceCallback & a_Callback);  // Lua-accessible
	
	/// Calls the callback for the chest at the specified coords; returns false if there's no chest at those coords, true if found
	bool DoWithChestAt  (int a_BlockX, int a_BlockY, int a_BlockZ, cChestCallback &   a_Callback);  // Lua-acessible

	/// Calls the callback for the dispenser at the specified coords; returns false if there's no dispenser at those coords or callback returns true, returns true if found
	bool DoWithDispenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDispenserCallback & a_Callback);  // Lua-accessible

	/// Calls the callback for the dropper at the specified coords; returns false if there's no dropper at those coords or callback returns true, returns true if found
	bool DoWithDropperAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropperCallback & a_Callback);  // Lua-accessible

	/// Calls the callback for the dropspenser at the specified coords; returns false if there's no dropspenser at those coords or callback returns true, returns true if found
	bool DoWithDropSpenserAt(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserCallback & a_Callback);  // Lua-accessible

	/// Calls the callback for the furnace at the specified coords; returns false if there's no furnace at those coords or callback returns true, returns true if found
	bool DoWithFurnaceAt(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceCallback & a_Callback);  // Lua-accessible

	/// Retrieves the test on the sign at the specified coords; returns false if there's no sign at those coords, true if found
	bool GetSignLines (int a_BlockX, int a_BlockY, int a_BlockZ, AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4);  // Lua-accessible

	/// Touches the chunk, causing it to be loaded or generated
	void TouchChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunk, if not already loaded. Doesn't generate. Returns true if chunk valid (even if already loaded before)
	bool LoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunks specified. Doesn't report failure, other than chunks being !IsValid()
	void LoadChunks(const cChunkCoordsList & a_Chunks);

	/// Marks the chunk as failed-to-load
	void ChunkLoadFailed(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Sets the sign text. Returns true if sign text changed.
	bool SetSignLines(int a_BlockX, int a_BlockY, int a_BlockZ, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);
	
	/// Marks (a_Stay == true) or unmarks (a_Stay == false) chunks as non-unloadable; to be used only by cChunkStay!
	void ChunksStay(const cChunkCoordsList & a_Chunks, bool a_Stay = true);
	
	/// Marks the chunk as being regenerated - all its clients want that chunk again (used by cWorld::RegenerateChunk() )
	void MarkChunkRegenerating(int a_ChunkX, int a_ChunkZ);
	
	bool IsChunkLighted(int a_ChunkX, int a_ChunkZ);
	
	/// Calls the callback for each chunk in the coords specified (all cords are inclusive). Returns true if all chunks have been processed successfully
	bool ForEachChunkInRect(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ, cChunkDataCallback & a_Callback);
	
	/// Writes the block area into the specified coords. Returns true if all chunks have been processed. Prefer cBlockArea::Write() instead.
	bool WriteBlockArea(cBlockArea & a_Area, int a_MinBlockX, int a_MinBlockY, int a_MinBlockZ, int a_DataTypes);

	/// Returns the number of valid chunks and the number of dirty chunks
	void GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty);
	
	/// Grows a melon or a pumpkin next to the block specified (assumed to be the stem)
	void GrowMelonPumpkin(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, MTRand & a_Rand);
	
	/// Grows a sugarcane present at the block specified by the amount of blocks specified, up to the max height specified in the config
	void GrowSugarcane(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow);
	
	/// Grows a cactus present at the block specified by the amount of blocks specified, up to the max height specified in the config
	void GrowCactus(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow);
	
		/// Sets the blockticking to start at the specified block. Only one blocktick per chunk may be set, second call overwrites the first call
	void SetNextBlockTick(int a_BlockX, int a_BlockY, int a_BlockZ);

	void Tick(float a_Dt);

	void UnloadUnusedChunks(void);
	void SaveAllChunks(void);

	cWorld * GetWorld(void) { return m_World; }

	int GetNumChunks(void);
	
	void ChunkValidated(void);  // Called by chunks that have become valid
	
	/// Queues the specified block for ticking (block update)
	void QueueTickBlock(int a_BlockX, int a_BlockY, int a_BlockZ);
	
	/// Returns the CS for locking the chunkmap; only cWorld::cLock may use this function!
	cCriticalSection & GetCS(void) { return m_CSLayers; }

private:

	friend class cChunk;  // The chunks can manipulate neighbors while in their Tick() method, using LockedGetBlock() and LockedSetBlock()

	class cChunkLayer
	{
	public:
		cChunkLayer(int a_LayerX, int a_LayerZ, cChunkMap * a_Parent);
		~cChunkLayer();

		/// Always returns an assigned chunkptr, but the chunk needn't be valid (loaded / generated) - callers must check
		cChunkPtr GetChunk( int a_ChunkX, int a_ChunkY, int a_ChunkZ );
		
		/// Returns the specified chunk, or NULL if not created yet
		cChunk * FindChunk(int a_ChunkX, int a_ChunkZ);
		
		int GetX(void) const {return m_LayerX; }
		int GetZ(void) const {return m_LayerZ; }
		
		int GetNumChunksLoaded(void) const ;
		
		void GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty) const;
		
		void Save(void);
		void UnloadUnusedChunks(void);
		
		void Tick(float a_Dt);
		
		void RemoveClient(cClientHandle * a_Client);
		
		/// Calls the callback for each entity in the entire world; returns true if all entities processed, false if the callback aborted by returning true
		bool ForEachEntity(cEntityCallback & a_Callback);  // Lua-accessible

		/// Calls the callback if the entity with the specified ID is found, with the entity object as the callback param. Returns true if entity found.
		bool DoWithEntityByID(int a_EntityID, cEntityCallback & a_Callback, bool & a_CallbackReturn);  // Lua-accessible

		/// Returns true if there is an entity with the specified ID within this layer's chunks
		bool HasEntity(int a_EntityID);
		
	protected:
	
		cChunkPtr m_Chunks[LAYER_SIZE * LAYER_SIZE];
		int m_LayerX;
		int m_LayerZ;
		cChunkMap * m_Parent;
		int m_NumChunksLoaded;
	};
	
	typedef std::list<cChunkLayer *> cChunkLayerList;

	/// Finds the cChunkLayer object responsible for the specified chunk; returns NULL if not found. Assumes m_CSLayers is locked.
	cChunkLayer * FindLayerForChunk(int a_ChunkX, int a_ChunkZ);
	
	/// Returns the specified cChunkLayer object; returns NULL if not found. Assumes m_CSLayers is locked.
	cChunkLayer * FindLayer(int a_LayerX, int a_LayerZ);
	
	/// Returns the cChunkLayer object responsible for the specified chunk; creates it if not found.
	cChunkLayer * GetLayerForChunk (int a_ChunkX, int a_ChunkZ);
	
	/// Returns the specified cChunkLayer object; creates it if not found.
	cChunkLayer * GetLayer(int a_LayerX, int a_LayerZ);
	
	void RemoveLayer(cChunkLayer * a_Layer);

	cCriticalSection m_CSLayers;
	cChunkLayerList  m_Layers;
	cEvent           m_evtChunkValid;  // Set whenever any chunk becomes valid, via ChunkValidated()

	cWorld * m_World;

	cChunkPtr GetChunk      (int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Also queues the chunk for loading / generating if not valid
	cChunkPtr GetChunkNoGen (int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Also queues the chunk for loading if not valid; doesn't generate
	cChunkPtr GetChunkNoLoad(int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Doesn't load, doesn't generate
	
	/// Gets a block in any chunk while in the cChunk's Tick() method; returns true if successful, false if chunk not loaded (doesn't queue load)
	bool LockedGetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);
	
	/// Gets a block type in any chunk while in the cChunk's Tick() method; returns true if successful, false if chunk not loaded (doesn't queue load)
	bool LockedGetBlockType(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType);
	
	/// Gets a block meta in any chunk while in the cChunk's Tick() method; returns true if successful, false if chunk not loaded (doesn't queue load)
	bool LockedGetBlockMeta(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE & a_BlockMeta);
	
	/// Sets a block in any chunk while in the cChunk's Tick() method; returns true if successful, false if chunk not loaded (doesn't queue load)
	bool LockedSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/// Fast-sets a block in any chunk while in the cChunk's Tick() method; returns true if successful, false if chunk not loaded (doesn't queue load)
	bool LockedFastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);
	
	/// Locates a chunk ptr in the chunkmap; doesn't create it when not found; assumes m_CSLayers is locked. To be called only from cChunkMap.
	cChunk * FindChunk(int a_ChunkX, int a_ChunkZ);
};





/** Makes chunks stay loaded until this object is cleared or destroyed
Works by setting internal flags in the cChunk that it should not be unloaded.
To optimize for speed, cChunkStay has an Enabled flag, it will "stay" the chunks only when enabled and it will refuse manipulations when enabled
The object itself is not made thread-safe, it's supposed to be used from a single thread only.
*/
class cChunkStay
{
public:
	cChunkStay(cWorld * a_World);
	~cChunkStay();
	
	void Clear(void);
	
	void Add(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void Remove(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	void Enable(void);
	void Disable(void);
	
	/// Queues each chunk in m_Chunks[] for loading / generating
	void Load(void);
	
	// Allow cChunkStay be passed to functions expecting a const cChunkCoordsList &
	operator const cChunkCoordsList(void) const {return m_Chunks; }
	
protected:

	cWorld * m_World;
	
	bool m_IsEnabled;
	
	cChunkCoordsList m_Chunks;
} ;




