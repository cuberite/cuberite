
#pragma once

#include "cWorldGenerator.h"





class cWorldGenerator_Test :
	public cWorldGenerator
{
public:

	cWorldGenerator_Test(cWorld * a_World) : cWorldGenerator(a_World) {}
	
protected:

	virtual void GenerateTerrain(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData) override;
	virtual void GenerateFoliage(cChunkPtr & a_Chunk ) override;
};




