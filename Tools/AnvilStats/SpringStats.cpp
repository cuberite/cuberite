
// SpringStats.cpp

// Implements the cSpringStats class representing a cCallback descendant that collects statistics on lava and water springs

#include "Globals.h"
#include "SpringStats.h"





////////////////////////////////////////////////////////////////////////////////
// cSpringStats::cStats

cSpringStats::cStats::cStats(void) :
	m_TotalChunks(0)
{
	memset(m_LavaSprings,  0, sizeof(m_LavaSprings));
	memset(m_WaterSprings, 0, sizeof(m_WaterSprings));
}





void cSpringStats::cStats::Add(const cSpringStats::cStats & a_Other)
{
	m_TotalChunks += a_Other.m_TotalChunks;
	for (int Biome = 0; Biome < 256; Biome++)
	{
		for (int Height = 0; Height < 256; Height++)
		{
			m_LavaSprings[Biome][Height]  += a_Other.m_LavaSprings[Biome][Height];
			m_WaterSprings[Biome][Height] += a_Other.m_WaterSprings[Biome][Height];
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cSpringStats:

cSpringStats::cSpringStats(void) :
	m_AreBiomesValid(false)
{
}





bool cSpringStats::OnNewChunk(int a_ChunkX, int a_ChunkZ)
{
	memset(m_BlockTypes, 0, sizeof(m_BlockTypes));
	m_AreBiomesValid = false;
	return false;
}





bool cSpringStats::OnBiomes(const unsigned char * a_BiomeData)
{
	memcpy(m_Biomes, a_BiomeData, sizeof(m_Biomes));
	m_AreBiomesValid = true;
	return false;
}





bool cSpringStats::OnSection(
	unsigned char a_Y,
	const BLOCKTYPE * a_BlockTypes,
	const NIBBLETYPE * a_BlockAdditional,
	const NIBBLETYPE * a_BlockMeta,
	const NIBBLETYPE * a_BlockLight,
	const NIBBLETYPE * a_BlockSkyLight
)
{
	memcpy(m_BlockTypes + ((int)a_Y) * 16 * 16 * 16,     a_BlockTypes, 16 * 16 * 16);
	memcpy(m_BlockMetas + ((int)a_Y) * 16 * 16 * 16 / 2, a_BlockMeta,  16 * 16 * 16 / 2);
	return false;
}





bool cSpringStats::OnSectionsFinished(void)
{
	if (!m_AreBiomesValid)
	{
		return true;
	}

	// Calc the spring stats:
	for (int y = 1; y < 255; y++)
	{
		int BaseY = y * 16 * 16;
		for (int z = 1; z < 15; z++)
		{
			int Base = BaseY + z * 16;
			for (int x = 1; x < 15; x++)
			{
				if (cChunkDef::GetNibble(m_BlockMetas, x, y, z) != 0)
				{
					// Not a source block
					continue;
				}
				switch (m_BlockTypes[Base + x])
				{
					case E_BLOCK_WATER:
					case E_BLOCK_STATIONARY_WATER:
					{
						TestSpring(x, y, z, m_Stats.m_WaterSprings);
						break;
					}
					case E_BLOCK_LAVA:
					case E_BLOCK_STATIONARY_LAVA:
					{
						TestSpring(x, y, z, m_Stats.m_LavaSprings);
						break;
					}
				}  // switch (BlockType)
			}  // for x
		}  // for z
	}  // for y
	m_Stats.m_TotalChunks += 1;
	return true;
}





void cSpringStats::TestSpring(int a_RelX, int a_RelY, int a_RelZ, cSpringStats::cStats::SpringStats & a_Stats)
{
	static const struct
	{
		int x, y, z;
	} Coords[] =
	{
		{-1,  0,  0},
		{ 1,  0,  0},
		{ 0, -1,  0},
		{ 0,  1,  0},
		{ 0,  0, -1},
		{ 0,  0,  1},
	} ;
	bool HasFluidNextToIt = false;
	for (int i = 0; i < ARRAYCOUNT(Coords); i++)
	{
		switch (cChunkDef::GetBlock(m_BlockTypes, a_RelX + Coords[i].x, a_RelY + Coords[i].y, a_RelZ + Coords[i].z))
		{
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER:
			case E_BLOCK_LAVA:
			case E_BLOCK_STATIONARY_LAVA:
			{
				if (cChunkDef::GetNibble(m_BlockMetas, a_RelX + Coords[i].x, a_RelY + Coords[i].y, a_RelZ + Coords[i].z) == 0)
				{
					// There is another source block next to this, so this is not a spring
					return;
				}
				HasFluidNextToIt = true;
			}
		}  // switch (BlockType)
	}  // for i - Coords[]

	if (!HasFluidNextToIt)
	{
		// Surrounded by solids on all sides, this is probably not a spring,
		// but rather a bedrocked lake or something similar. Dont want.
		return;
	}

	// No source blocks next to the specified block, so it is a spring. Add it to stats:
	a_Stats[a_RelY][((unsigned char *)m_Biomes)[a_RelX + 16 * a_RelZ]] += 1;
}





////////////////////////////////////////////////////////////////////////////////
// cSpringStatsFactory:

cSpringStatsFactory::~cSpringStatsFactory()
{
	LOG("cSpringStats:");
	LOG("  Joining results...");
	JoinResults();
	LOG("  Total %llu chunks went through", m_CombinedStats.m_TotalChunks);

	// Save statistics:
	LOG("  Saving statistics into files:");
	LOG("    Springs.xls");
	SaveTotals("Springs.xls");
	LOG("    BiomeWaterSprings.xls");
	SaveStatistics(m_CombinedStats.m_WaterSprings, "BiomeWaterSprings.xls");
	LOG("    BiomeLavaSprings.xls");
	SaveStatistics(m_CombinedStats.m_LavaSprings, "BiomeLavaSprings.xls");
}





void cSpringStatsFactory::JoinResults(void)
{
	for (cCallbacks::iterator itr = m_Callbacks.begin(), end = m_Callbacks.end(); itr != end; ++itr)
	{
		m_CombinedStats.Add(((cSpringStats *)(*itr))->GetStats());
	}  // for itr - m_Callbacks[]
}





void cSpringStatsFactory::SaveTotals(const AString & a_FileName)
{
	cFile f(a_FileName, cFile::fmWrite);
	if (!f.IsOpen())
	{
		LOG("Cannot open file \"%s\" for writing!", a_FileName.c_str());
		return;
	}
	f.Printf("Height\tWater\tLava\n");
	for (int Height = 0; Height < 256; Height++)
	{
		UInt64 TotalW = 0;
		UInt64 TotalL = 0;
		for (int Biome = 0; Biome < 256; Biome++)
		{
			TotalW += m_CombinedStats.m_WaterSprings[Height][Biome];
			TotalL += m_CombinedStats.m_LavaSprings[Height][Biome];
		}
		f.Printf("%d\t%llu\t%llu\n", Height, TotalW, TotalL);
	}
	f.Printf("\n# Chunks\t%llu", m_CombinedStats.m_TotalChunks);
}





void cSpringStatsFactory::SaveStatistics(const cSpringStats::cStats::SpringStats & a_Stats, const AString & a_FileName)
{
	cFile f(a_FileName, cFile::fmWrite);
	if (!f.IsOpen())
	{
		LOG("Cannot open file \"%s\" for writing!", a_FileName.c_str());
		return;
	}
	for (int Height = 0; Height < 256; Height++)
	{
		AString Line;
		Line.reserve(2000);
		Printf(Line, "%d\t", Height);
		for (int Biome = 0; Biome < 256; Biome++)
		{
			AppendPrintf(Line, "%llu\t", a_Stats[Height][Biome]);
		}
		Line.append("\n");
		f.Write(Line.c_str(), Line.size());
	}
}
