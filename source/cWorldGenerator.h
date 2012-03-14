
#pragma once





#include "ChunkDef.h"
#include "MersenneTwister.h"





// fwd:
class cWorld;





class cWorldGenerator
{
public:
	cWorldGenerator(cWorld * a_World);
	~cWorldGenerator();

	virtual void GenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities);
	
	virtual void PostGenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Called when the chunk has been already generated and set valid

protected:

	cWorld * m_World;
	
	// Thread-unsafe:
	MTRand r1;
	
	void GenerateOre(char a_OreType, int a_MaxHeight, int a_NumNests, int a_NestSize, BLOCKTYPE * a_BlockData);

	static unsigned int MakeIndex(int x, int y, int z );

	virtual void GenerateTerrain(int a_ChunkX, int a_ChunkY, int a_ChunkZ, BLOCKTYPE * a_BlockData);
	
	virtual void GenerateFoliage(int a_ChunkX, int a_ChunkY, int a_ChunkZ);

	/// Checks if the chunk has all 8 neighbors valid, if so, foliage is generated on that chunk
	void CheckNeighbors(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
};




