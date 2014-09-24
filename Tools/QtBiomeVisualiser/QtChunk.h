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

	/** Sets the image data for this chunk. */
	void setImage(const Image & a_Image);

protected:
	/** Flag that specifies if the chunk data is valid - loaded or generated. */
	bool m_IsValid;

	/** Cached rendered image of this chunk's biomes. Updated in render(). */
	Image m_Image;
};

typedef std::shared_ptr<Chunk> ChunkPtr;





