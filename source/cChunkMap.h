
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

	cChunkPtr GetChunk     ( int a_ChunkX, int a_ChunkY, int a_ChunkZ );  // Also queues the chunk for loading / generating if not valid
	cChunkPtr GetChunkNoGen( int a_ChunkX, int a_ChunkY, int a_ChunkZ );  // Also queues the chunk for loading if not valid; doesn't generate

	void Tick( float a_Dt, MTRand & a_TickRand );

	void UnloadUnusedChunks();
	void SaveAllChunks();

	cWorld* GetWorld() { return m_World; }

	int GetNumChunks(void);
	
private:

	class cChunkLayer
	{
	public:
		cChunkLayer(int a_LayerX, int a_LayerZ, cChunkMap * a_Parent);

		/// Always returns an assigned chunkptr, but the chunk needn't be valid (loaded / generated) - callers must check
		cChunkPtr GetChunk( int a_ChunkX, int a_ChunkZ );
		
		int GetX(void) const {return m_LayerX; }
		int GetZ(void) const {return m_LayerZ; }
		int GetNumChunksLoaded(void) const {return m_NumChunksLoaded; }
		
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

	cWorld * m_World;
};




