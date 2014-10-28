#pragma once

#include "QtChunk.h"





class Region
{
public:
	Region();

	/** Retrieves the chunk with the specified relative coords. */
	Chunk & getRelChunk(int a_RelChunkX, int a_RelChunkZ);

	/** Returns true iff the chunk data for all chunks has been loaded.
	This doesn't mean that all the chunks are valid, only that the entire region has been processed and should
	be displayed. */
	bool isValid(void) const { return m_IsValid; }

	/** Returns the biome in the block coords relative to this region.
	Returns biInvalidBiome if the underlying chunk is not valid. */
	int getRelBiome(int a_RelBlockX, int a_RelBlockZ);

	/** Converts block coordinates into region coordinates. */
	static void blockToRegion(int a_BlockX, int a_BlockZ, int & a_RegionX, int & a_RegionZ);

	/** Converts chunk coordinates into region coordinates. */
	static void chunkToRegion(int a_ChunkX, int a_ChunkZ, int & a_RegionX, int & a_RegionZ);

protected:
	friend class RegionLoader;


	Chunk m_Chunks[32 * 32];

	/** True iff the data for all the chunks has been loaded.
	This doesn't mean that all the chunks are valid, only that the entire region has been processed and should
	be displayed. */
	bool m_IsValid;
};




