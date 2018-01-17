
// Statistics.cpp

// Implements the various statistics-collecting classes

#include "Globals.h"
#include "Statistics.h"
#include "../../src/WorldStorage/FastNBT.h"





////////////////////////////////////////////////////////////////////////////////
// cStatistics::cStats:

cStatistics::cStats::cStats(void) :
	m_TotalChunks(0),
	m_BiomeNumChunks(0),
	m_BlockNumChunks(0),
	m_NumEntities(0),
	m_NumTileEntities(0),
	m_NumTileTicks(0),
	m_MinChunkX(0x7fffffff),
	m_MaxChunkX(0x80000000),
	m_MinChunkZ(0x7fffffff),
	m_MaxChunkZ(0x80000000)
{
	memset(m_BiomeCounts,          0, sizeof(m_BiomeCounts));
	memset(m_BlockCounts,          0, sizeof(m_BlockCounts));
	memset(m_PerHeightBlockCounts, 0, sizeof(m_PerHeightBlockCounts));
	memset(m_PerHeightSpawners,    0, sizeof(m_PerHeightSpawners));
	memset(m_SpawnerEntity,        0, sizeof(m_SpawnerEntity));
}





void cStatistics::cStats::Add(const cStatistics::cStats & a_Stats)
{
	for (int i = 0; i <= 255; i++)
	{
		m_BiomeCounts[i] += a_Stats.m_BiomeCounts[i];
	}
	for (int i = 0; i <= 255; i++)
	{
		for (int j = 0; j <= 255; j++)
		{
			m_BlockCounts[i][j] += a_Stats.m_BlockCounts[i][j];
			m_PerHeightBlockCounts[i][j] += a_Stats.m_PerHeightBlockCounts[i][j];
		}
		for (int j = 0; j < ARRAYCOUNT(m_PerHeightSpawners[0]); j++)
		{
			m_PerHeightSpawners[i][j] += a_Stats.m_PerHeightSpawners[i][j];
		}
	}
	for (int i = 0; i < ARRAYCOUNT(m_SpawnerEntity); i++)
	{
		m_SpawnerEntity[i] += a_Stats.m_SpawnerEntity[i];
	}
	m_BiomeNumChunks += a_Stats.m_BiomeNumChunks;
	m_BlockNumChunks += a_Stats.m_BlockNumChunks;
	m_TotalChunks += a_Stats.m_TotalChunks;
	m_NumEntities += a_Stats.m_NumEntities;
	m_NumTileEntities += a_Stats.m_NumTileEntities;
	m_NumTileTicks += a_Stats.m_NumTileTicks;
	UpdateCoordsRange(a_Stats.m_MinChunkX, a_Stats.m_MinChunkZ);
	UpdateCoordsRange(a_Stats.m_MinChunkX, a_Stats.m_MinChunkZ);
}





void cStatistics::cStats::UpdateCoordsRange(int a_ChunkX, int a_ChunkZ)
{
	if (a_ChunkX < m_MinChunkX)
	{
		m_MinChunkX = a_ChunkX;
	}
	if (a_ChunkX > m_MaxChunkX)
	{
		m_MaxChunkX = a_ChunkX;
	}
	if (a_ChunkZ < m_MinChunkZ)
	{
		m_MinChunkZ = a_ChunkZ;
	}
	if (a_ChunkZ > m_MaxChunkZ)
	{
		m_MaxChunkZ = a_ChunkZ;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cStatistics:

cStatistics::cStatistics(void)
{
}





bool cStatistics::OnNewChunk(int a_ChunkX, int a_ChunkZ)
{
	m_Stats.m_TotalChunks++;
	m_Stats.UpdateCoordsRange(a_ChunkX, a_ChunkZ);
	m_IsBiomesValid = false;
	m_IsFirstSectionInChunk = true;
	return false;
}





bool cStatistics::OnBiomes(const unsigned char * a_BiomeData)
{
	for (int i = 0; i < 16 * 16; i++)
	{
		m_Stats.m_BiomeCounts[a_BiomeData[i]] += 1;
	}
	m_Stats.m_BiomeNumChunks += 1;
	memcpy(m_BiomeData, a_BiomeData, sizeof(m_BiomeData));
	m_IsBiomesValid = true;
	return false;
}






bool cStatistics::OnSection
(
	unsigned char a_Y,
	const BLOCKTYPE * a_BlockTypes,
	const NIBBLETYPE * a_BlockAdditional,
	const NIBBLETYPE * a_BlockMeta,
	const NIBBLETYPE * a_BlockLight,
	const NIBBLETYPE * a_BlockSkyLight
)
{
	if (!m_IsBiomesValid)
	{
		// The current biome data is not valid, we don't have the means for sorting the BlockTypes into per-biome arrays
		return true;
	}

	for (int y = 0; y < 16; y++)
	{
		int Height = (int)a_Y * 16 + y;
		for (int z = 0; z < 16; z++)
		{
			for (int x = 0; x < 16; x++)
			{
				unsigned char Biome = m_BiomeData[x + 16 * z];  // Cannot use cChunkDef, different datatype
				unsigned char BlockType = cChunkDef::GetBlock(a_BlockTypes, x, y, z);
				m_Stats.m_BlockCounts[Biome][BlockType] += 1;
				m_Stats.m_PerHeightBlockCounts[Height][BlockType] += 1;
			}
		}
	}

	m_Stats.m_BlockNumChunks += m_IsFirstSectionInChunk ? 1 : 0;
	m_IsFirstSectionInChunk = false;

	return false;
}





bool cStatistics::OnEmptySection(unsigned char a_Y)
{
	if (!m_IsBiomesValid)
	{
		// The current biome data is not valid, we don't have the means for sorting the BlockTypes into per-biome arrays
		return true;
	}

	// Add air to all columns:
	for (int z = 0; z < 16; z++)
	{
		for (int x = 0; x < 16; x++)
		{
			unsigned char Biome = m_BiomeData[x + 16 * z];  // Cannot use cChunkDef, different datatype
			m_Stats.m_BlockCounts[Biome][0] += 16;  // 16 blocks in a column, all air
		}
	}

	m_Stats.m_BlockNumChunks += m_IsFirstSectionInChunk ? 1 : 0;
	m_IsFirstSectionInChunk = false;

	return false;
}





bool cStatistics::OnEntity(
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
)
{
	m_Stats.m_NumEntities += 1;

	// TODO

	return false;
}





bool cStatistics::OnTileEntity(
	const AString & a_EntityType,
	int a_PosX, int a_PosY, int a_PosZ,
	cParsedNBT & a_NBT,
	int a_NBTTag
)
{
	m_Stats.m_NumTileEntities += 1;

	if (a_EntityType == "MobSpawner")
	{
		OnSpawner(a_NBT, a_NBTTag);
	}

	return false;
}





bool cStatistics::OnTileTick(
	int a_BlockType,
	int a_TicksLeft,
	int a_PosX, int a_PosY, int a_PosZ
)
{
	m_Stats.m_NumTileTicks += 1;
	return false;
}





void cStatistics::OnSpawner(cParsedNBT & a_NBT, int a_TileEntityTag)
{
	// Get the spawned entity type:
	int EntityIDTag = a_NBT.FindChildByName(a_TileEntityTag, "EntityId");
	if ((EntityIDTag < 0) || (a_NBT.GetType(EntityIDTag) != TAG_String))
	{
		return;
	}
	eEntityType Ent = GetEntityType(a_NBT.GetString(EntityIDTag));
	if (Ent >= ARRAYCOUNT(m_Stats.m_SpawnerEntity))
	{
		return;
	}
	m_Stats.m_SpawnerEntity[Ent] += 1;

	// Get the spawner pos:
	int PosYTag = a_NBT.FindChildByName(a_TileEntityTag, "y");
	if ((PosYTag < 0) || (a_NBT.GetType(PosYTag) != TAG_Int))
	{
		return;
	}
	int BlockY = Clamp(a_NBT.GetInt(PosYTag), 0, 255);
	m_Stats.m_PerHeightSpawners[BlockY][Ent] += 1;
}





////////////////////////////////////////////////////////////////////////////////
// cStatisticsFactory:

cStatisticsFactory::cStatisticsFactory(void) :
	m_BeginTick(clock())
{
}





cStatisticsFactory::~cStatisticsFactory()
{
	// Join the results together:
	LOG("cStatistics:");
	LOG("  Joining results...");
	JoinResults();
	LOG("  Total %llu chunks went through", m_CombinedStats.m_TotalChunks);
	LOG("  Biomes processed for %llu chunks", m_CombinedStats.m_BiomeNumChunks);

	// Check the number of blocks processed
	UInt64 TotalBlocks = 0;
	for (int i = 0; i <= 255; i++)
	{
		for (int j = 0; j < 255; j++)
		{
			TotalBlocks += m_CombinedStats.m_BlockCounts[i][j];
		}
	}
	UInt64 ExpTotalBlocks = m_CombinedStats.m_BlockNumChunks * 16LL * 16LL * 256LL;
	LOG("  BlockIDs processed for %llu chunks, %llu blocks (exp %llu; %s)", m_CombinedStats.m_BlockNumChunks, TotalBlocks, ExpTotalBlocks, (TotalBlocks == ExpTotalBlocks) ? "match" : "failed");

	// Save statistics:
	LOG("  Saving statistics into files:");
	LOG("    Statistics.txt");
	SaveStatistics();
	LOG("    Biomes.xls");
	SaveBiomes();
	LOG("    BlockTypes.xls");
	SaveBlockTypes();
	LOG("    PerHeightBlockTypes.xls");
	SavePerHeightBlockTypes();
	LOG("    BiomeBlockTypes.xls");
	SaveBiomeBlockTypes();
	LOG("    Spawners.xls");
	SaveSpawners();
	LOG("    PerHeightSpawners.xls");
	SavePerHeightSpawners();
}





void cStatisticsFactory::JoinResults(void)
{
	for (cCallbacks::iterator itr = m_Callbacks.begin(), end = m_Callbacks.end(); itr != end; ++itr)
	{
		m_CombinedStats.Add(((cStatistics *)(*itr))->GetStats());
	}  // for itr - m_Callbacks[]
}





void cStatisticsFactory::SaveBiomes(void)
{
	cFile f;
	if (!f.Open("Biomes.xls", cFile::fmWrite))
	{
		LOG("Cannot write to file Biomes.xls. Statistics not written.");
		return;
	}
	double TotalColumns = (double)(m_CombinedStats.m_BiomeNumChunks) * 16 * 16 / 100;  // Total number of columns processed; convert into percent
	if (TotalColumns < 1)
	{
		// Avoid division by zero
		TotalColumns = 1;
	}
	for (int i = 0; i <= 255; i++)
	{
		AString Line;
		Printf(Line, "%s\t%d\t%llu\t%.05f\n", GetBiomeString(i), i, m_CombinedStats.m_BiomeCounts[i], ((double)(m_CombinedStats.m_BiomeCounts[i])) / TotalColumns);
		f.Write(Line.c_str(), Line.length());
	}
}





void cStatisticsFactory::SaveBlockTypes(void)
{
	cFile f;
	if (!f.Open("BlockTypes.xls", cFile::fmWrite))
	{
		LOG("Cannot write to file Biomes.xls. Statistics not written.");
		return;
	}
	double TotalBlocks = ((double)(m_CombinedStats.m_BlockNumChunks)) * 16 * 16 * 256 / 100;  // Total number of blocks processed; convert into percent
	if (TotalBlocks < 1)
	{
		// Avoid division by zero
		TotalBlocks = 1;
	}
	for (int i = 0; i <= 255; i++)
	{
		UInt64 Count = 0;
		for (int Biome = 0; Biome <= 255; ++Biome)
		{
			Count += m_CombinedStats.m_BlockCounts[Biome][i];
		}
		AString Line;
		Printf(Line, "%s\t%d\t%llu\t%.08f\n", GetBlockTypeString(i), i, Count, ((double)Count) / TotalBlocks);
		f.Write(Line.c_str(), Line.length());
	}
}





void cStatisticsFactory::SavePerHeightBlockTypes(void)
{
	// Export as two tables: biomes 0-127 and 128-255, because OpenOffice doesn't support more than 256 columns

	cFile f;
	if (!f.Open("PerHeightBlockTypes.xls", cFile::fmWrite))
	{
		LOG("Cannot write to file PerHeightBlockTypes.xls. Statistics not written.");
		return;
	}

	// Write header:
	f.Printf("Blocks 0 - 127:\nHeight");
	for (int i = 0; i < 128; i++)
	{
		f.Printf("\t%s(%d)", GetBlockTypeString(i), i);
	}
	f.Printf("\n");

	// Write first half:
	for (int y = 0; y < 256; y++)
	{
		f.Printf("%d", y);
		for (int BlockType = 0; BlockType < 128; BlockType++)
		{
			f.Printf("\t%llu", m_CombinedStats.m_PerHeightBlockCounts[y][BlockType]);
		}  // for BlockType
		f.Printf("\n");
	}  // for y - height (0 - 127)
	f.Printf("\n");

	// Write second header:
	f.Printf("Blocks 128 - 255:\nHeight");
	for (int i = 128; i < 256; i++)
	{
		f.Printf("\t%s(%d)", GetBlockTypeString(i), i);
	}
	f.Printf("\n");

	// Write second half:
	for (int y = 0; y < 256; y++)
	{
		f.Printf("%d", y);
		for (int BlockType = 128; BlockType < 256; BlockType++)
		{
			f.Printf("\t%llu", m_CombinedStats.m_PerHeightBlockCounts[y][BlockType]);
		}  // for BlockType
		f.Printf("\n");
	}  // for y - height (0 - 127)
}





void cStatisticsFactory::SaveBiomeBlockTypes(void)
{
	// Export as two tables: biomes 0-127 and 128-255, because OpenOffice doesn't support more than 256 columns
	cFile f;
	if (!f.Open("BiomeBlockTypes.xls", cFile::fmWrite))
	{
		LOG("Cannot write to file BiomeBlockTypes.xls. Statistics not written.");
		return;
	}

	AString FileHeader("Biomes 0-127:\n");
	f.Write(FileHeader.c_str(), FileHeader.length());

	AString Header("BlockType\tBlockType");
	for (int Biome = 0; Biome <= 127; Biome++)
	{
		const char * BiomeName = GetBiomeString(Biome);
		if ((BiomeName != NULL) && (BiomeName[0] != 0))
		{
			AppendPrintf(Header, "\t%s (%d)", BiomeName, Biome);
		}
		else
		{
			AppendPrintf(Header, "\t%d", Biome);
		}
	}
	Header.append("\n");
	f.Write(Header.c_str(), Header.length());

	for (int BlockType = 0; BlockType <= 255; BlockType++)
	{
		AString Line;
		Printf(Line, "%s\t%d", GetBlockTypeString(BlockType), BlockType);
		for (int Biome = 0; Biome <= 127; Biome++)
		{
			AppendPrintf(Line, "\t%llu", m_CombinedStats.m_BlockCounts[Biome][BlockType]);
		}
		Line.append("\n");
		f.Write(Line.c_str(), Line.length());
	}

	Header.assign("\n\nBiomes 127-255:\nBlockType\tBlockType");
	for (int Biome = 0; Biome <= 127; Biome++)
	{
		const char * BiomeName = GetBiomeString(Biome);
		if ((BiomeName != NULL) && (BiomeName[0] != 0))
		{
			AppendPrintf(Header, "\t%s (%d)", BiomeName, Biome);
		}
		else
		{
			AppendPrintf(Header, "\t%d", Biome);
		}
	}
	Header.append("\n");
	f.Write(Header.c_str(), Header.length());

	for (int BlockType = 0; BlockType <= 255; BlockType++)
	{
		AString Line;
		Printf(Line, "%s\t%d", GetBlockTypeString(BlockType), BlockType);
		for (int Biome = 128; Biome <= 255; Biome++)
		{
			AppendPrintf(Line, "\t%llu", m_CombinedStats.m_BlockCounts[Biome][BlockType]);
		}
		Line.append("\n");
		f.Write(Line.c_str(), Line.length());
	}
}






void cStatisticsFactory::SaveStatistics(void)
{
	cFile f;
	if (!f.Open("Statistics.txt", cFile::fmWrite))
	{
		LOG("Cannot write to file Statistics.txt. Statistics not written.");
		return;
	}

	int Elapsed = (clock() - m_BeginTick) / CLOCKS_PER_SEC;
	f.Printf("Time elapsed: %d seconds (%d hours, %d minutes and %d seconds)\n", Elapsed, Elapsed / 3600, (Elapsed / 60) % 60, Elapsed % 60);
	f.Printf("Total chunks processed: %llu\n", m_CombinedStats.m_TotalChunks);
	if (Elapsed > 0)
	{
		f.Printf("Chunk processing speed: %.02f chunks per second\n", (double)(m_CombinedStats.m_TotalChunks) / Elapsed);
	}
	f.Printf("Biomes counted for %llu chunks.\n", m_CombinedStats.m_BiomeNumChunks);
	f.Printf("Blocktypes counted for %llu chunks.\n", m_CombinedStats.m_BlockNumChunks);
	f.Printf("Total blocks counted: %llu\n", m_CombinedStats.m_BlockNumChunks * 16 * 16 * 256);
	f.Printf("Total biomes counted: %llu\n", m_CombinedStats.m_BiomeNumChunks * 16 * 16);
	f.Printf("Total entities counted: %llu\n", m_CombinedStats.m_NumEntities);
	f.Printf("Total tile entities counted: %llu\n", m_CombinedStats.m_NumTileEntities);
	f.Printf("Total tile ticks counted: %llu\n", m_CombinedStats.m_NumTileTicks);
	f.Printf("Chunk coord ranges:\n");
	f.Printf("\tX: %d .. %d\n", m_CombinedStats.m_MinChunkX, m_CombinedStats.m_MaxChunkX);
	f.Printf("\tZ: %d .. %d\n", m_CombinedStats.m_MinChunkZ, m_CombinedStats.m_MaxChunkZ);
}





void cStatisticsFactory::SaveSpawners(void)
{
	cFile f;
	if (!f.Open("Spawners.xls", cFile::fmWrite))
	{
		LOG("Cannot write to file Spawners.xls. Statistics not written.");
		return;
	}

	f.Printf("Entity type\tTotal count\tCount per chunk\n");
	for (int i = 0; i < entMax; i++)
	{
		f.Printf("%s\t%llu\t%0.4f\n", GetEntityTypeString((eEntityType)i), m_CombinedStats.m_SpawnerEntity[i], (double)(m_CombinedStats.m_SpawnerEntity[i]) / m_CombinedStats.m_BlockNumChunks);
	}
}





void cStatisticsFactory::SavePerHeightSpawners(void)
{
	cFile f;
	if (!f.Open("PerHeightSpawners.xls", cFile::fmWrite))
	{
		LOG("Cannot write to file PerHeightSpawners.xls. Statistics not written.");
		return;
	}

	// Write header:
	f.Printf("Height\tTotal");
	for (int i = 0; i < entMax; i++)
	{
		f.Printf("\t%s", GetEntityTypeString((eEntityType)i));
	}
	f.Printf("\n");

	// Write individual lines:
	for (int y = 0; y < 256; y++)
	{
		UInt64 Total = 0;
		for (int i = 0; i < entMax; i++)
		{
			Total += m_CombinedStats.m_PerHeightSpawners[y][i];
		}
		f.Printf("%d\t%llu", y, Total);
		for (int i = 0; i < entMax; i++)
		{
			f.Printf("\t%llu", m_CombinedStats.m_PerHeightSpawners[y][i]);
		}
		f.Printf("\n");
	}
}
