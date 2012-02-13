
#pragma once





#include "cChunk.h"
#include "MersenneTwister.h"





class cWorldGenerator
{
public:
	cWorldGenerator();
	~cWorldGenerator();

	virtual void GenerateChunk( cChunkPtr a_Chunk );

protected:

	MTRand r1;

	virtual void GenerateTerrain( cChunkPtr a_Chunk );
	virtual void GenerateFoliage( cChunkPtr a_Chunk );

};




