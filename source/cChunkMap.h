
// cChunkMap.h

// Interfaces to the cChunkMap class representing the chunk storage for a single world

#pragma once

#include "ChunkDef.h"





class cWorld;
class cItem;
class MTRand;
class cChunkStay;
class cChunk;
class cPacket;
class cPlayer;

typedef std::list<cClientHandle *>  cClientHandleList;
typedef cChunk * cChunkPtr;





class cChunkMap
{
public:

	static const int LAYER_SIZE = 32;

	cChunkMap(cWorld* a_World );
	~cChunkMap();

	// Direct action methods:
	/// Broadcast a_Packet to all clients in the chunk specified
	void BroadcastToChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const cPacket & a_Packet, cClientHandle * a_Exclude = NULL);
	
	/// Broadcasts a_Packet to all clients in the chunk where block [x, y, z] is, except to client a_Exclude
	void BroadcastToChunkOfBlock(int a_X, int a_Y, int a_Z, const cPacket * a_Packet, cClientHandle * a_Exclude = NULL);
	
	/// a_Player rclked block entity at the coords specified, handle it
	void UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z);

	void MarkChunkDirty     (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void MarkChunkSaving    (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void MarkChunkSaved     (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/** Sets the chunk data as either loaded from the storage or generated.
	a_BlockLight and a_BlockSkyLight are optional, if not present, chunk will be marked as unlighted.
	a_BiomeMap is optional, if not present, biomes will be calculated by the generator
	a_HeightMap is optional, if not present, will be calculated.
	If a_MarkDirty is set, the chunk is set as dirty (used after generating)
	*/
	void SetChunkData(
		int a_ChunkX, int a_ChunkY, int a_ChunkZ, 
		const BLOCKTYPE * a_BlockTypes,
		const NIBBLETYPE * a_BlockMeta,
		const NIBBLETYPE * a_BlockLight,
		const NIBBLETYPE * a_BlockSkyLight,
		const cChunkDef::HeightMap * a_HeightMap,
		const cChunkDef::BiomeMap &  a_BiomeMap,
		cEntityList & a_Entities,
		cBlockEntityList & a_BlockEntities,
		bool a_MarkDirty
	);
	
	void ChunkLighted(
		int a_ChunkX, int a_ChunkZ,
		const cChunkDef::BlockNibbles & a_BlockLight,
		const cChunkDef::BlockNibbles & a_SkyLight
	);
	
	bool GetChunkData       (int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback & a_Callback);
	
	/// Gets the chunk's blocks, only the block types
	bool GetChunkBlockTypes (int a_ChunkX, int a_ChunkY, int a_ChunkZ, BLOCKTYPE * a_Blocks);
	
	/// Gets the chunk's block data, the entire 4 arrays (Types, Meta, Light, SkyLight)
	bool GetChunkBlockData  (int a_ChunkX, int a_ChunkY, int a_ChunkZ, BLOCKTYPE * a_BlockData);
	
	bool      IsChunkValid       (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	bool      HasChunkAnyClients (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	int       GetHeight          (int a_BlockX, int a_BlockZ);
	void      FastSetBlocks      (sSetBlockList & a_BlockList);
	void      CollectPickupsByPlayer(cPlayer * a_Player);
	BLOCKTYPE GetBlock           (int a_X, int a_Y, int a_Z);
	BLOCKTYPE GetBlockMeta       (int a_X, int a_Y, int a_Z);
	BLOCKTYPE GetBlockSkyLight   (int a_X, int a_Y, int a_Z);
	void      SetBlockMeta       (int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockMeta);
	void      SetBlock           (int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta);
	void      GetBlockTypeMeta(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);

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
	void CompareChunkClients(int a_ChunkX1, int a_ChunkY1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkY2, int a_ChunkZ2, cClientDiffCallback & a_Callback);
	
	/// Adds client to a chunk, if not already present; returns true if added, false if present
	bool AddChunkClient(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client);

	/// Removes the client from the chunk
	void RemoveChunkClient(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes the client from all chunks it is present in
	void RemoveClientFromChunks(cClientHandle * a_Client);

	/// Moves the entity from its current chunk to the new chunk specified
	void MoveEntityToChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ);

	/// Removes the entity from the chunk specified
	void RemoveEntityFromChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Touches the chunk, causing it to be loaded or generated
	void TouchChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunk, if not already loaded. Doesn't generate. Returns true if chunk valid (even if already loaded before)
	bool LoadChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Loads the chunks specified. Doesn't report failure, other than chunks being !IsValid()
	void LoadChunks(const cChunkCoordsList & a_Chunks);

	/// Marks the chunk as failed-to-load:
	void ChunkLoadFailed(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	void UpdateSign(int a_X, int a_Y, int a_Z, const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4);
	
	/// Marks (a_Stay == true) or unmarks (a_Stay == false) chunks as non-unloadable; to be used only by cChunkStay!
	void ChunksStay(const cChunkCoordsList & a_Chunks, bool a_Stay = true);
	
	/// Marks the chunk as being regenerated - all its clients want that chunk again (used by cWorld::RegenerateChunk() )
	void MarkChunkRegenerating(int a_ChunkX, int a_ChunkZ);
	
	bool IsChunkLighted(int a_ChunkX, int a_ChunkZ);
	
	/// Returns the number of valid chunks and the number of dirty chunks
	void GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty);
	
	/// Grows a melon or a pumpkin next to the block specified (assumed to be the stem)
	void GrowMelonPumpkin(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, MTRand & a_Rand);
	
	/// Grows a sugarcane present at the block specified by the amount of blocks specified, up to the max height of 3
	void GrowSugarcane(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow);
	
	/// Grows a cactus present at the block specified by the amount of blocks specified, up to the max height of 3
	void GrowCactus(int a_BlockX, int a_BlockY, int a_BlockZ, int a_NumBlocksToGrow);
	
		/// Sets the blockticking to start at the specified block. Only one blocktick per chunk may be set, second call overwrites the first call
	void SetNextBlockTick(int a_BlockX, int a_BlockY, int a_BlockZ);

	void Tick( float a_Dt, MTRand & a_TickRand );

	void UnloadUnusedChunks();
	void SaveAllChunks();

	cWorld * GetWorld() { return m_World; }

	int GetNumChunks(void);
	
	void ChunkValidated(void);  // Called by chunks that have become valid

private:

	friend class cChunk;  // The chunks can manipulate neighbors while in their Tick() method, using LockedGetBlock() and LockedSetBlock()

	class cChunkLayer
	{
	public:
		cChunkLayer(int a_LayerX, int a_LayerZ, cChunkMap * a_Parent);
		~cChunkLayer();

		/// Always returns an assigned chunkptr, but the chunk needn't be valid (loaded / generated) - callers must check
		cChunkPtr GetChunk( int a_ChunkX, int a_ChunkY, int a_ChunkZ );
		
		int GetX(void) const {return m_LayerX; }
		int GetZ(void) const {return m_LayerZ; }
		
		int GetNumChunksLoaded(void) const ;
		
		void GetChunkStats(int & a_NumChunksValid, int & a_NumChunksDirty) const;
		
		void Save(void);
		void UnloadUnusedChunks(void);
		
		void Tick( float a_Dt, MTRand & a_TickRand );
		
		void RemoveClient(cClientHandle * a_Client);
		
	protected:
	
		cChunkPtr m_Chunks[LAYER_SIZE * LAYER_SIZE];
		int m_LayerX;
		int m_LayerZ;
		cChunkMap * m_Parent;
		int m_NumChunksLoaded;
	};
	
	typedef std::list<cChunkLayer *> cChunkLayerList;
	// TODO: Use smart pointers for cChunkLayerList as well, so that ticking and saving needn't lock the entire layerlist
	// This however means that cChunkLayer needs to interlock its m_Chunks[]

	cChunkLayer * GetLayerForChunk( int a_ChunkX, int a_ChunkZ );  // Creates the layer if it doesn't already exist
	cChunkLayer * GetLayer( int a_LayerX, int a_LayerZ );  // Creates the layer if it doesn't already exist
	void RemoveLayer( cChunkLayer* a_Layer );

	cCriticalSection m_CSLayers;
	cChunkLayerList  m_Layers;
	cEvent           m_evtChunkValid;  // Set whenever any chunk becomes valid, via ChunkValidated()

	cWorld * m_World;

	cChunkPtr GetChunk      (int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Also queues the chunk for loading / generating if not valid
	cChunkPtr GetChunkNoGen (int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Also queues the chunk for loading if not valid; doesn't generate
	cChunkPtr GetChunkNoLoad(int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Doesn't load, doesn't generate
	
	/// Gets a block in any chunk while in the cChunk's Tick() method; returns true if successful, false if chunk not loaded (doesn't queue load)
	bool LockedGetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta);
	
	/// Sets a block in any chunk while in the cChunk's Tick() method; returns true if successful, false if chunk not loaded (doesn't queue load)
	bool LockedSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE   a_BlockType, NIBBLETYPE   a_BlockMeta);

	/// Fast-sets a block in any chunk while in the cChunk's Tick() method; returns true if successful, false if chunk not loaded (doesn't queue load)
	bool LockedFastSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE   a_BlockType, NIBBLETYPE   a_BlockMeta);
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




