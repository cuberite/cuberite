
// Statistics.h

// Interfaces to the cStatistics class representing a statistics-collecting callback





#pragma once

#include "Callback.h"





class cStatistics :
	public cCallback
{
	friend class cStatisticsFactory;
	
public:
	cStatistics(void);
	
protected:
	int m_TotalChunks;  // Total number of chunks that go through this callback (OnNewChunk())
	int m_BiomeCounts[256];
	int m_BlockCounts[256][256];  // First dimension is the biome, second dimension is BlockType
	int m_BiomeNumChunks;  // Num chunks that have been processed for biome stats
	int m_BlockNumChunks;  // Num chunks that have been processed for block stats
	bool m_IsBiomesValid;  // Set to true in OnBiomes(), reset to false in OnNewChunk(); if true, the m_BiomeData is valid for the current chunk
	unsigned char m_BiomeData[16 * 16];
	bool m_IsFirstSectionInChunk;  // True if there was no section in the chunk yet. Set by OnNewChunk(), reset by OnSection()

	// cCallback overrides:
	virtual bool OnNewChunk(int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnHeader(int a_FileOffset, unsigned char a_NumSectors, int a_Timestamp) override { return false; }
	virtual bool OnCompressedDataSizePos(int a_CompressedDataSize, int a_DataOffset, char a_CompressionMethod) override { return false; }
	virtual bool OnDecompressedData(const char * a_DecompressedNBT, int a_DataSize) override { return false; }
	virtual bool OnRealCoords(int a_ChunkX, int a_ChunkZ) override { return false; }
	virtual bool OnLastUpdate(Int64 a_LastUpdate) override { return false; }
	virtual bool OnTerrainPopulated(bool a_Populated) override { return !a_Populated; }  // If not populated, we don't want it!
	virtual bool OnBiomes(const unsigned char * a_BiomeData) override;
	virtual bool OnHeightMap(const int * a_HeightMap) override { return false; }
	virtual bool OnSection(
		unsigned char a_Y,
		const BLOCKTYPE * a_BlockTypes,
		const NIBBLETYPE * a_BlockAdditional,
		const NIBBLETYPE * a_BlockMeta,
		const NIBBLETYPE * a_BlockLight,
		const NIBBLETYPE * a_BlockSkyLight
	) override;
} ;





class cStatisticsFactory :
	public cCallbackFactory
{
public:
	virtual ~cStatisticsFactory();
	
	virtual cCallback * CreateNewCallback(void)
	{
		return new cStatistics;
	}
	
protected:
	// The results, combined, are stored here:
	int m_TotalChunks;
	int m_BiomeCounts[256];
	int m_BlockCounts[256][256];  // First dimension is the biome, second dimension is BlockType
	int m_BiomeNumChunks;  // Num chunks that have been processed for biome stats
	int m_BlockNumChunks;  // Num chunks that have been processed for block stats

	void JoinResults(void);
	void SaveBiomes(void);
	void SaveBlockTypes(void);
	void SaveBiomeBlockTypes(void);

} ;




