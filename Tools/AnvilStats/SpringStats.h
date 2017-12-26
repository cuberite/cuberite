
// SpringStats.h

// Declares the cSpringStats class representing a cCallback descendant that collects statistics on lava and water springs





#pragma once

#include "Callback.h"





class cSpringStats :
	public cCallback
{
public:
	class cStats
	{
	public:
		/** Per-height, per-biome frequencies of springs */
		typedef UInt64 SpringStats[256][256];

		SpringStats m_LavaSprings;
		SpringStats m_WaterSprings;

		/** Total number of chunks that are fully processed through this callback(OnSectionsFinished()) */
		UInt64 m_TotalChunks;

		cStats(void);
		void Add(const cStats & a_Other);
	} ;

	cSpringStats(void);

	const cStats & GetStats(void) const { return m_Stats; }

protected:

	BLOCKTYPE  m_BlockTypes[16 * 16 * 256];
	NIBBLETYPE m_BlockMetas[16 * 16 * 256 / 2];
	char       m_Biomes[16 * 16];
	bool       m_AreBiomesValid;

	cStats m_Stats;

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
	virtual bool OnSectionsFinished(void) override;

	/** Tests the specified block, if it appears to be a spring, it is added to a_Stats */
	void TestSpring(int a_RelX, int a_RelY, int a_RelZ, cStats::SpringStats & a_Stats);
} ;





class cSpringStatsFactory :
	public cCallbackFactory
{
public:
	virtual ~cSpringStatsFactory();

	virtual cCallback * CreateNewCallback(void) override
	{
		return new cSpringStats;
	}

	cSpringStats::cStats m_CombinedStats;

	void JoinResults(void);

	/** Saves total per-height data (summed through biomes) for both spring types to the file */
	void SaveTotals(const AString & a_FileName);

	/** Saves complete per-height, per-biome statistics for the springs to the file */
	void SaveStatistics(const cSpringStats::cStats::SpringStats & a_Stats, const AString & a_FileName);
} ;
