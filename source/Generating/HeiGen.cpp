
// HeiGen.cpp

// Implements the various terrain height generators

#include "Globals.h"
#include "HeiGen.h"
#include "../LinearUpscale.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHeiGenFlat:

void cHeiGenFlat::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	for (int i = 0; i < ARRAYCOUNT(a_HeightMap); i++)
	{
		a_HeightMap[i] = m_Height;
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHeiGenCache:

cHeiGenCache::cHeiGenCache(cTerrainHeightGen * a_HeiGenToCache, int a_CacheSize) :
	m_HeiGenToCache(a_HeiGenToCache),
	m_CacheSize(a_CacheSize),
	m_CacheOrder(new int[a_CacheSize]),
	m_CacheData(new sCacheData[a_CacheSize]),
	m_NumHits(0),
	m_NumMisses(0),
	m_TotalChain(0)
{
	for (int i = 0; i < m_CacheSize; i++)
	{
		m_CacheOrder[i] = i;
		m_CacheData[i].m_ChunkX = 0x7fffffff;
		m_CacheData[i].m_ChunkZ = 0x7fffffff;
	}
}





cHeiGenCache::~cHeiGenCache()
{
	delete[] m_CacheData;
	delete[] m_CacheOrder;
}





void cHeiGenCache::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	/*
	if (((m_NumHits + m_NumMisses) % 1024) == 10)
	{
		LOGD("HeiGenCache: %d hits, %d misses, saved %.2f %%", m_NumHits, m_NumMisses, 100.0 * m_NumHits / (m_NumHits + m_NumMisses));
		LOGD("HeiGenCache: Avg cache chain length: %.2f", (float)m_TotalChain / m_NumHits);
	}
	//*/
	
	for (int i = 0; i < m_CacheSize; i++)
	{
		if (
			(m_CacheData[m_CacheOrder[i]].m_ChunkX != a_ChunkX) ||
			(m_CacheData[m_CacheOrder[i]].m_ChunkZ != a_ChunkZ)
		)
		{
			continue;
		}
		// Found it in the cache
		int Idx = m_CacheOrder[i];
		
		// Move to front:
		for (int j = i; j > 0; j--)
		{
			m_CacheOrder[j] = m_CacheOrder[j - 1];
		}
		m_CacheOrder[0] = Idx;
		
		// Use the cached data:
		memcpy(a_HeightMap, m_CacheData[Idx].m_HeightMap, sizeof(a_HeightMap));
		
		m_NumHits++;
		m_TotalChain += i;
		return;
	}  // for i - cache
	
	// Not in the cache:
	m_NumMisses++;
	m_HeiGenToCache->GenHeightMap(a_ChunkX, a_ChunkZ, a_HeightMap);
	
	// Insert it as the first item in the MRU order:
	int Idx = m_CacheOrder[m_CacheSize - 1];
	for (int i = m_CacheSize - 1; i > 0; i--)
	{
		m_CacheOrder[i] = m_CacheOrder[i - 1];
	}  // for i - m_CacheOrder[]
	m_CacheOrder[0] = Idx;
	memcpy(m_CacheData[Idx].m_HeightMap, a_HeightMap, sizeof(a_HeightMap));
	m_CacheData[Idx].m_ChunkX = a_ChunkX;
	m_CacheData[Idx].m_ChunkZ = a_ChunkZ;
}





bool cHeiGenCache::GetHeightAt(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelZ, HEIGHTTYPE & a_Height)
{
	for (int i = 0; i < m_CacheSize; i++)
	{
		if ((m_CacheData[i].m_ChunkX == a_ChunkX) && (m_CacheData[i].m_ChunkZ == a_ChunkZ))
		{
			a_Height = cChunkDef::GetHeight(m_CacheData[i].m_HeightMap, a_RelX, a_RelZ);
			return true;
		}
	}  // for i - m_CacheData[]
	return false;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHeiGenClassic:

cHeiGenClassic::cHeiGenClassic(int a_Seed, float a_HeightFreq1, float a_HeightAmp1, float a_HeightFreq2, float a_HeightAmp2, float a_HeightFreq3, float a_HeightAmp3) :
	m_Seed(a_Seed),
	m_Noise(a_Seed),
	m_HeightFreq1(a_HeightFreq1),
	m_HeightAmp1 (a_HeightAmp1),
	m_HeightFreq2(a_HeightFreq2),
	m_HeightAmp2 (a_HeightAmp2),
	m_HeightFreq3(a_HeightFreq3),
	m_HeightAmp3 (a_HeightAmp3)
{
	// Nothing needed yet
}





float cHeiGenClassic::GetNoise(float x, float y)
{
	float oct1 = m_Noise.CubicNoise2D(x * m_HeightFreq1, y * m_HeightFreq1) * m_HeightAmp1;
	float oct2 = m_Noise.CubicNoise2D(x * m_HeightFreq2, y * m_HeightFreq2) * m_HeightAmp2;
	float oct3 = m_Noise.CubicNoise2D(x * m_HeightFreq3, y * m_HeightFreq3) * m_HeightAmp3;

	float height = m_Noise.CubicNoise2D(x * 0.1f, y * 0.1f ) * 2;

	float flatness = ((m_Noise.CubicNoise2D(x * 0.5f, y * 0.5f) + 1.f) * 0.5f) * 1.1f; // 0 ... 1.5
	flatness *= flatness * flatness;

	return (oct1 + oct2 + oct3) * flatness + height;
}





void cHeiGenClassic::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	for (int z = 0; z < cChunkDef::Width; z++) 
	{
		const float zz = (float)(a_ChunkZ * cChunkDef::Width + z);
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			const float xx = (float)(a_ChunkX * cChunkDef::Width + x);
			
			int hei = 64 + (int)(GetNoise(xx * 0.05f, zz * 0.05f) * 16);
			if (hei < 10)
			{
				hei = 10;
			}
			if (hei > 250)
			{
				hei = 250;
			}
			cChunkDef::SetHeight(a_HeightMap, x , z, hei);
		}  // for x
	}  // for z
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHeiGenBiomal:

const cHeiGenBiomal::sGenParam cHeiGenBiomal::m_GenParam[biNumBiomes] =
{
	/*                        Fast-changing | Middle-changing | Slow-changing |*/
	/* Biome               |  Freq1  | Amp1 | Freq2   | Amp2  | Freq3  | Amp3 | BaseHeight */
	/* biOcean            */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},
	/* biPlains           */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  68},
	/* biDesert           */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  68},
	/* biExtremeHills     */ { 0.2f,   4.0f,  0.05f,    20.0f,  0.01f,   16.0f, 100},
	/* biForest           */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},
	/* biTaiga            */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},
	/* biSwampland        */ { 0.1f,   1.1f,  0.05f,     1.5f,  0.02f,    2.5f,  61.5},
	/* biRiver            */ { 0.2f,   0.1f,  0.05f,     0.1f,  0.01f,    0.1f,  56},
	/* biNether           */ { 0.1f,   0.0f,  0.01f,     0.0f,  0.01f,    0.0f,   0},  // Unused, but must be here due to indexing
	/* biSky              */ { 0.1f,   0.0f,  0.01f,     0.0f,  0.01f,    0.0f,   0},  // Unused, but must be here due to indexing
	/* biFrozenOcean      */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},
	/* biFrozenRiver      */ { 0.2f,   0.1f,  0.05f,     0.1f,  0.01f,    0.1f,  56},
	/* biIcePlains        */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  68},
	/* biIceMountains     */ { 0.2f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  80},
	/* biMushroomIsland   */ { 0.1f,   2.0f,  0.05f,     8.0f,  0.01f,    6.0f,  80},
	/* biMushroomShore    */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  64},
	/* biBeach            */ { 0.1f,   0.5f,  0.05f,     1.0f,  0.01f,    1.0f,  64},
	/* biDesertHills      */ { 0.2f,   2.0f,  0.05f,     5.0f,  0.01f,    4.0f,  75},
	/* biForestHills      */ { 0.2f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  80},
	/* biTaigaHills       */ { 0.2f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  80},
	/* biExtremeHillsEdge */ { 0.2f,   3.0f,  0.05f,    16.0f,  0.01f,   12.0f,  80},
	/* biJungle           */ { 0.1f,   3.0f,  0.05f,     6.0f,  0.01f,    6.0f,  70},
	/* biJungleHills      */ { 0.2f,   3.0f,  0.05f,    12.0f,  0.01f,   10.0f,  80},
} ;





void cHeiGenBiomal::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	// Generate a 3x3 chunk area of biomes around this chunk:
	BiomeNeighbors Biomes;
	for (int z = -1; z <= 1; z++)
	{
		for (int x = -1; x <= 1; x++)
		{
			m_BiomeGen.GenBiomes(a_ChunkX + x, a_ChunkZ + z, Biomes[x + 1][z + 1]);
		}  // for x
	}  // for z
	
	/*
	_X 2013_04_22:
	There's no point in precalculating the entire perlin noise arrays, too many values are calculated uselessly,
	resulting in speed DEcrease.
	*/
	
	//*
	// Linearly interpolate 4x4 blocks of heightmap:
	// Must be done on a floating point datatype, else the results are ugly!
	const int STEPZ = 4;  // Must be a divisor of 16
	const int STEPX = 4;  // Must be a divisor of 16
	NOISE_DATATYPE Height[17 * 17];
	for (int z = 0; z < 17; z += STEPZ)
	{
		for (int x = 0; x < 17; x += STEPX)
		{
			Height[x + 17 * z] = GetHeightAt(x, z, a_ChunkX, a_ChunkZ, Biomes);
		}
	}
	ArrayLinearUpscale2DInPlace(Height, 17, 17, STEPX, STEPZ);
	
	// Copy into the heightmap
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			cChunkDef::SetHeight(a_HeightMap, x, z, (int)Height[x + 17 * z]);
		}
	}
	//*/
	
	/*
	// For each height, go through neighboring biomes and add up their idea of height:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			cChunkDef::SetHeight(a_HeightMap, x, z, GetHeightAt(x, z, a_ChunkX, a_ChunkZ, Biomes));
		}  // for x
	}
	//*/
}





NOISE_DATATYPE cHeiGenBiomal::GetHeightAt(int a_RelX, int a_RelZ, int a_ChunkX, int a_ChunkZ, const cHeiGenBiomal::BiomeNeighbors & a_BiomeNeighbors)
{
	// Sum up how many biomes of each type there are in the neighborhood:
	int BiomeCounts[biNumBiomes];
	memset(BiomeCounts, 0, sizeof(BiomeCounts));
	int Sum = 0;
	for (int z = -8; z <= 8; z++)
	{
		int FinalZ = a_RelZ + z + cChunkDef::Width;
		int IdxZ = FinalZ / cChunkDef::Width;
		int ModZ = FinalZ % cChunkDef::Width;
		int WeightZ = 9 - abs(z);
		for (int x = -8; x <= 8; x++)
		{
			int FinalX = a_RelX + x + cChunkDef::Width;
			int IdxX = FinalX / cChunkDef::Width;
			int ModX = FinalX % cChunkDef::Width;
			EMCSBiome Biome = cChunkDef::GetBiome(a_BiomeNeighbors[IdxX][IdxZ], ModX, ModZ);
			if ((Biome < 0) || (Biome >= ARRAYCOUNT(BiomeCounts)))
			{
				continue;
			}
			int WeightX = 9 - abs(x);
			BiomeCounts[Biome] += WeightX + WeightZ;
			Sum += WeightX + WeightZ;
		}  // for x
	}  // for z
	
	// For each biome type that has a nonzero count, calc its height and add it:
	if (Sum > 0)
	{
		NOISE_DATATYPE Height = 0;
		int BlockX = a_ChunkX * cChunkDef::Width + a_RelX;
		int BlockZ = a_ChunkZ * cChunkDef::Width + a_RelZ;
		for (int i = 0; i < ARRAYCOUNT(BiomeCounts); i++)
		{
			if (BiomeCounts[i] == 0)
			{
				continue;
			}
			NOISE_DATATYPE oct1 = m_Noise.CubicNoise2D(BlockX * m_GenParam[i].m_HeightFreq1, BlockZ * m_GenParam[i].m_HeightFreq1) * m_GenParam[i].m_HeightAmp1;
			NOISE_DATATYPE oct2 = m_Noise.CubicNoise2D(BlockX * m_GenParam[i].m_HeightFreq2, BlockZ * m_GenParam[i].m_HeightFreq2) * m_GenParam[i].m_HeightAmp2;
			NOISE_DATATYPE oct3 = m_Noise.CubicNoise2D(BlockX * m_GenParam[i].m_HeightFreq3, BlockZ * m_GenParam[i].m_HeightFreq3) * m_GenParam[i].m_HeightAmp3;
			Height += BiomeCounts[i] * (m_GenParam[i].m_BaseHeight + oct1 + oct2 + oct3);
		}
		NOISE_DATATYPE res = Height / Sum;
		return std::min((NOISE_DATATYPE)250, std::max(res, (NOISE_DATATYPE)5));
	}
	
	// No known biome around? Weird. Return a bogus value:
	ASSERT(!"cHeiGenBiomal: Biome sum failed, no known biome around");
	return 5;
}





