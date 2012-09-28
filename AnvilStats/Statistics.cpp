
// Statistics.cpp

// Implements the various statistics-collecting classes

#include "Globals.h"
#include "Statistics.h"
#include "Utils.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStatistics:

cStatistics::cStatistics(void) :
	m_TotalChunks(0),
	m_BiomeNumChunks(0),
	m_BlockNumChunks(0)
{
	memset(m_BiomeCounts, 0, sizeof(m_BiomeCounts));
	memset(m_BlockCounts, 0, sizeof(m_BlockCounts));
}





bool cStatistics::OnNewChunk(int a_ChunkX, int a_ChunkZ)
{
	m_TotalChunks++;
	m_IsBiomesValid = false;
	m_IsFirstSectionInChunk = true;
	return false;
}





bool cStatistics::OnBiomes(const unsigned char * a_BiomeData)
{
	for (int i = 0; i < 16 * 16; i++)
	{
		m_BiomeCounts[a_BiomeData[i]] += 1;
	}
	m_BiomeNumChunks += 1;
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
		for (int z = 0; z < 16; z++)
		{
			for (int x = 0; x < 16; x++)
			{
				unsigned char Biome = m_BiomeData[x + 16 * z];  // Cannot use cChunkDef, different datatype
				unsigned char BlockType = cChunkDef::GetBlock(a_BlockTypes, x, y, z);
				m_BlockCounts[Biome][BlockType] += 1;
			}
		}
	}
	
	m_BlockNumChunks += m_IsFirstSectionInChunk ? 1 : 0;
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
			m_BlockCounts[Biome][0] += 16;  // 16 blocks in a column, all air
		}
	}
	
	return false;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStatisticsFactory:

cStatisticsFactory::~cStatisticsFactory()
{
	// Join the results together:
	LOG("cStatistics:");
	LOG("  Joining results...");
	JoinResults();
	LOG("  Total %d chunks went through", m_TotalChunks);
	LOG("  Biomes processed for %d chunks", m_BiomeNumChunks);
	
	// Check the number of blocks processed
	Int64 TotalBlocks = 0;
	for (int i = 0; i <= 255; i++)
	{
		for (int j = 0; j < 255; j++)
		{
			TotalBlocks += m_BlockCounts[i][j];
		}
	}
	Int64 ExpTotalBlocks = (Int64)m_BlockNumChunks * 16LL * 16LL * 256LL;
	LOG("  BlockIDs processed for %d chunks, %lld blocks (exp %lld; %s)", m_BlockNumChunks, TotalBlocks, ExpTotalBlocks, (TotalBlocks == ExpTotalBlocks) ? "match" : "failed");
	
	// Save statistics:
	LOG("  Saving statistics into files:");
	LOG("    Biomes.txt");
	SaveBiomes();
	LOG("    BlockTypes.txt");
	SaveBlockTypes();
	LOG("    BiomeBlockTypes.txt");
	SaveBiomeBlockTypes();
}





void cStatisticsFactory::JoinResults(void)
{
	m_BiomeNumChunks = 0;
	m_BlockNumChunks = 0;
	m_TotalChunks = 0;
	memset(m_BiomeCounts, 0, sizeof(m_BiomeCounts));
	memset(m_BlockCounts, 0, sizeof(m_BlockCounts));
	for (cCallbacks::iterator itr = m_Callbacks.begin(), end = m_Callbacks.end(); itr != end; ++itr)
	{
		cStatistics * stats = (cStatistics *)(*itr);
		for (int i = 0; i <= 255; i++)
		{
			m_BiomeCounts[i] += stats->m_BiomeCounts[i];
		}
		for (int i = 0; i <= 255; i++)
		{
			for (int j = 0; j <= 255; j++)
			{
				m_BlockCounts[i][j] += stats->m_BlockCounts[i][j];
			}
		}
		m_BiomeNumChunks += stats->m_BiomeNumChunks;
		m_BlockNumChunks += stats->m_BlockNumChunks;
		m_TotalChunks += stats->m_TotalChunks;
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
	double TotalColumns = (double)m_BiomeNumChunks * 16 * 16;  // Total number of columns processed
	if (TotalColumns < 1)
	{
		// Avoid division by zero
		TotalColumns = 1;
	}
	for (int i = 0; i <= 255; i++)
	{
		AString Line;
		Printf(Line, "%s\t%d\t%.05f\n", GetBiomeString(i), i, m_BiomeCounts[i], ((double)m_BiomeCounts[i]) / TotalColumns);
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
	double TotalBlocks = ((double)m_BlockNumChunks) * 16 * 16 * 256 / 100;  // Total number of blocks processed
	if (TotalBlocks < 1)
	{
		// Avoid division by zero
		TotalBlocks = 1;
	}
	for (int i = 0; i <= 255; i++)
	{
		int Count = 0;
		for (int Biome = 0; Biome <= 255; ++Biome)
		{
			Count += m_BlockCounts[Biome][i];
		}
		AString Line;
		Printf(Line, "%s\t%d\t%d\t%.08f\n", GetBlockTypeString(i), i, Count, ((double)Count) / TotalBlocks);
		f.Write(Line.c_str(), Line.length());
	}
	// TODO
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
			AppendPrintf(Line, "\t%d", m_BlockCounts[Biome][BlockType]);
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
			AppendPrintf(Line, "\t%d", m_BlockCounts[Biome][BlockType]);
		}
		Line.append("\n");
		f.Write(Line.c_str(), Line.length());
	}
}





