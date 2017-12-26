
// Statistics.h

// Interfaces to the cStatistics class representing a statistics-collecting callback





#pragma once

#include "Callback.h"
#include "Utils.h"





class cStatistics :
	public cCallback
{
public:
	class cStats
	{
	public:
		UInt64 m_TotalChunks;  // Total number of chunks that go through this callback (OnNewChunk())
		UInt64 m_BiomeCounts[256];
		UInt64 m_BlockCounts[256][256];  // First dimension is the biome, second dimension is BlockType
		UInt64 m_BiomeNumChunks;  // Num chunks that have been processed for biome stats
		UInt64 m_BlockNumChunks;  // Num chunks that have been processed for block stats
		UInt64 m_NumEntities;
		UInt64 m_NumTileEntities;
		UInt64 m_NumTileTicks;
		UInt64 m_PerHeightBlockCounts[256][256];  // First dimension is the height, second dimension is BlockType
		UInt64 m_PerHeightSpawners[256][entMax + 1];  // First dimension is the height, second dimension is spawned entity type
		int m_MinChunkX, m_MaxChunkX;  // X coords range
		int m_MinChunkZ, m_MaxChunkZ;  // Z coords range

		Int64 m;
		UInt64 m_SpawnerEntity[entMax + 1];

		cStats(void);
		void Add(const cStats & a_Stats);
		void UpdateCoordsRange(int a_ChunkX, int a_ChunkZ);
	} ;

	cStatistics(void);

	const cStats & GetStats(void) const { return m_Stats; }

protected:
	cStats m_Stats;

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

	virtual bool OnEmptySection(unsigned char a_Y) override;

	virtual bool OnSectionsFinished(void) override { return false; }  // continue processing

	virtual bool OnEntity(
		const AString & a_EntityType,
		double a_PosX, double a_PosY, double a_PosZ,
		double a_SpeedX, double a_SpeedY, double a_SpeedZ,
		float a_Yaw, float a_Pitch,
		float a_FallDistance,
		short a_FireTicksLeft,
		short a_AirTicks,
		char a_IsOnGround,
		cParsedNBT & a_NBT,
		int a_NBTTag
	) override;

	virtual bool OnTileEntity(
		const AString & a_EntityType,
		int a_PosX, int a_PosY, int a_PosZ,
		cParsedNBT & a_NBT,
		int a_NBTTag
	) override;

	virtual bool OnTileTick(
		int a_BlockType,
		int a_TicksLeft,
		int a_PosX, int a_PosY, int a_PosZ
	) override;

	void OnSpawner(cParsedNBT & a_NBT, int a_TileEntityTag);
} ;





class cStatisticsFactory :
	public cCallbackFactory
{
public:
	cStatisticsFactory(void);
	virtual ~cStatisticsFactory();

	virtual cCallback * CreateNewCallback(void)
	{
		return new cStatistics;
	}

protected:
	// The results, combined, are stored here:
	cStatistics::cStats m_CombinedStats;

	clock_t m_BeginTick;

	void JoinResults(void);
	void SaveBiomes(void);
	void SaveBlockTypes(void);
	void SavePerHeightBlockTypes(void);
	void SaveBiomeBlockTypes(void);
	void SaveStatistics(void);
	void SaveSpawners(void);
	void SavePerHeightSpawners(void);
} ;
