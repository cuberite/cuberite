
// HeiGen.cpp

// Implements the various terrain height generators

#include "Globals.h"
#include "HeiGen.h"
#include "../LinearUpscale.h"
#include "../IniFile.h"
#include "DistortedHeightmap.h"
#include "EndGen.h"
#include "Noise3DGenerator.h"
#include "ProtIntGen.h"





////////////////////////////////////////////////////////////////////////////////
// cHeiGenSteppy:

class cHeiGenSteppy:
	public cTerrainHeightGen
{
public:
	cHeiGenSteppy(int a_Seed) :
		m_Seed(a_Seed)
	{
		m_Gen =
			std::make_shared<cProtIntGenWeightAvg<16, 1, 0>>(
			std::make_shared<cProtIntGenSmooth>             (a_Seed + 1,
			std::make_shared<cProtIntGenZoom>               (a_Seed + 2,
			std::make_shared<cProtIntGenSmooth>             (a_Seed + 3,
			std::make_shared<cProtIntGenZoom>               (a_Seed + 4,
			std::make_shared<cProtIntGenAddRnd>             (a_Seed + 5, 1,
			std::make_shared<cProtIntGenSmooth>             (a_Seed + 6,
			std::make_shared<cProtIntGenZoom>               (a_Seed + 7,
			std::make_shared<cProtIntGenRndBetween>         (a_Seed + 8, 60,
			std::make_shared<cProtIntGenAddRnd>             (a_Seed + 9, 1,
			std::make_shared<cProtIntGenSmooth>             (a_Seed + 1,
			std::make_shared<cProtIntGenZoom>               (a_Seed + 2,
			std::make_shared<cProtIntGenRndBetween>         (a_Seed + 3, 60,
			std::make_shared<cProtIntGenSmooth>             (a_Seed + 4,
			std::make_shared<cProtIntGenZoom>               (a_Seed + 5,
			std::make_shared<cProtIntGenRndBetween>         (a_Seed + 6, 60,
			std::make_shared<cProtIntGenRndChoice>          (a_Seed + 7, 10, 50, 50,
			std::make_shared<cProtIntGenSmooth>             (a_Seed + 8,
			std::make_shared<cProtIntGenZoom>               (a_Seed + 9,
			std::make_shared<cProtIntGenRndChoice>          (a_Seed + 1, 10, 50, 50,
			std::make_shared<cProtIntGenAddRnd>             (a_Seed + 2, 2,
			std::make_shared<cProtIntGenZoom>               (a_Seed + 3,
			std::make_shared<cProtIntGenZoom>               (a_Seed + 4,
			std::make_shared<cProtIntGenChoice>             (a_Seed + 5, 10)
		)))))))))))))))))))))));
	}

	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override
	{
		int heights[cChunkDef::Width * cChunkDef::Width];
		m_Gen->GetInts(a_ChunkX * cChunkDef::Width, a_ChunkZ * cChunkDef::Width, static_cast<size_t>(cChunkDef::Width), static_cast<size_t>(cChunkDef::Width), heights);
		for (size_t i = 0; i < ARRAYCOUNT(heights); i++)
		{
			a_HeightMap[i] = static_cast<HEIGHTTYPE>(std::max(std::min(60 + heights[i], cChunkDef::Height - 60), 40));
		}
	}

protected:
	int m_Seed;

	SharedPtr<cProtIntGen> m_Gen;
};





////////////////////////////////////////////////////////////////////////////////
// cHeiGenFlat:

void cHeiGenFlat::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	for (size_t i = 0; i < ARRAYCOUNT(a_HeightMap); i++)
	{
		a_HeightMap[i] = m_Height;
	}
}





void cHeiGenFlat::InitializeHeightGen(cIniFile & a_IniFile)
{
	m_Height = static_cast<HEIGHTTYPE>(a_IniFile.GetValueSetI("Generator", "FlatHeight", m_Height));
}





////////////////////////////////////////////////////////////////////////////////
// cHeiGenCache:

cHeiGenCache::cHeiGenCache(cTerrainHeightGenPtr a_HeiGenToCache, size_t a_CacheSize) :
	m_HeiGenToCache(a_HeiGenToCache),
	m_CacheSize(a_CacheSize),
	m_CacheOrder(new size_t[a_CacheSize]),
	m_CacheData(new sCacheData[a_CacheSize]),
	m_NumHits(0),
	m_NumMisses(0),
	m_TotalChain(0)
{
	for (size_t i = 0; i < m_CacheSize; i++)
	{
		m_CacheOrder[i] = i;
		m_CacheData[i].m_ChunkX = 0x7fffffff;
		m_CacheData[i].m_ChunkZ = 0x7fffffff;
	}
}





cHeiGenCache::~cHeiGenCache()
{
	delete[] m_CacheData;
	m_CacheData = nullptr;
	delete[] m_CacheOrder;
	m_CacheOrder = nullptr;
}





void cHeiGenCache::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	/*
	if (((m_NumHits + m_NumMisses) % 1024) == 10)
	{
		LOGD("HeiGenCache: %d hits, %d misses, saved %.2f %%", m_NumHits, m_NumMisses, 100.0 * m_NumHits / (m_NumHits + m_NumMisses));
		LOGD("HeiGenCache: Avg cache chain length: %.2f", static_cast<double>(m_TotalChain) / m_NumHits);
	}
	//*/
	
	for (size_t i = 0; i < m_CacheSize; i++)
	{
		if (
			(m_CacheData[m_CacheOrder[i]].m_ChunkX != a_ChunkX) ||
			(m_CacheData[m_CacheOrder[i]].m_ChunkZ != a_ChunkZ)
		)
		{
			continue;
		}
		// Found it in the cache
		auto Idx = m_CacheOrder[i];
		
		// Move to front:
		for (size_t j = i; j > 0; j--)
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
	auto Idx = m_CacheOrder[m_CacheSize - 1];
	for (auto i = m_CacheSize - 1; i > 0; i--)
	{
		m_CacheOrder[i] = m_CacheOrder[i - 1];
	}  // for i - m_CacheOrder[]
	m_CacheOrder[0] = Idx;
	memcpy(m_CacheData[Idx].m_HeightMap, a_HeightMap, sizeof(a_HeightMap));
	m_CacheData[Idx].m_ChunkX = a_ChunkX;
	m_CacheData[Idx].m_ChunkZ = a_ChunkZ;
}





HEIGHTTYPE cHeiGenCache::GetHeightAt(int a_BlockX, int a_BlockZ)
{
	// First try if the chunk is already in the cache:
	int chunkX, chunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, chunkX, chunkZ);
	HEIGHTTYPE res;
	if (GetHeightAt(chunkX, chunkZ, a_BlockX - chunkX * cChunkDef::Width, a_BlockZ - chunkZ * cChunkDef::Width, res))
	{
		return res;
	}

	// Chunk not in cache, generate the chunk and ask again:
	cChunkDef::HeightMap heightMap;
	GenHeightMap(chunkX, chunkZ, heightMap);
	return cChunkDef::GetHeight(heightMap, a_BlockX - chunkX * cChunkDef::Width, a_BlockZ - chunkZ * cChunkDef::Width);
}





bool cHeiGenCache::GetHeightAt(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelZ, HEIGHTTYPE & a_Height)
{
	for (size_t i = 0; i < m_CacheSize; i++)
	{
		if ((m_CacheData[i].m_ChunkX == a_ChunkX) && (m_CacheData[i].m_ChunkZ == a_ChunkZ))
		{
			a_Height = cChunkDef::GetHeight(m_CacheData[i].m_HeightMap, a_RelX, a_RelZ);
			return true;
		}
	}  // for i - m_CacheData[]
	return false;
}





////////////////////////////////////////////////////////////////////////////////
// cHeiGenMultiCache:

cHeiGenMultiCache::cHeiGenMultiCache(cTerrainHeightGenPtr a_HeiGenToCache, size_t a_SubCacheSize, size_t a_NumSubCaches):
	m_NumSubCaches(a_NumSubCaches)
{
	// Create the individual sub-caches:
	m_SubCaches.reserve(a_NumSubCaches);
	for (size_t i = 0; i < a_NumSubCaches; i++)
	{
		m_SubCaches.push_back(std::make_shared<cHeiGenCache>(a_HeiGenToCache, a_SubCacheSize));
	}
}





void cHeiGenMultiCache::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	// Get the subcache responsible for this chunk:
	const size_t cacheIdx = (static_cast<size_t>(a_ChunkX) + m_CoeffZ * static_cast<size_t>(a_ChunkZ)) % m_NumSubCaches;

	// Ask the subcache:
	m_SubCaches[cacheIdx]->GenHeightMap(a_ChunkX, a_ChunkZ, a_HeightMap);
}





HEIGHTTYPE cHeiGenMultiCache::GetHeightAt(int a_BlockX, int a_BlockZ)
{
	// First try if the chunk is already in the cache:
	int chunkX, chunkZ;
	cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, chunkX, chunkZ);
	HEIGHTTYPE res;
	if (GetHeightAt(chunkX, chunkZ, a_BlockX - chunkX * cChunkDef::Width, a_BlockZ - chunkZ * cChunkDef::Width, res))
	{
		return res;
	}

	// Chunk not in cache, generate the chunk and ask again:
	cChunkDef::HeightMap heightMap;
	GenHeightMap(chunkX, chunkZ, heightMap);
	return cChunkDef::GetHeight(heightMap, a_BlockX - chunkX * cChunkDef::Width, a_BlockZ - chunkZ * cChunkDef::Width);
}





bool cHeiGenMultiCache::GetHeightAt(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelZ, HEIGHTTYPE & a_Height)
{
	// Get the subcache responsible for this chunk:
	const size_t cacheIdx = (static_cast<size_t>(a_ChunkX) + m_CoeffZ * static_cast<size_t>(a_ChunkZ)) % m_NumSubCaches;

	// Ask the subcache:
	return m_SubCaches[cacheIdx]->GetHeightAt(a_ChunkX, a_ChunkZ, a_RelX, a_RelZ, a_Height);
}






////////////////////////////////////////////////////////////////////////////////
// cHeiGenClassic:

cHeiGenClassic::cHeiGenClassic(int a_Seed) :
	m_Seed(a_Seed),
	m_Noise(a_Seed),
	m_HeightFreq1(1.0f),
	m_HeightAmp1(1.0f),
	m_HeightFreq2(0.5f),
	m_HeightAmp2(0.5f),
	m_HeightFreq3(0.1f),
	m_HeightAmp3(0.1f)
{
}





float cHeiGenClassic::GetNoise(float x, float y)
{
	float oct1 = m_Noise.CubicNoise2D(x * m_HeightFreq1, y * m_HeightFreq1) * m_HeightAmp1;
	float oct2 = m_Noise.CubicNoise2D(x * m_HeightFreq2, y * m_HeightFreq2) * m_HeightAmp2;
	float oct3 = m_Noise.CubicNoise2D(x * m_HeightFreq3, y * m_HeightFreq3) * m_HeightAmp3;

	float height = m_Noise.CubicNoise2D(x * 0.1f, y * 0.1f) * 2;

	float flatness = ((m_Noise.CubicNoise2D(x * 0.5f, y * 0.5f) + 1.f) * 0.5f) * 1.1f;  // 0 ... 1.5
	flatness *= flatness * flatness;

	return (oct1 + oct2 + oct3) * flatness + height;
}





void cHeiGenClassic::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		const float zz = static_cast<float>(a_ChunkZ * cChunkDef::Width + z);
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			const float xx = static_cast<float>(a_ChunkX * cChunkDef::Width + x);
			
			HEIGHTTYPE hei = static_cast<HEIGHTTYPE>(Clamp(static_cast<int>(64 + (GetNoise(xx * 0.05f, zz * 0.05f) * 16)), 10, 250));
			cChunkDef::SetHeight(a_HeightMap, x, z, hei);
		}  // for x
	}  // for z
}





void cHeiGenClassic::InitializeHeightGen(cIniFile & a_IniFile)
{
	m_HeightFreq1 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "ClassicHeightFreq1", 0.1));
	m_HeightFreq2 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "ClassicHeightFreq2", 1.0));
	m_HeightFreq3 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "ClassicHeightFreq3", 2.0));
	m_HeightAmp1  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "ClassicHeightAmp1",  1.0));
	m_HeightAmp2  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "ClassicHeightAmp2",  0.5));
	m_HeightAmp3  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "ClassicHeightAmp3",  0.5));
}





////////////////////////////////////////////////////////////////////////////////
// cHeiGenMountains:

cHeiGenMountains::cHeiGenMountains(int a_Seed) :
	m_Seed(a_Seed),
	m_MountainNoise(a_Seed + 100),
	m_DitchNoise(a_Seed + 200),
	m_Perlin(a_Seed + 300)
{
}





void cHeiGenMountains::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	NOISE_DATATYPE StartX = static_cast<NOISE_DATATYPE>(a_ChunkX * cChunkDef::Width);
	NOISE_DATATYPE EndX   = static_cast<NOISE_DATATYPE>(a_ChunkX * cChunkDef::Width + cChunkDef::Width - 1);
	NOISE_DATATYPE StartZ = static_cast<NOISE_DATATYPE>(a_ChunkZ * cChunkDef::Width);
	NOISE_DATATYPE EndZ   = static_cast<NOISE_DATATYPE>(a_ChunkZ * cChunkDef::Width + cChunkDef::Width - 1);
	NOISE_DATATYPE Workspace[16 * 16];
	NOISE_DATATYPE MountainNoise[16 * 16];
	NOISE_DATATYPE DitchNoise[16 * 16];
	NOISE_DATATYPE PerlinNoise[16 * 16];
	m_MountainNoise.Generate2D(MountainNoise, 16, 16, StartX, EndX, StartZ, EndZ, Workspace);
	m_DitchNoise.Generate2D(DitchNoise, 16, 16, StartX, EndX, StartZ, EndZ, Workspace);
	m_Perlin.Generate2D(PerlinNoise, 16, 16, StartX, EndX, StartZ, EndZ, Workspace);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int IdxZ = z * cChunkDef::Width;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int idx = IdxZ + x;
			HEIGHTTYPE hei = static_cast<HEIGHTTYPE>(Clamp(100 - static_cast<int>((MountainNoise[idx] - DitchNoise[idx] + PerlinNoise[idx]) * 15), 10, 250));
			cChunkDef::SetHeight(a_HeightMap, x, z, hei);
		}  // for x
	}  // for z
}





void cHeiGenMountains::InitializeHeightGen(cIniFile & a_IniFile)
{
	// TODO: Read the params from an INI file
	m_MountainNoise.AddOctave(0.1f,  0.2f);
	m_MountainNoise.AddOctave(0.05f, 0.4f);
	m_MountainNoise.AddOctave(0.02f, 1.0f);
	m_DitchNoise.AddOctave(0.1f,  0.2f);
	m_DitchNoise.AddOctave(0.05f, 0.4f);
	m_DitchNoise.AddOctave(0.02f, 1.0f);

	m_Perlin.AddOctave(0.01f, 1.5f);
}





////////////////////////////////////////////////////////////////////////////////
// cHeiGenBiomal:

const cHeiGenBiomal::sGenParam cHeiGenBiomal::m_GenParam[256] =
{
	/*                        Fast-changing | Middle-changing | Slow-changing | */
	/* Biome               |  Freq1  | Amp1 | Freq2   | Amp2  | Freq3  | Amp3 | BaseHeight */
	/* biOcean               */ { 0.1f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  50},
	/* biPlains              */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  68},
	/* biDesert              */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  68},
	/* biExtremeHills        */ { 0.2f,   4.0f,  0.05f,    20.0f,  0.01f,   16.0f, 100},
	/* biForest              */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},
	/* biTaiga               */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},
	/* biSwampland           */ { 0.1f,   1.1f,  0.05f,     1.5f,  0.02f,    2.5f,  61.5},
	/* biRiver               */ { 0.2f,   0.1f,  0.05f,     0.1f,  0.01f,    0.1f,  56},
	/* biNether              */ { 0.1f,   0.0f,  0.01f,     0.0f,  0.01f,    0.0f,   0},  // Unused, but must be here due to indexing
	/* biSky                 */ { 0.1f,   0.0f,  0.01f,     0.0f,  0.01f,    0.0f,   0},  // Unused, but must be here due to indexing
	/* biFrozenOcean         */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},
	/* biFrozenRiver         */ { 0.2f,   0.1f,  0.05f,     0.1f,  0.01f,    0.1f,  56},
	/* biIcePlains           */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  68},
	/* biIceMountains        */ { 0.2f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  80},
	/* biMushroomIsland      */ { 0.1f,   2.0f,  0.05f,     8.0f,  0.01f,    6.0f,  80},
	/* biMushroomShore       */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  64},
	/* biBeach               */ { 0.1f,   0.5f,  0.05f,     1.0f,  0.01f,    1.0f,  64},
	/* biDesertHills         */ { 0.2f,   2.0f,  0.05f,     5.0f,  0.01f,    4.0f,  75},
	/* biForestHills         */ { 0.2f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  80},
	/* biTaigaHills          */ { 0.2f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  80},
	/* biExtremeHillsEdge    */ { 0.2f,   3.0f,  0.05f,    16.0f,  0.01f,   12.0f,  80},
	/* biJungle              */ { 0.1f,   3.0f,  0.05f,     6.0f,  0.01f,    6.0f,  70},
	/* biJungleHills         */ { 0.2f,   3.0f,  0.05f,    12.0f,  0.01f,   10.0f,  80},
	/* biJungleEdge          */ { 0.1f,   3.0f,  0.05f,     6.0f,  0.01f,    6.0f,  70},
	/* biDeepOcean           */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},
	/* biStoneBeach          */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},
	/* biColdBeach           */ { 0.1f,   0.5f,  0.05f,     1.0f,  0.01f,    1.0f,  64},
	/* biBirchForest         */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},
	/* biBirchForestHills    */ { 0.2f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  80},
	/* biRoofedForest        */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},
	/* biColdTaiga           */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},
	/* biColdTaigaHills      */ { 0.2f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  80},
	/* biMegaTaiga           */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},
	/* biMegaTaigaHills      */ { 0.2f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  80},
	/* biExtremeHillsPlus    */ { 0.2f,   4.0f,  0.05f,    20.0f,  0.01f,   16.0f, 120},
	/* biSavanna             */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  68},
	/* biSavannaPlateau      */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  80},
	/* biMesa                */ { 0.2f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  70},  // 165
	/* biMesaPlateauF        */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  80},
	/* biMesaPlateau         */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  80},
	
	// biomes 40 .. 128 are unused, 89 empty placeholders here:
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 40 .. 49
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 50 .. 59
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 60 .. 69
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 70 .. 79
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 80 .. 89
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 90 .. 99
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 100 .. 109
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 110 .. 119
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},                                           // 120 .. 128
	
	/* biSunflowerPlains      */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},  // 129
	/* biDesertM              */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},  // 130
	/* biExtremeHillsM        */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},  // 131
	/* biFlowerForest         */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},  // 132
	/* biTaigaM               */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},  // 133
	/* biSwamplandM           */ { 1.0f,   3.0f,  1.10f,     7.0f,  0.01f,   0.01f,  60},  // 134
	
	// Biomes 135 .. 139 unused, 5 empty placeholders here:
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 135 .. 139
	
	/* biIcePlainsSpikes      */ { 0.1f,   2.0f,  0.05f,    12.0f,  0.01f,   10.0f,  40},  // 140
	
	// Biomes 141 .. 148 unused, 8 empty placeholders here:
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 141 .. 148
	
	/* biJungleM              */ { 0.1f,   3.0f,  0.05f,     6.0f,  0.01f,    6.0f,  70},  // 149
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},                                               // 150
	/* biJungleEdgeM          */ { 0.1f,   3.0f,  0.05f,     6.0f,  0.01f,    6.0f,  70},  // 151
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},  // 152 .. 154
	/* biBirchForestM         */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},  // 155
	/* biBirchForestHillsM    */ { 0.2f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  80},  // 156
	/* biRoofedForestM        */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},  // 157
	/* biColdTaigaM           */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},  // 158
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0},                                               // 159
	/* biMegaSpruceTaiga      */ { 0.1f,   1.0f,  0.05f,     2.0f,  0.01f,    4.0f,  70},  // 160
	/* biMegaSpruceTaigaHills */ { 0.2f,   2.0f,  0.05f,    10.0f,  0.01f,    8.0f,  80},  // 161
	/* biExtremeHillsPlusM    */ { 0.2f,   4.0f,  0.05f,    20.0f,  0.01f,   16.0f, 120},  // 162
	/* biSavannaM             */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  68},  // 163
	/* biSavannaPlateauM      */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  80},  // 164
	/* biMesaBryce            */ { 0.2f,   2.0f,  0.1f,     30.0f,  0.01f,    8.0f,  80},
	/* biMesaPlateauFM        */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  80},  // 166
	/* biMesaPlateauM         */ { 0.1f,   1.0f,  0.05f,     1.5f,  0.01f,    4.0f,  80},  // 167
} ;





void cHeiGenBiomal::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	// Generate a 3x3 chunk area of biomes around this chunk:
	BiomeNeighbors Biomes;
	for (int z = -1; z <= 1; z++)
	{
		for (int x = -1; x <= 1; x++)
		{
			m_BiomeGen->GenBiomes(a_ChunkX + x, a_ChunkZ + z, Biomes[x + 1][z + 1]);
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
	LinearUpscale2DArrayInPlace<17, 17, STEPX, STEPZ>(Height);
	
	// Copy into the heightmap
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			cChunkDef::SetHeight(a_HeightMap, x, z, static_cast<HEIGHTTYPE>(Height[x + 17 * z]));
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





void cHeiGenBiomal::InitializeHeightGen(cIniFile & a_IniFile)
{
	// No user-settable params
}





NOISE_DATATYPE cHeiGenBiomal::GetHeightAt(int a_RelX, int a_RelZ, int a_ChunkX, int a_ChunkZ, const cHeiGenBiomal::BiomeNeighbors & a_BiomeNeighbors)
{
	// Sum up how many biomes of each type there are in the neighborhood:
	int BiomeCounts[256];
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
		for (size_t i = 0; i < ARRAYCOUNT(BiomeCounts); i++)
		{
			if (BiomeCounts[i] == 0)
			{
				continue;
			}
			
			/*
			// Sanity checks for biome parameters, enable them to check the biome param table in runtime (slow):
			ASSERT(m_GenParam[i].m_HeightFreq1 >= 0);
			ASSERT(m_GenParam[i].m_HeightFreq1 < 1000);
			ASSERT(m_GenParam[i].m_HeightFreq2 >= 0);
			ASSERT(m_GenParam[i].m_HeightFreq2 < 1000);
			ASSERT(m_GenParam[i].m_HeightFreq3 >= 0);
			ASSERT(m_GenParam[i].m_HeightFreq3 < 1000);
			*/
			
			NOISE_DATATYPE oct1 = m_Noise.CubicNoise2D(BlockX * m_GenParam[i].m_HeightFreq1, BlockZ * m_GenParam[i].m_HeightFreq1) * m_GenParam[i].m_HeightAmp1;
			NOISE_DATATYPE oct2 = m_Noise.CubicNoise2D(BlockX * m_GenParam[i].m_HeightFreq2, BlockZ * m_GenParam[i].m_HeightFreq2) * m_GenParam[i].m_HeightAmp2;
			NOISE_DATATYPE oct3 = m_Noise.CubicNoise2D(BlockX * m_GenParam[i].m_HeightFreq3, BlockZ * m_GenParam[i].m_HeightFreq3) * m_GenParam[i].m_HeightAmp3;
			Height += BiomeCounts[i] * (m_GenParam[i].m_BaseHeight + oct1 + oct2 + oct3);
		}
		NOISE_DATATYPE res = Height / Sum;
		return std::min(static_cast<NOISE_DATATYPE>(250), std::max(res, static_cast<NOISE_DATATYPE>(5)));
	}
	
	// No known biome around? Weird. Return a bogus value:
	ASSERT(!"cHeiGenBiomal: Biome sum failed, no known biome around");
	return 5;
}





////////////////////////////////////////////////////////////////////////////////
// cHeiGenMinMax:

class cHeiGenMinMax:
	public cTerrainHeightGen
{
	typedef cTerrainHeightGen super;

	/** Size of the averaging process, in columns (for each direction). Must be less than 16. */
	static const int AVERAGING_SIZE = 4;

public:
	cHeiGenMinMax(int a_Seed, cBiomeGenPtr a_BiomeGen):
		m_Noise(a_Seed),
		m_BiomeGen(a_BiomeGen),
		m_TotalWeight(0)
	{
		// Initialize the weights:
		for (int z = 0; z <= AVERAGING_SIZE * 2; z++)
		{
			for (int x = 0; x <= AVERAGING_SIZE * 2; x++)
			{
				m_Weights[z][x] = 1 + 2 * AVERAGING_SIZE - std::abs(x - AVERAGING_SIZE) - std::abs(z - AVERAGING_SIZE);
				m_TotalWeight += m_Weights[z][x];
			}
		}

		// Initialize the Perlin generator:
		m_Perlin.AddOctave(0.04f,   0.2f);
		m_Perlin.AddOctave(0.02f,   0.1f);
		m_Perlin.AddOctave(0.01f,   0.05f);
	}


	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
	{
		// Generate the biomes for the 3 * 3 neighbors:
		cChunkDef::BiomeMap neighborBiomes[3][3];
		for (int z = 0; z < 3; z++) for (int x = 0; x < 3; x++)
		{
			m_BiomeGen->GenBiomes(a_ChunkX + x - 1, a_ChunkZ + z - 1, neighborBiomes[z][x]);
		}

		// Get the min and max heights based on the biomes:
		double minHeight[cChunkDef::Width * cChunkDef::Width];
		double maxHeight[cChunkDef::Width * cChunkDef::Width];
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				// For each column, sum the min and max values of the neighborhood around it:
				double min = 0, max = 0;
				for (int relz = 0; relz <= AVERAGING_SIZE * 2; relz++)
				{
					int bz = z + 16 + relz - AVERAGING_SIZE;  // Biome Z coord relative to the neighborBiomes start
					int cz = bz / 16;                         // Chunk Z coord relative to the neighborBiomes start
					bz = bz % 16;                             // Biome Z coord relative to cz in neighborBiomes
					for (int relx = 0; relx <= AVERAGING_SIZE * 2; relx++)
					{
						int bx = x + 16 + relx - AVERAGING_SIZE;  // Biome X coord relative to the neighborBiomes start
						int cx = bx / 16;                         // Chunk X coord relative to the neighborBiomes start
						bx = bx % 16;                             // Biome X coord relative to cz in neighborBiomes

						// Get the biome's min and max heights:
						double bmin, bmax;
						getBiomeMinMax(cChunkDef::GetBiome(neighborBiomes[cz][cx], bx, bz), bmin, bmax);

						// Add them to the total, with the weight depending on their relative position to the column:
						min += bmin * m_Weights[relz][relx];
						max += bmax * m_Weights[relz][relx];
					}  // for relx
				}  // for relz
				minHeight[x + z * cChunkDef::Width] = min / m_TotalWeight;
				maxHeight[x + z * cChunkDef::Width] = max / m_TotalWeight;
			}  // for x
		}  // for z

		// Generate the base noise:
		NOISE_DATATYPE noise[cChunkDef::Width * cChunkDef::Width];
		NOISE_DATATYPE workspace[cChunkDef::Width * cChunkDef::Width];
		NOISE_DATATYPE startX = static_cast<float>(a_ChunkX * cChunkDef::Width);
		NOISE_DATATYPE endX = startX + cChunkDef::Width - 1;
		NOISE_DATATYPE startZ = static_cast<float>(a_ChunkZ * cChunkDef::Width);
		NOISE_DATATYPE endZ = startZ + cChunkDef::Width - 1;
		m_Perlin.Generate2D(noise, 16, 16, startX, endX, startZ, endZ, workspace);

		// Make the height by ranging the noise between min and max:
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				double min = minHeight[x + z * cChunkDef::Width];
				double max = maxHeight[x + z * cChunkDef::Width];
				double h = (max + min) / 2 + noise[x + z * cChunkDef::Width] * (max - min);
				cChunkDef::SetHeight(a_HeightMap, x, z, static_cast<HEIGHTTYPE>(h));
			}
		}
	}

	
	virtual void InitializeHeightGen(cIniFile & a_IniFile)
	{
		// No settings available
	}

protected:
	cNoise m_Noise;

	cPerlinNoise m_Perlin;

	/** The biome generator to query for the underlying biomes. */
	cBiomeGenPtr m_BiomeGen;

	/** Weights applied to each of the min / max values in the neighborhood of the currently evaluated column. */
	double m_Weights[AVERAGING_SIZE * 2 + 1][AVERAGING_SIZE * 2 + 1];

	/** Sum of all the m_Weights items. */
	double m_TotalWeight;


	/** Returns the minimum and maximum heights for the given biome. */
	void getBiomeMinMax(EMCSBiome a_Biome, double & a_Min, double & a_Max)
	{
		switch (a_Biome)
		{
			case biBeach:                a_Min = 61; a_Max = 64; break;
			case biBirchForest:          a_Min = 63; a_Max = 75; break;
			case biBirchForestHills:     a_Min = 63; a_Max = 90; break;
			case biBirchForestHillsM:    a_Min = 63; a_Max = 90; break;
			case biBirchForestM:         a_Min = 63; a_Max = 75; break;
			case biColdBeach:            a_Min = 61; a_Max = 64; break;
			case biColdTaiga:            a_Min = 63; a_Max = 75; break;
			case biColdTaigaHills:       a_Min = 63; a_Max = 90; break;
			case biColdTaigaM:           a_Min = 63; a_Max = 75; break;
			case biDeepOcean:            a_Min = 30; a_Max = 60; break;
			case biDesert:               a_Min = 63; a_Max = 70; break;
			case biDesertHills:          a_Min = 63; a_Max = 85; break;
			case biDesertM:              a_Min = 63; a_Max = 70; break;
			case biEnd:                  a_Min = 10; a_Max = 100; break;
			case biExtremeHills:         a_Min = 60; a_Max = 120; break;
			case biExtremeHillsEdge:     a_Min = 63; a_Max = 100; break;
			case biExtremeHillsM:        a_Min = 60; a_Max = 120; break;
			case biExtremeHillsPlus:     a_Min = 60; a_Max = 140; break;
			case biExtremeHillsPlusM:    a_Min = 60; a_Max = 140; break;
			case biFlowerForest:         a_Min = 63; a_Max = 75; break;
			case biForest:               a_Min = 63; a_Max = 75; break;
			case biForestHills:          a_Min = 63; a_Max = 90; break;
			case biFrozenOcean:          a_Min = 45; a_Max = 64; break;
			case biFrozenRiver:          a_Min = 60; a_Max = 62; break;
			case biIceMountains:         a_Min = 63; a_Max = 90; break;
			case biIcePlains:            a_Min = 63; a_Max = 70; break;
			case biIcePlainsSpikes:      a_Min = 60; a_Max = 70; break;
			case biJungle:               a_Min = 60; a_Max = 80; break;
			case biJungleEdge:           a_Min = 62; a_Max = 75; break;
			case biJungleEdgeM:          a_Min = 62; a_Max = 75; break;
			case biJungleHills:          a_Min = 60; a_Max = 90; break;
			case biJungleM:              a_Min = 60; a_Max = 75; break;
			case biMegaSpruceTaiga:      a_Min = 63; a_Max = 75; break;
			case biMegaSpruceTaigaHills: a_Min = 63; a_Max = 90; break;
			case biMegaTaiga:            a_Min = 63; a_Max = 75; break;
			case biMegaTaigaHills:       a_Min = 63; a_Max = 90; break;
			case biMesa:                 a_Min = 63; a_Max = 90; break;
			case biMesaBryce:            a_Min = 60; a_Max = 67; break;
			case biMesaPlateau:          a_Min = 75; a_Max = 85; break;
			case biMesaPlateauF:         a_Min = 80; a_Max = 90; break;
			case biMesaPlateauFM:        a_Min = 80; a_Max = 90; break;
			case biMesaPlateauM:         a_Min = 75; a_Max = 85; break;
			case biMushroomIsland:       a_Min = 63; a_Max = 90; break;
			case biMushroomShore:        a_Min = 60; a_Max = 75; break;
			case biNether:               a_Min = 10; a_Max = 100; break;
			case biOcean:                a_Min = 45; a_Max = 64; break;
			case biPlains:               a_Min = 63; a_Max = 70; break;
			case biRiver:                a_Min = 60; a_Max = 62; break;
			case biRoofedForest:         a_Min = 63; a_Max = 75; break;
			case biRoofedForestM:        a_Min = 63; a_Max = 75; break;
			case biSavanna:              a_Min = 63; a_Max = 75; break;
			case biSavannaM:             a_Min = 63; a_Max = 80; break;
			case biSavannaPlateau:       a_Min = 75; a_Max = 100; break;
			case biSavannaPlateauM:      a_Min = 80; a_Max = 160; break;
			case biStoneBeach:           a_Min = 60; a_Max = 64; break;
			case biSunflowerPlains:      a_Min = 63; a_Max = 70; break;
			case biSwampland:            a_Min = 60; a_Max = 67; break;
			case biSwamplandM:           a_Min = 61; a_Max = 67; break;
			case biTaiga:                a_Min = 63; a_Max = 75; break;
			case biTaigaHills:           a_Min = 63; a_Max = 90; break;
			case biTaigaM:               a_Min = 63; a_Max = 80; break;
			case biInvalidBiome:
			case biNumBiomes:
			case biVariant:
			case biNumVariantBiomes:
			{
				ASSERT(!"Unknown biome");
				a_Min = 10;
				a_Max = 10;
				break;
			}
		}
	}
};





////////////////////////////////////////////////////////////////////////////////
// cTerrainHeightGen:

cTerrainHeightGenPtr cTerrainHeightGen::CreateHeightGen(cIniFile & a_IniFile, cBiomeGenPtr a_BiomeGen, int a_Seed, bool & a_CacheOffByDefault)
{
	AString HeightGenName = a_IniFile.GetValueSet("Generator", "HeightGen", "");
	if (HeightGenName.empty())
	{
		LOGWARN("[Generator] HeightGen value not set in world.ini, using \"Biomal\".");
		HeightGenName = "Biomal";
	}
	
	a_CacheOffByDefault = false;
	cTerrainHeightGenPtr res;
	if (NoCaseCompare(HeightGenName, "Flat") == 0)
	{
		res = std::make_shared<cHeiGenFlat>();
		a_CacheOffByDefault = true;  // We're generating faster than a cache would retrieve data
	}
	else if (NoCaseCompare(HeightGenName, "classic") == 0)
	{
		res = std::make_shared<cHeiGenClassic>(a_Seed);
	}
	else if (NoCaseCompare(HeightGenName, "DistortedHeightmap") == 0)
	{
		// Not a heightmap-based generator, but it used to be accessible via HeightGen, so we need to skip making the default out of it
		// Return an empty pointer, the caller will create the proper generator:
		return cTerrainHeightGenPtr();
	}
	else if (NoCaseCompare(HeightGenName, "End") == 0)
	{
		// Not a heightmap-based generator, but it used to be accessible via HeightGen, so we need to skip making the default out of it
		// Return an empty pointer, the caller will create the proper generator:
		return cTerrainHeightGenPtr();
	}
	else if (NoCaseCompare(HeightGenName, "MinMax") == 0)
	{
		res = std::make_shared<cHeiGenMinMax>(a_Seed, a_BiomeGen);
	}
	else if (NoCaseCompare(HeightGenName, "Mountains") == 0)
	{
		res = std::make_shared<cHeiGenMountains>(a_Seed);
	}
	else if (NoCaseCompare(HeightGenName, "BiomalNoise3D") == 0)
	{
		// Not a heightmap-based generator, but it used to be accessible via HeightGen, so we need to skip making the default out of it
		// Return an empty pointer, the caller will create the proper generator:
		return cTerrainHeightGenPtr();
	}
	else if (NoCaseCompare(HeightGenName, "Steppy") == 0)
	{
		res = std::make_shared<cHeiGenSteppy>(a_Seed);
	}
	else if (NoCaseCompare(HeightGenName, "Noise3D") == 0)
	{
		// Not a heightmap-based generator, but it used to be accessible via HeightGen, so we need to skip making the default out of it
		// Return an empty pointer, the caller will create the proper generator:
		return cTerrainHeightGenPtr();
	}
	else if (NoCaseCompare(HeightGenName, "Biomal") == 0)
	{
		res = std::make_shared<cHeiGenBiomal>(a_Seed, a_BiomeGen);

		/*
		// Performance-testing:
		LOGINFO("Measuring performance of cHeiGenBiomal...");
		clock_t BeginTick = clock();
		for (int x = 0; x < 500; x++)
		{
			cChunkDef::HeightMap Heights;
			res->GenHeightMap(x * 5, x * 5, Heights);
		}
		clock_t Duration = clock() - BeginTick;
		LOGINFO("HeightGen for 500 chunks took %d ticks (%.02f sec)", Duration, (double)Duration / CLOCKS_PER_SEC);
		//*/
	}
	else
	{
		// No match found, force-set the default and retry
		LOGWARN("Unknown HeightGen \"%s\", using \"Biomal\" instead.", HeightGenName.c_str());
		a_IniFile.SetValue("Generator", "HeightGen", "Biomal");
		return CreateHeightGen(a_IniFile, a_BiomeGen, a_Seed, a_CacheOffByDefault);
	}
	
	// Read the settings:
	res->InitializeHeightGen(a_IniFile);

	return res;
}





