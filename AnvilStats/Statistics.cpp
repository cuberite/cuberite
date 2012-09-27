
// Statistics.cpp

// Implements the various statistics-collecting classes

#include "Globals.h"
#include "Statistics.h"





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
				unsigned char Biome = m_BiomeData[x + 16 * z];  // Cannot use cChunkDef, different data size
				unsigned char BlockType = cChunkDef::GetBlock(a_BlockTypes, x, y, z);
				if (BlockType == 12)
				{
					__asm nop;
				}
				m_BlockCounts[Biome][BlockType] += 1;
			}
		}
	}
	m_BlockNumChunks += m_IsFirstSectionInChunk ? 1 : 0;
	m_IsFirstSectionInChunk = false;
	
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStatisticsFactory:

cStatisticsFactory::~cStatisticsFactory()
{
	// TODO: Join the results together and export
	LOG("cStatistics:");
	LOG("  Joining results...");
	JoinResults();
	LOG("  Total %d chunks went through", m_TotalChunks);
	LOG("  Biomes processed for %d chunks", m_BiomeNumChunks);
	LOG("  BlockIDs processed for %d chunks", m_BlockNumChunks);
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
		LOG("Cannot write to file Biomes.txt. Statistics not written.");
		return;
	}
	for (int i = 0; i <= 255; i++)
	{
		AString Line;
		Printf(Line, "%d\t%d\n", i, m_BiomeCounts[i]);
		f.Write(Line.c_str(), Line.length());
	}
}





void cStatisticsFactory::SaveBlockTypes(void)
{
	cFile f;
	if (!f.Open("BlockTypes.xls", cFile::fmWrite))
	{
		LOG("Cannot write to file Biomes.txt. Statistics not written.");
		return;
	}
	for (int i = 0; i <= 255; i++)
	{
		int Count = 0;
		for (int Biome = 0; Biome <= 255; ++Biome)
		{
			Count += m_BlockCounts[Biome][i];
		}
		AString Line;
		Printf(Line, "%d\t%d\n", i, Count);
		f.Write(Line.c_str(), Line.length());
	}
	// TODO
}





void cStatisticsFactory::SaveBiomeBlockTypes(void)
{
	LOG("Not implemented yet!");
	// TODO
}





