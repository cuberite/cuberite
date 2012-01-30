#pragma once

class cChunk;
class cChunkLoader
{
public:
	cChunkLoader();
	~cChunkLoader();

	cChunk* LoadChunk( int a_X, int a_Y, int a_Z );
	bool SaveChunk( const cChunk & a_Chunk );

	struct ChunkPack;
private:
	cChunk* LoadFormat1( int a_X, int a_Y, int a_Z );
	ChunkPack* LoadPak1( int PakX, int PakY, int PakZ ); // This loads a .pak file from disk and returns it, nothing more
	
	// Old stuffs
	cChunk* LoadOldFormat( int a_X, int a_Y, int a_Z );
	bool SaveOldFormat( const cChunk & a_Chunk );

	static void SaveThread( void* a_Param );

	bool m_bStop;
	cCriticalSection* m_CriticalSection;
	cEvent* m_Event;

	struct ChunkPacks; // Defined in .cpp
	ChunkPacks* m_ChunkPacks;
};