
// HeightMap.h

// Declares the cHeightMap class representing a cCallback descendant that draws a B & W map of heights for the world





#pragma once

#include "Callback.h"





class cHeightMap :
	public cCallback
{
public:
	cHeightMap(void);

	void Finish(void);

	static bool IsGround(BLOCKTYPE a_BlockType);

protected:
	int m_CurrentChunkX;  // Absolute chunk coords
	int m_CurrentChunkZ;
	int m_CurrentChunkOffX;  // Chunk offset from the start of the region
	int m_CurrentChunkOffZ;
	int m_CurrentRegionX;
	int m_CurrentRegionZ;
	bool m_IsCurrentRegionValid;
	/** Height-map of the entire current region [x + 16 * 32 * z] */
	int  m_Height[16 * 32 * 16 * 32];
	/** Block data of the currently processed chunk (between OnSection() and OnSectionsFinished()) */
	BLOCKTYPE m_BlockTypes[16 * 16 * 256];

	// cCallback overrides:
	virtual bool OnNewChunk(int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnHeader(int a_FileOffset, unsigned char a_NumSectors, int a_Timestamp) override { return false; }
	virtual bool OnCompressedDataSizePos(int a_CompressedDataSize, int a_DataOffset, char a_CompressionMethod) override { return false; }
	virtual bool OnDecompressedData(const char * a_DecompressedNBT, int a_DataSize) override { return false; }
	virtual bool OnRealCoords(int a_ChunkX, int a_ChunkZ) override { return false; }
	virtual bool OnLastUpdate(Int64 a_LastUpdate) override { return false; }
	virtual bool OnTerrainPopulated(bool a_Populated) override { return !a_Populated; }  // If not populated, we don't want it!
	virtual bool OnBiomes(const unsigned char * a_BiomeData) { return false; }
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

	void StartNewRegion(int a_RegionX, int a_RegionZ);
} ;





class cHeightMapFactory :
	public cCallbackFactory
{
public:
	virtual ~cHeightMapFactory();

	virtual cCallback * CreateNewCallback(void) override
	{
		return new cHeightMap;
	}

} ;
