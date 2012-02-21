
// cChunkMap.h

// Interfaces to the cChunkMap class representing the chunk storage for a single world

#pragma once

#include "cChunk.h"





class cWorld;
class cEntity;
class MTRand;




class cChunkMap
{
public:

	static const int LAYER_SIZE = 32;

	cChunkMap(cWorld* a_World );
	~cChunkMap();

	// TODO: Get rid of these (put into Private section) in favor of the direct action methods:
	cChunkPtr GetChunk     ( int a_ChunkX, int a_ChunkY, int a_ChunkZ );  // Also queues the chunk for loading / generating if not valid
	cChunkPtr GetChunkNoGen( int a_ChunkX, int a_ChunkY, int a_ChunkZ );  // Also queues the chunk for loading if not valid; doesn't generate
	
	// Direct action methods:
	/// Broadcast a_Packet to all clients in the chunk specified
	void BroadcastToChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cPacket & a_Packet, cClientHandle * a_Exclude = NULL);
	
	/// Broadcasts a_Packet to all clients in the chunk where block [x, y, z] is, except to client a_Exclude
	void BroadcastToChunkOfBlock(int a_X, int a_Y, int a_Z, cPacket * a_Packet, cClientHandle * a_Exclude = NULL);
	
	/// a_Player rclked block entity at the coords specified, handle it
	void UseBlockEntity(cPlayer * a_Player, int a_X, int a_Y, int a_Z);

	void MarkChunkDirty     (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void MarkChunkSaving    (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void MarkChunkSaved     (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void ChunkDataLoaded    (int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);
	void ChunkDataGenerated (int a_ChunkX, int a_ChunkY, int a_ChunkZ, const char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);
	void GetChunkData       (int a_ChunkX, int a_ChunkY, int a_ChunkZ, cChunkDataCallback * a_Callback);
	bool GetChunkBlocks     (int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_Blocks);
	bool IsChunkValid       (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	bool HasChunkAnyClients (int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	void SpreadChunkLighting(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	int  GetHeight          (int a_BlockX, int a_BlockZ);
	void FastSetBlocks      (sSetBlockList & a_BlockList);
	void CollectPickupsByPlayer(cPlayer * a_Player);
	char GetBlock           (int a_X, int a_Y, int a_Z);
	char GetBlockMeta       (int a_X, int a_Y, int a_Z);
	
	/// Compares clients of two chunks, calls the callback accordingly
	void CompareChunkClients(int a_ChunkX1, int a_ChunkY1, int a_ChunkZ1, int a_ChunkX2, int a_ChunkY2, int a_ChunkZ2, cClientDiffCallback & a_Callback);

	/// Moves the entity from its current chunk to the new chunk specified
	void MoveEntityToChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ);

	/// Removes the entity from the chunk specified
	void RemoveEntityFromChunk(cEntity * a_Entity, int a_ChunkX, int a_ChunkY, int a_ChunkZ);

	void Tick( float a_Dt, MTRand & a_TickRand );

	void UnloadUnusedChunks();
	void SaveAllChunks();

	cWorld * GetWorld() { return m_World; }

	int GetNumChunks(void);
	
	/// Converts absolute block coords into relative (chunk + block) coords:
	inline static void AbsoluteToRelative(/* in-out */ int & a_X, int & a_Y, int & a_Z, /* out */ int & a_ChunkX, int & a_ChunkZ )
	{
		BlockToChunk(a_X, a_Y, a_Z, a_ChunkX, a_ChunkZ);

		a_X = a_X - a_ChunkX * 16;
		a_Z = a_Z - a_ChunkZ * 16;
	}
	
	/// Converts absolute block coords to chunk coords:
	inline static void BlockToChunk( int a_X, int a_Y, int a_Z, int & a_ChunkX, int & a_ChunkZ )
	{
		(void)a_Y;
		a_ChunkX = a_X / 16;
		if ((a_X < 0) && (a_X % 16 != 0))
		{
			a_ChunkX--;
		}
		a_ChunkZ = a_Z / 16;
		if ((a_Z < 0) && (a_Z % 16 != 0))
		{
			a_ChunkZ--;
		}
	}

	void ChunkValidated(void);  // Called by chunks that have become valid

private:

	class cChunkLayer
	{
	public:
		cChunkLayer(int a_LayerX, int a_LayerZ, cChunkMap * a_Parent);

		/// Always returns an assigned chunkptr, but the chunk needn't be valid (loaded / generated) - callers must check
		cChunkPtr GetChunk( int a_ChunkX, int a_ChunkY, int a_ChunkZ );
		
		int GetX(void) const {return m_LayerX; }
		int GetZ(void) const {return m_LayerZ; }
		
		int GetNumChunksLoaded(void) const ;
		
		void Save(void);
		void UnloadUnusedChunks(void);
		
		void Tick( float a_Dt, MTRand & a_TickRand );
		
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
};




