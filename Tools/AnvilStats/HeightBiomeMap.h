
// HeightBiomeMap.h

// Declares the cHeightBiomeMap class representing a stats module that produces an image of heights and biomes combined





#pragma once

#include "ImageComposingCallback.h"





class cHeightBiomeMap :
	public cImageComposingCallback
{
	typedef cImageComposingCallback super;

public:
	// Minima and maxima for the regions processed through this callback
	int m_MinRegionX, m_MaxRegionX;
	int m_MinRegionZ, m_MaxRegionZ;

	cHeightBiomeMap(void);

protected:
	int m_CurrentChunkX;  // Absolute chunk coords
	int m_CurrentChunkZ;
	int m_CurrentChunkRelX;  // Chunk offset from the start of the region
	int m_CurrentChunkRelZ;

	/** Biome-map  for the current chunk */
	char      m_ChunkBiomes[16 * 16];
	/** Height-map for the current chunk */
	int       m_ChunkHeight[16 * 16];
	/** Block data for the current chunk (between OnSection() and OnSectionsFinished()) */
	BLOCKTYPE m_BlockTypes [16 * 16 * 256];

	// cCallback overrides:
	virtual bool OnNewRegion(int a_RegionX, int a_RegionZ) override;
	virtual bool OnNewChunk(int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnHeader(int a_FileOffset, unsigned char a_NumSectors, int a_Timestamp) override { return CALLBACK_CONTINUE; }
	virtual bool OnCompressedDataSizePos(int a_CompressedDataSize, int a_DataOffset, char a_CompressionMethod) override { return CALLBACK_CONTINUE; }
	virtual bool OnDecompressedData(const char * a_DecompressedNBT, int a_DataSize) override { return CALLBACK_CONTINUE; }
	virtual bool OnRealCoords(int a_ChunkX, int a_ChunkZ) override { return CALLBACK_CONTINUE; }
	virtual bool OnLastUpdate(Int64 a_LastUpdate) override { return CALLBACK_CONTINUE; }
	virtual bool OnTerrainPopulated(bool a_Populated) override { return a_Populated ? CALLBACK_CONTINUE : CALLBACK_ABORT; }  // If not populated, we don't want it!
	virtual bool OnBiomes(const unsigned char * a_BiomeData) override;
	virtual bool OnHeightMap(const int * a_HeightMapBE) override;
	virtual bool OnSection(
		unsigned char a_Y,
		const BLOCKTYPE * a_BlockTypes,
		const NIBBLETYPE * a_BlockAdditional,
		const NIBBLETYPE * a_BlockMeta,
		const NIBBLETYPE * a_BlockLight,
		const NIBBLETYPE * a_BlockSkyLight
	) override;
	virtual bool OnSectionsFinished(void) override;

} ;





class cHeightBiomeMapFactory :
	public cCallbackFactory
{
public:
	virtual ~cHeightBiomeMapFactory();

	virtual cCallback * CreateNewCallback(void) override
	{
		return new cHeightBiomeMap;
	}
} ;
