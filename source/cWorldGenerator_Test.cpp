
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWorldGenerator_Test.h"
#include "cChunk.h"
#include "BlockID.h"





void cWorldGenerator_Test::GenerateTerrain(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData)
{
	memset(a_BlockData, E_BLOCK_DIRT, cChunk::c_NumBlocks);
	for(int x = 0; x < 16; x++) 
	{
		for(int z = 0; z < 16; z++)
		{
			a_BlockData[MakeIndex(x, 0, z)] = E_BLOCK_BEDROCK;
		}
	}
}





void cWorldGenerator_Test::GenerateFoliage( cChunkPtr & a_Chunk )
{
	(void)a_Chunk;
}




