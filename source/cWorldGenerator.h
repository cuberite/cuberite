
#pragma once





#include "cChunk.h"
#include "MersenneTwister.h"





class cWorldGenerator
{
public:
	cWorldGenerator(cWorld * a_World);
	~cWorldGenerator();

	virtual void GenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);

protected:

	cWorld * m_World;
	
	// Thread-unsafe:
	MTRand r1;

	virtual void GenerateTerrain( cChunkPtr a_Chunk );
	virtual void GenerateFoliage( cChunkPtr a_Chunk );

};




