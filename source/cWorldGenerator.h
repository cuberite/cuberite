
#pragma once





#include "cChunk.h"
#include "MersenneTwister.h"





class cWorldGenerator
{
public:
	cWorldGenerator(cWorld * a_World);
	~cWorldGenerator();

	virtual void GenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);

protected:

	cWorld * m_World;
	
	// Thread-unsafe:
	MTRand r1;

	static unsigned int MakeIndex(int x, int y, int z );

	virtual void GenerateTerrain(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData);
	
	virtual void GenerateFoliage(cChunkPtr & a_Chunk );

};




