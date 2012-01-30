#pragma once

class cWorld;
class cEntity;
class cChunk;
class cChunkMap
{
public:
	cChunkMap(cWorld* a_World );
	~cChunkMap();

	void AddChunk( cChunk* a_Chunk );
	unsigned int MakeHash( int a_X, int a_Z );

	cChunk* GetChunk( int a_X, int a_Y, int a_Z );
	void RemoveChunk( cChunk* a_Chunk );

	void Tick( float a_Dt );

	void UnloadUnusedChunks();
	bool RemoveEntityFromChunk( cEntity & a_Entity, cChunk* a_CalledFrom = 0 );
	void SaveAllChunks();

	cWorld* GetWorld() { return m_World; }

	int GetNumChunks();
private:
	class cChunkData
	{
	public:
		cChunkData()
			: m_Compressed( 0 )
			, m_LiveChunk( 0 )
			, m_CompressedSize( 0 )
			, m_UncompressedSize( 0 )
		{}
		char* m_Compressed;
		unsigned int m_CompressedSize;
		unsigned int m_UncompressedSize;
		cChunk* m_LiveChunk;
	};

	class cChunkLayer
	{
	public:
		cChunkLayer()
			: m_Chunks( 0 )
			, m_X( 0 )
			, m_Z( 0 )
			, m_NumChunksLoaded( 0 )
		{}
		cChunkLayer( int a_NumChunks )
			: m_Chunks( new cChunkData[a_NumChunks] )
			, m_X( 0 )
			, m_Z( 0 )
			, m_NumChunksLoaded( 0 )
		{}
		cChunkData * GetChunk( int a_X, int a_Z );
		
		cChunkData * m_Chunks;
		int m_X, m_Z;
		int m_NumChunksLoaded;
	};

	void SaveLayer( cChunkLayer* a_Layer );
	cChunkLayer* LoadLayer( int a_LayerX, int a_LayerZ );
	cChunkLayer* GetLayerForChunk( int a_ChunkX, int a_ChunkZ );
	cChunkLayer* GetLayer( int a_LayerX, int a_LayerZ );
	cChunkLayer* AddLayer( const cChunkLayer & a_Layer );
	bool RemoveLayer( cChunkLayer* a_Layer );
	void CompressChunk( cChunkData* a_ChunkData );

	int m_NumLayers;
	cChunkLayer* m_Layers;

	cWorld* m_World;
};
