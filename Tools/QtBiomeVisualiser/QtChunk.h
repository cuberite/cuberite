#pragma once

#include <qglobal.h>





class Chunk
{
public:
	/** The type used for storing image data for a chunk. */
	typedef uchar Image[16 * 16 * 4];


	Chunk(void);

	/** Returns true iff the chunk data is valid - loaded or generated. */
	bool isValid(void) const { return m_IsValid; }

	/** Sets the biomes to m_Biomes and renders them into m_Image. */
	void setBiomes(const cChunkDef::BiomeMap & a_Biomes);

	/** Returns the biome at the specified relative coords, or biInvalidBiome if not valid.
	Coords must be valid inside this chunk. */
	EMCSBiome getBiome(int a_RelX, int a_RelZ);

	/** Returns the raw biome data for this chunk. */
	const short * getBiomes(void) const { return m_Biomes; }

protected:
	/** Flag that specifies if the chunk data is valid - loaded or generated. */
	bool m_IsValid;

	/** Biomes comprising the chunk, in the X + 16 * Z ordering.
	Typed as short to save on memory, converted automatically when needed. */
	short m_Biomes[16 * 16];
};

typedef std::shared_ptr<Chunk> ChunkPtr;





