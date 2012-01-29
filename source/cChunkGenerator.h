#pragma once

class cChunk;
class cChunkMap;
class cChunkGenerator
{
public:
	cChunkGenerator( cChunkMap* a_pChunkMap );
	~cChunkGenerator();

	void GenerateChunk( int a_X, int a_Z );

private:
	static void GenerateThread( void* a_Params );

	cChunkMap* m_pChunkMap;

	struct sChunkGeneratorState;
	sChunkGeneratorState* m_pState;
};