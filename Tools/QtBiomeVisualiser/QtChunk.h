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

	/** Returns the image of the chunk's biomes. Assumes that the chunk is valid. */
	const uchar * getImage(void) const;

	/** Sets the biomes to m_Biomes and renders them into m_Image. */
	void setBiomes(const cChunkDef::BiomeMap & a_Biomes);

	/** Returns the biome at the specified relative coords, or biInvalidBiome if not valid.
	Coords must be valid inside this chunk. */
	EMCSBiome getBiome(int a_RelX, int a_RelZ);

protected:
	/** Flag that specifies if the chunk data is valid - loaded or generated. */
	bool m_IsValid;

	/** Cached rendered image of this chunk's biomes. Updated in render(). */
	Image m_Image;

	/** Biomes comprising the chunk, in the X + 16 * Z ordering. */
	cChunkDef::BiomeMap m_Biomes;


	/** Renders biomes from m_Biomes into m_Image. */
	void renderBiomes();
};

typedef std::shared_ptr<Chunk> ChunkPtr;





