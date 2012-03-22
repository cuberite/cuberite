
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
	
	void ChunkDataLoaded(
		int a_ChunkX, int a_ChunkY, int a_ChunkZ, 
		const BLOCKTYPE * a_BlockTypes,
		const BLOCKTYPE * a_BlockMeta,
		const BLOCKTYPE * a_BlockLight,
		const BLOCKTYPE * a_BlockSkyLight,
		const cChunkDef::HeightMap * a_HeightMap,
		cEntityList & a_Entities,
		cBlockEntityList & a_BlockEntities
	);
	
	void ChunkDataGenerated (
		int a_ChunkX, int a_ChunkY, int a_ChunkZ, 
		const BLOCKTYPE * a_BlockTypes,
		const BLOCKTYPE * a_BlockMeta,
		const BLOCKTYPE * a_BlockLight,
		const BLOCKTYPE * a_BlockSkyLight,
		const cChunkDef::HeightMap * a_HeightMap,
		cEntityList & a_Entities, 
		cBlockEntityList & a_BlockEntities
	);
	
	bool GetChunkData       (int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback & a_Callback);
	
	/// Gets the chunk's blocks, only the block types
	bool GetChunkBlockTypes (int a_ChunkX, int a_ChunkY, int a_ChunkZ, BLOCKTYPE * a_Blocks);
	
	/// Gets the chunk's block data, the entire 4 arrays (Types, Meta, Light, SkyLight)
	bool GetChunkBlockData  (int a_ChunkX, int a_ChunkY, int a_ChunkZ, BLOCKTYPE * a_BlockData);
	
	bool      IsChunkValid       (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	bool      HasChunkAnyClients (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void      SpreadChunkLighting(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	int       GetHeight          (int a_BlockX, int a_BlockZ);
	void      FastSetBlocks      (sSetBlockList & a_BlockList);
	void      CollectPickupsByPlayer(cPlayer * a_Player);
	BLOCKTYPE GetBlock           (int a_X, int a_Y, int a_Z);
	BLOCKTYPE GetBlockMeta       (int a_X, int a_Y, int a_Z);
	BLOCKTYPE GetBlockSkyLight   (int a_X, int a_Y, int a_Z);
	void      SetBlockMeta       (int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockMeta);
	void      SetBlock           (int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockType, BLOCKTYPE a_BlockMeta);
	bool      DigBlock           (int a_X, int a_Y, int a_Z, cItem & a_PickupItem);
	void      SendBlockTo        (int a_X, int a_Y, int a_Z, cPlayer * a_Player);
	
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

	void Tick( float a_Dt, MTRand & a_TickRand );

	void UnloadUnusedChunks();
	void SaveAllChunks();

	cWorld * GetWorld() { return m_World; }

	int GetNumChunks(void);
	
	void ChunkValidated(void);  // Called by chunks that have become valid

private:

	friend class cChunk;  // Temporary (until we have a separate Lighting thread), so that cChunk's lighting calc can ask for neighbor chunks

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
	
	// Allow cChunkStay be passed to functions expecting a const cChunkCoordsList &
	operator const cChunkCoordsList(void) const {return m_Chunks; }
	
protected:

	cWorld * m_World;
	
	bool m_IsEnabled;
	
	cChunkCoordsList m_Chunks;
} ;




