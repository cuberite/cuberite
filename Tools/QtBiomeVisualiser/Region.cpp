
#include "Globals.h"
#include "Region.h"





Region::Region()
{
}





Chunk & Region::getRelChunk(int a_RelChunkX, int a_RelChunkZ)
{
		ASSERT(a_RelChunkX >= 0);
		ASSERT(a_RelChunkZ >= 0);
		ASSERT(a_RelChunkX < 32);
		ASSERT(a_RelChunkZ < 32);

		return m_Chunks[a_RelChunkX + a_RelChunkZ * 32];
}





int Region::getRelBiome(int a_RelBlockX, int a_RelBlockZ)
{
	ASSERT(a_RelBlockX >= 0);
	ASSERT(a_RelBlockZ >= 0);
	ASSERT(a_RelBlockX < 512);
	ASSERT(a_RelBlockZ < 512);

	int chunkX = a_RelBlockX / 16;
	int chunkZ = a_RelBlockZ / 16;
	Chunk & chunk = m_Chunks[chunkX + 32 * chunkZ];
	if (chunk.isValid())
	{
		return chunk.getBiome(a_RelBlockX - 16 * chunkX, a_RelBlockZ - 16 * chunkZ);
	}
	else
	{
		return biInvalidBiome;
	}
}




void Region::blockToRegion(int a_BlockX, int a_BlockZ, int & a_RegionX, int & a_RegionZ)
{
	a_RegionX = static_cast<int>(std::floor(static_cast<float>(a_BlockX) / 512));
	a_RegionZ = static_cast<int>(std::floor(static_cast<float>(a_BlockZ) / 512));
}





void Region::chunkToRegion(int a_ChunkX, int a_ChunkZ, int & a_RegionX, int & a_RegionZ)
{
	a_RegionX = static_cast<int>(std::floor(static_cast<float>(a_ChunkX) / 32));
	a_RegionZ = static_cast<int>(std::floor(static_cast<float>(a_ChunkZ) / 32));
}




