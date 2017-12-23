
// Zapper.h

// Declares the cZapper class representing the processor that actually zaps blocks and entities





#pragma once

#include "Regions.h"





// fwd: ParsedNBT.h
class cParsedNBT;
class cFastNBTWriter;





class cZapper
{
public:
	cZapper(const AString & a_MCAFolder);

	/** Zaps all the specified regions */
	void ZapRegions(const cRegionVector & a_Regions);

protected:
	static const int BlocksPerChunkX = 16;
	static const int BlocksPerChunkZ = 16;
	static const int ChunksPerMCAX = 32;
	static const int ChunksPerMCAZ = 32;

	AString m_MCAFolder;

	/** Converts from block coords to MCA coords */
	void BlockToMCA(int a_BlockX, int a_BlockZ, int & a_MCAX, int & a_MCAZ);

	/** Converts from block coords to chunk coords */
	void BlockToChunk(int a_BlockX, int a_BlockZ, int & a_ChunkX, int & a_ChunkZ);

	/** Zaps the specified region in the MCA file with the specified MCA coords */
	void ZapRegionInMCAFile(const cRegion & a_Region, int a_MCAX, int a_MCAZ);

	/** Loads raw compressed chunk data from the specified file
	* chunk is specified by ChunkHeaderValue, which is the int describing the chunk in file header.
	*/
	void LoadChunkData(cFile & a_InFile, int a_ChunkHeaderValue, AString & a_ChunkData, int a_ChunkX, int a_ChunkZ);

	/** Zaps the specified region in the raw (compressed) chunk data. */
	void ZapRegionInRawChunkData(const cRegion & a_Region, AString & a_ChunkData, int a_ChunkX, int a_ChunkZ);

	/** Zaps the specified region in the specified NBT structure */
	void ZapRegionInNBTChunk(const cRegion & a_Region, cParsedNBT & a_NBT, int a_ChunkX, int a_ChunkZ);

	/** Zaps the blocks in the specified region from the specified NBT */
	void ZapRegionBlocksInNBT(const cRegion & a_Region, cParsedNBT & a_NBT, int a_SectionsTag);

	/** Zaps the blocks in the specified bytes (types) from one vertical section (16^3 blocks) of a chunk. */
	void ZapRegionInNBTSectionBytes(const cRegion & a_Region, int a_SectionY, unsigned char * a_BlockBytes);

	/** Zaps the blocks in the specified nibbles (meta, add) from one vertical section (16^3 blocks) of a chunk. */
	void ZapRegionInNBTSectionNibbles(const cRegion & a_Region, int a_SectionY, unsigned char * a_BlockNibbles);

	/** Zaps entities in the specified region from the specified NBT */
	void ZapRegionEntitiesInNBT(const cRegion & a_Region, cParsedNBT & a_NBT, int a_EntitiesTag);

	/** Serializes the NBT subtree into a writer */
	void SerializeNBTTag(const cParsedNBT & a_NBT, int a_Tag, cFastNBTWriter & a_Writer);
} ;
