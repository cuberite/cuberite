
// BioGen.cpp

// Implements the various biome generators

#include "Globals.h"
#include "BioGen.h"
#include <iostream>
#include "IntGen.h"
#include "ProtIntGen.h"
#include "../IniFile.h"
#include "../LinearUpscale.h"





////////////////////////////////////////////////////////////////////////////////
// cBioGenConstant:

void cBioGenConstant::GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	UNUSED(a_ChunkCoords);
	for (size_t i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		a_BiomeMap[i] = m_Biome;
	}
}





void cBioGenConstant::InitializeBiomeGen(cIniFile & a_IniFile)
{
	AString defaultBiome;
	switch (StringToDimension(a_IniFile.GetValue("General", "Dimension", "Overworld")))
	{
		case dimOverworld: defaultBiome = "Plains";    break;
		case dimNether:    defaultBiome = "Nether";    break;
		case dimEnd:       defaultBiome = "End";       break;
		case dimNotSet:    defaultBiome = "Swampland"; break;
	}
	AString Biome = a_IniFile.GetValueSet("Generator", "ConstantBiome", defaultBiome);
	m_Biome = StringToBiome(Biome);
	if (m_Biome == biInvalidBiome)
	{
		LOGWARN("[Generator]::ConstantBiome value \"%s\" not recognized, using \"Plains\".", Biome.c_str());
		m_Biome = biPlains;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cBioGenCache:

cBioGenCache::cBioGenCache(cBiomeGenPtr a_BioGenToCache, size_t a_CacheSize) :
	m_BioGenToCache(std::move(a_BioGenToCache)),
	m_CacheSize(a_CacheSize),
	m_NumHits(0),
	m_NumMisses(0),
	m_TotalChain(0)
{
	m_CacheData.resize(m_CacheSize);
	m_CacheOrder.resize(m_CacheSize);
	for (size_t i = 0; i < m_CacheSize; i++)
	{
		m_CacheOrder[i] = i;
	}
}





void cBioGenCache::GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	if (((m_NumHits + m_NumMisses) % 1024) == 10)
	{
		// LOGD("BioGenCache: %u hits, %u misses, saved %.2f %%", static_cast<unsigned>(m_NumHits), static_cast<unsigned>(m_NumMisses), 100.0 * m_NumHits / (m_NumHits + m_NumMisses));
		// LOGD("BioGenCache: Avg cache chain length: %.2f", static_cast<double>(m_TotalChain) / m_NumHits);
	}

	for (size_t i = 0; i < m_CacheSize; i++)
	{
		if (m_CacheData[m_CacheOrder[i]].m_Coords != a_ChunkCoords)
		{
			continue;
		}
		// Found it in the cache
		size_t Idx = m_CacheOrder[i];

		// Move to front:
		for (size_t j = i; j > 0; j--)
		{
			m_CacheOrder[j] = m_CacheOrder[j - 1];
		}
		m_CacheOrder[0] = Idx;

		// Use the cached data:
		memcpy(a_BiomeMap, m_CacheData[Idx].m_BiomeMap, sizeof(a_BiomeMap));

		m_NumHits++;
		m_TotalChain += i;
		return;
	}  // for i - cache

	// Not in the cache:
	m_NumMisses++;
	m_BioGenToCache->GenBiomes(a_ChunkCoords, a_BiomeMap);

	// Insert it as the first item in the MRU order:
	size_t Idx = m_CacheOrder[m_CacheSize - 1];
	for (size_t i = m_CacheSize - 1; i > 0; i--)
	{
		m_CacheOrder[i] = m_CacheOrder[i - 1];
	}  // for i - m_CacheOrder[]
	m_CacheOrder[0] = Idx;
	memcpy(m_CacheData[Idx].m_BiomeMap, a_BiomeMap, sizeof(a_BiomeMap));
	m_CacheData[Idx].m_Coords = a_ChunkCoords;
}





void cBioGenCache::InitializeBiomeGen(cIniFile & a_IniFile)
{
	Super::InitializeBiomeGen(a_IniFile);
	m_BioGenToCache->InitializeBiomeGen(a_IniFile);
}




////////////////////////////////////////////////////////////////////////////////
// cBioGenMulticache:

cBioGenMulticache::cBioGenMulticache(const cBiomeGenPtr & a_BioGenToCache, size_t a_SubCacheSize, size_t a_NumSubCaches) :
	m_NumSubCaches(a_NumSubCaches)
{
	m_Caches.reserve(a_NumSubCaches);
	for (size_t i = 0; i < a_NumSubCaches; i++)
	{
		m_Caches.push_back(cBiomeGenPtr(new cBioGenCache(a_BioGenToCache, a_SubCacheSize)));
	}
}





void cBioGenMulticache::GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	const size_t coefficient = 3;
	const size_t cacheIdx = (static_cast<size_t>(a_ChunkCoords.m_ChunkX) + coefficient * static_cast<size_t>(a_ChunkCoords.m_ChunkZ)) % m_NumSubCaches;

	m_Caches[cacheIdx]->GenBiomes(a_ChunkCoords, a_BiomeMap);
}





void cBioGenMulticache::InitializeBiomeGen(cIniFile & a_IniFile)
{
	for (const auto & itr : m_Caches)
	{
		itr->InitializeBiomeGen(a_IniFile);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cBiomeGenList:

void cBiomeGenList::InitializeBiomes(const AString & a_Biomes)
{
	AStringVector Split = StringSplitAndTrim(a_Biomes, ",");

	// Convert each string in the list into biome:
	for (AStringVector::const_iterator itr = Split.begin(); itr != Split.end(); ++itr)
	{
		AStringVector Split2 = StringSplit(*itr, ":");
		if (Split2.size() < 1)
		{
			continue;
		}
		int Count = 1;
		if (Split2.size() >= 2)
		{
			if (!StringToInteger(Split2[1], Count))
			{
				LOGWARNING("Cannot decode biome count: \"%s\"; using 1.", Split2[1].c_str());
				Count = 1;
			}
		}
		EMCSBiome Biome = StringToBiome(Split2[0]);
		if (Biome != biInvalidBiome)
		{
			for (int i = 0; i < Count; i++)
			{
				m_Biomes.push_back(Biome);
			}
		}
		else
		{
			LOGWARNING("Cannot decode biome name: \"%s\"; skipping", Split2[0].c_str());
		}
	}  // for itr - Split[]
	if (!m_Biomes.empty())
	{
		m_BiomesCount = static_cast<int>(m_Biomes.size());
		return;
	}

	// There were no biomes, add default biomes:
	static EMCSBiome Biomes[] =
	{
		biOcean,
		biPlains,
		biDesert,
		biExtremeHills,
		biForest,
		biTaiga,
		biSwampland,
		biRiver,
		biFrozenOcean,
		biFrozenRiver,
		biIcePlains,
		biIceMountains,
		biMushroomIsland,
		biMushroomShore,
		biBeach,
		biDesertHills,
		biForestHills,
		biTaigaHills,
		biExtremeHillsEdge,
		biJungle,
		biJungleHills,
	} ;
	m_Biomes.reserve(ARRAYCOUNT(Biomes));
	for (size_t i = 0; i < ARRAYCOUNT(Biomes); i++)
	{
		m_Biomes.push_back(Biomes[i]);
	}
	m_BiomesCount = static_cast<int>(m_Biomes.size());
}





////////////////////////////////////////////////////////////////////////////////
// cBioGenCheckerboard:

void cBioGenCheckerboard::GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int Base = (cChunkDef::Width * a_ChunkCoords.m_ChunkZ + z) / m_BiomeSize;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Add = cChunkDef::Width * a_ChunkCoords.m_ChunkX + x;
			size_t BiomeIdx = static_cast<size_t>((((Base + Add / m_BiomeSize) % m_BiomesCount) + m_BiomesCount) % m_BiomesCount);  // Need to add and modulo twice because of negative numbers
			a_BiomeMap[x + cChunkDef::Width * z] = m_Biomes[BiomeIdx];
		}
	}
}





void cBioGenCheckerboard::InitializeBiomeGen(cIniFile & a_IniFile)
{
	Super::InitializeBiomeGen(a_IniFile);
	AString Biomes = a_IniFile.GetValueSet ("Generator", "CheckerBoardBiomes",    "");
	m_BiomeSize    = a_IniFile.GetValueSetI("Generator", "CheckerboardBiomeSize", 64);
	m_BiomeSize = (m_BiomeSize < 8) ? 8 : m_BiomeSize;
	InitializeBiomes(Biomes);
}





////////////////////////////////////////////////////////////////////////////////
// cBioGenVoronoi :

void cBioGenVoronoi::GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	int BaseZ = cChunkDef::Width * a_ChunkCoords.m_ChunkZ;
	int BaseX = cChunkDef::Width * a_ChunkCoords.m_ChunkX;
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int AbsoluteZ = BaseZ + z;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int VoronoiCellValue = m_Voronoi.GetValueAt(BaseX + x, AbsoluteZ) / 8;
			cChunkDef::SetBiome(a_BiomeMap, x, z, m_Biomes[static_cast<size_t>(VoronoiCellValue % m_BiomesCount)]);
		}  // for x
	}  // for z
}





void cBioGenVoronoi::InitializeBiomeGen(cIniFile & a_IniFile)
{
	Super::InitializeBiomeGen(a_IniFile);
	int CellSize     = a_IniFile.GetValueSetI("Generator", "VoronoiCellSize", 128);
	int JitterSize   = a_IniFile.GetValueSetI("Generator", "VoronoiJitterSize", CellSize);
	int OddRowOffset = a_IniFile.GetValueSetI("Generator", "VoronoiOddRowOffset", 0);
	m_Voronoi.SetCellSize(CellSize);
	m_Voronoi.SetJitterSize(JitterSize);
	m_Voronoi.SetOddRowOffset(OddRowOffset);
	InitializeBiomes(a_IniFile.GetValueSet ("Generator", "VoronoiBiomes",   ""));
}





////////////////////////////////////////////////////////////////////////////////
// cBioGenDistortedVoronoi:

void cBioGenDistortedVoronoi::GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	int BaseZ = cChunkDef::Width * a_ChunkCoords.m_ChunkZ;
	int BaseX = cChunkDef::Width * a_ChunkCoords.m_ChunkX;

	// Distortions for linear interpolation:
	int DistortX[cChunkDef::Width + 1][cChunkDef::Width + 1];
	int DistortZ[cChunkDef::Width + 1][cChunkDef::Width + 1];
	for (int x = 0; x <= 4; x++) for (int z = 0; z <= 4; z++)
	{
		Distort(BaseX + x * 4, BaseZ + z * 4, DistortX[4 * x][4 * z], DistortZ[4 * x][4 * z]);
	}

	LinearUpscale2DArrayInPlace<cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4>(&DistortX[0][0]);
	LinearUpscale2DArrayInPlace<cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4>(&DistortZ[0][0]);

	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int VoronoiCellValue = m_Voronoi.GetValueAt(DistortX[x][z], DistortZ[x][z]) / 8;
			cChunkDef::SetBiome(a_BiomeMap, x, z, m_Biomes[static_cast<size_t>(VoronoiCellValue % m_BiomesCount)]);
		}  // for x
	}  // for z
}





void cBioGenDistortedVoronoi::InitializeBiomeGen(cIniFile & a_IniFile)
{
	Super::InitializeBiomeGen(a_IniFile);
	m_CellSize = a_IniFile.GetValueSetI("Generator", "DistortedVoronoiCellSize", 96);
	m_Voronoi.SetCellSize(m_CellSize);
	InitializeBiomes(a_IniFile.GetValueSet("Generator", "DistortedVoronoiBiomes", ""));
}





void cBioGenDistortedVoronoi::Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ)
{
	double NoiseX = m_Noise.CubicNoise3D(static_cast<float>(a_BlockX / m_CellSize), static_cast<float>(a_BlockZ / m_CellSize), 1000);
	NoiseX += 0.5 * m_Noise.CubicNoise3D(2 *  static_cast<float>(a_BlockX / m_CellSize), 2 *  static_cast<float>(a_BlockZ / m_CellSize), 2000);
	NoiseX += 0.08 * m_Noise.CubicNoise3D(16 * static_cast<float>(a_BlockX / m_CellSize), 16 * static_cast<float>(a_BlockZ / m_CellSize), 3000);
	double NoiseZ = m_Noise.CubicNoise3D(static_cast<float>(a_BlockX / m_CellSize), static_cast<float>(a_BlockZ / m_CellSize), 4000);
	NoiseZ += 0.5 * m_Noise.CubicNoise3D(2 *  static_cast<float>(a_BlockX / m_CellSize), 2 *  static_cast<float>(a_BlockZ / m_CellSize), 5000);
	NoiseZ += 0.08 * m_Noise.CubicNoise3D(16 * static_cast<float>(a_BlockX / m_CellSize), 16 * static_cast<float>(a_BlockZ / m_CellSize), 6000);

	a_DistortedX = a_BlockX + static_cast<int>(m_CellSize * 0.5 * NoiseX);
	a_DistortedZ = a_BlockZ + static_cast<int>(m_CellSize * 0.5 * NoiseZ);
}





////////////////////////////////////////////////////////////////////////////////
// cBioGenMultiStepMap :

cBioGenMultiStepMap::cBioGenMultiStepMap(int a_Seed) :
	m_Noise1(a_Seed + 1000),
	m_Noise2(a_Seed + 2000),
	m_Noise3(a_Seed + 3000),
	m_Noise4(a_Seed + 4000),
	m_Noise5(a_Seed + 5000),
	m_Noise6(a_Seed + 6000),
	m_Seed(a_Seed),
	m_OceanCellSize(384),
	m_MushroomIslandSize(64),
	m_RiverCellSize(384),
	m_RiverWidthThreshold(0.125),
	m_LandBiomesSize(1024)
{
}





void cBioGenMultiStepMap::InitializeBiomeGen(cIniFile & a_IniFile)
{
	m_OceanCellSize       =        a_IniFile.GetValueSetI("Generator", "MultiStepMapOceanCellSize",      m_OceanCellSize);
	m_MushroomIslandSize  =        a_IniFile.GetValueSetI("Generator", "MultiStepMapMushroomIslandSize", m_MushroomIslandSize);
	m_RiverCellSize       =        a_IniFile.GetValueSetI("Generator", "MultiStepMapRiverCellSize",      m_RiverCellSize);
	m_RiverWidthThreshold =        a_IniFile.GetValueSetF("Generator", "MultiStepMapRiverWidth",         m_RiverWidthThreshold);
	m_LandBiomesSize      = static_cast<float>(a_IniFile.GetValueSetI("Generator", "MultiStepMapLandBiomeSize",      static_cast<int>(m_LandBiomesSize)));
}





void cBioGenMultiStepMap::GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	DecideOceanLandMushroom(a_ChunkCoords, a_BiomeMap);
	AddRivers(a_ChunkCoords, a_BiomeMap);
	ApplyTemperatureHumidity(a_ChunkCoords, a_BiomeMap);
}





void cBioGenMultiStepMap::DecideOceanLandMushroom(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	// Distorted Voronoi over 3 biomes, with mushroom having only a special occurence.

	// Prepare a distortion lookup table, by distorting a 5x5 area and using that as 1:4 zoom (linear interpolate):
	int BaseZ = cChunkDef::Width * a_ChunkCoords.m_ChunkZ;
	int BaseX = cChunkDef::Width * a_ChunkCoords.m_ChunkX;
	int DistortX[cChunkDef::Width + 1][cChunkDef::Width + 1];
	int DistortZ[cChunkDef::Width + 1][cChunkDef::Width + 1];
	int DistortSize = m_OceanCellSize / 2;
	for (int x = 0; x <= 4; x++) for (int z = 0; z <= 4; z++)
	{
		Distort(BaseX + x * 4, BaseZ + z * 4, DistortX[4 * x][4 * z], DistortZ[4 * x][4 * z], DistortSize);
	}
	LinearUpscale2DArrayInPlace<cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4>(&DistortX[0][0]);
	LinearUpscale2DArrayInPlace<cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4>(&DistortZ[0][0]);

	// Prepare a 9x9 area of neighboring cell seeds
	// (assuming that 7x7 cell area is larger than a chunk being generated)
	const int NEIGHBORHOOD_SIZE = 4;  // How many seeds in each direction to check
	int CellX = BaseX / m_OceanCellSize;
	int CellZ = BaseZ / m_OceanCellSize;
	int SeedX[2 * NEIGHBORHOOD_SIZE + 1][2 * NEIGHBORHOOD_SIZE + 1];
	int SeedZ[2 * NEIGHBORHOOD_SIZE + 1][2 * NEIGHBORHOOD_SIZE + 1];
	EMCSBiome SeedV[2 * NEIGHBORHOOD_SIZE + 1][2 * NEIGHBORHOOD_SIZE + 1];
	for (int xc = 0; xc < 2 * NEIGHBORHOOD_SIZE + 1; xc++)
	{
		int RealCellX = xc + CellX - NEIGHBORHOOD_SIZE;
		int CellBlockX = RealCellX * m_OceanCellSize;
		for (int zc = 0; zc < 2 * NEIGHBORHOOD_SIZE + 1; zc++)
		{
			int RealCellZ = zc + CellZ - NEIGHBORHOOD_SIZE;
			int CellBlockZ = RealCellZ * m_OceanCellSize;
			int OffsetX = (m_Noise2.IntNoise3DInt(RealCellX, 16 * RealCellX + 32 * RealCellZ, RealCellZ) / 8) % m_OceanCellSize;
			int OffsetZ = (m_Noise4.IntNoise3DInt(RealCellX, 32 * RealCellX - 16 * RealCellZ, RealCellZ) / 8) % m_OceanCellSize;
			SeedX[xc][zc] = CellBlockX + OffsetX;
			SeedZ[xc][zc] = CellBlockZ + OffsetZ;
			SeedV[xc][zc] = (((m_Noise6.IntNoise3DInt(RealCellX, RealCellX - RealCellZ + 1000, RealCellZ) / 11) % 256) > 90) ? biOcean : (biInvalidBiome);
		}  // for z
	}  // for x

	for (int xc = 1; xc < 2 * NEIGHBORHOOD_SIZE; xc++) for (int zc = 1; zc < 2 * NEIGHBORHOOD_SIZE; zc++)
	{
		if (
			(SeedV[xc    ][zc]     == biOcean) &&
			(SeedV[xc - 1][zc]     == biOcean) &&
			(SeedV[xc + 1][zc]     == biOcean) &&
			(SeedV[xc    ][zc - 1] == biOcean) &&
			(SeedV[xc    ][zc + 1] == biOcean) &&
			(SeedV[xc - 1][zc - 1] == biOcean) &&
			(SeedV[xc + 1][zc - 1] == biOcean) &&
			(SeedV[xc - 1][zc + 1] == biOcean) &&
			(SeedV[xc + 1][zc + 1] == biOcean)
		)
		{
			SeedV[xc][zc] = biMushroomIsland;
		}
	}

	// For each column find the nearest distorted cell and use its value as the biome:
	int MushroomOceanThreshold = m_OceanCellSize * m_OceanCellSize * m_MushroomIslandSize / 1024;
	int MushroomShoreThreshold = m_OceanCellSize * m_OceanCellSize * m_MushroomIslandSize / 2048;
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int AbsoluteZ = DistortZ[x][z];
			int AbsoluteX = DistortX[x][z];
			int MinDist = m_OceanCellSize * m_OceanCellSize * 16;  // There has to be a cell closer than this
			EMCSBiome Biome = biPlains;
			// Find the nearest cell seed:
			for (int xs = 1; xs < 2 * NEIGHBORHOOD_SIZE; xs++) for (int zs = 1; zs < 2 * NEIGHBORHOOD_SIZE; zs++)
			{
				int Dist = (SeedX[xs][zs] - AbsoluteX) * (SeedX[xs][zs] - AbsoluteX) + (SeedZ[xs][zs] - AbsoluteZ) * (SeedZ[xs][zs] - AbsoluteZ);
				if (Dist >= MinDist)
				{
					continue;
				}
				MinDist = Dist;
				Biome = SeedV[xs][zs];
				// Shrink mushroom biome and add a shore:
				if (Biome == biMushroomIsland)
				{
					if (Dist > MushroomOceanThreshold)
					{
						Biome = biOcean;
					}
					else if (Dist > MushroomShoreThreshold)
					{
						Biome = biMushroomShore;
					}
				}
			}  // for zs, xs

			cChunkDef::SetBiome(a_BiomeMap, x, z, Biome);
		}  // for x
	}  // for z
}





void cBioGenMultiStepMap::AddRivers(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		float NoiseCoordZ = static_cast<float>(a_ChunkCoords.m_ChunkZ * cChunkDef::Width + z) / m_RiverCellSize;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			if (cChunkDef::GetBiome(a_BiomeMap, x, z) != biInvalidBiome)
			{
				// Biome already set, skip this column
				continue;
			}

			float NoiseCoordX = static_cast<float>(a_ChunkCoords.m_ChunkX * cChunkDef::Width + x) / m_RiverCellSize;

			double Noise = m_Noise1.CubicNoise2D(    NoiseCoordX,     NoiseCoordZ);
			Noise += 0.5 * m_Noise3.CubicNoise2D(2 * NoiseCoordX, 2 * NoiseCoordZ);
			Noise += 0.1 * m_Noise5.CubicNoise2D(8 * NoiseCoordX, 8 * NoiseCoordZ);

			if ((Noise > 0) && (Noise < m_RiverWidthThreshold))
			{
				cChunkDef::SetBiome(a_BiomeMap, x, z, biRiver);
			}
		}  // for x
	}  // for z
}





void cBioGenMultiStepMap::ApplyTemperatureHumidity(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	IntMap TemperatureMap;
	IntMap HumidityMap;
	BuildTemperatureHumidityMaps(a_ChunkCoords, TemperatureMap, HumidityMap);

	FreezeWaterBiomes(a_BiomeMap, TemperatureMap);
	DecideLandBiomes(a_BiomeMap, TemperatureMap, HumidityMap);
}





void cBioGenMultiStepMap::Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ, int a_CellSize)
{
	double NoiseX = m_Noise3.CubicNoise2D(     static_cast<float>(a_BlockX / a_CellSize),      static_cast<float>(a_BlockZ / a_CellSize));
	NoiseX += 0.5 * m_Noise2.CubicNoise2D(2 *  static_cast<float>(a_BlockX / a_CellSize), 2 *  static_cast<float>(a_BlockZ / a_CellSize));
	NoiseX += 0.1 * m_Noise1.CubicNoise2D(16 * static_cast<float>(a_BlockX / a_CellSize), 16 * static_cast<float>(a_BlockZ / a_CellSize));
	double NoiseZ = m_Noise6.CubicNoise2D(     static_cast<float>(a_BlockX / a_CellSize),      static_cast<float>(a_BlockZ / a_CellSize));
	NoiseZ += 0.5 * m_Noise5.CubicNoise2D(2 *  static_cast<float>(a_BlockX / a_CellSize), 2 *  static_cast<float>(a_BlockZ / a_CellSize));
	NoiseZ += 0.1 * m_Noise4.CubicNoise2D(16 * static_cast<float>(a_BlockX / a_CellSize), 16 * static_cast<float>(a_BlockZ / a_CellSize));

	a_DistortedX = a_BlockX + static_cast<int>(a_CellSize * 0.5 * NoiseX);
	a_DistortedZ = a_BlockZ + static_cast<int>(a_CellSize * 0.5 * NoiseZ);
}





void cBioGenMultiStepMap::BuildTemperatureHumidityMaps(cChunkCoords a_ChunkCoords, IntMap & a_TemperatureMap, IntMap & a_HumidityMap)
{
	// Linear interpolation over 8x8 blocks; use double for better precision:
	DblMap TemperatureMap;
	DblMap HumidityMap;
	for (int z = 0; z < 17; z += 8)
	{
		float NoiseCoordZ = static_cast<float>(a_ChunkCoords.m_ChunkZ * cChunkDef::Width + z) / m_LandBiomesSize;
		for (int x = 0; x < 17; x += 8)
		{
			float NoiseCoordX = static_cast<float>(a_ChunkCoords.m_ChunkX * cChunkDef::Width + x) / m_LandBiomesSize;

			double NoiseT = m_Noise1.CubicNoise2D(    NoiseCoordX,     NoiseCoordZ);
			NoiseT += 0.5 * m_Noise2.CubicNoise2D(2 * NoiseCoordX, 2 * NoiseCoordZ);
			NoiseT += 0.1 * m_Noise3.CubicNoise2D(8 * NoiseCoordX, 8 * NoiseCoordZ);
			TemperatureMap[x + 17 * z] = NoiseT;

			double NoiseH = m_Noise4.CubicNoise2D(    NoiseCoordX,     NoiseCoordZ);
			NoiseH += 0.5 * m_Noise5.CubicNoise2D(2 * NoiseCoordX, 2 * NoiseCoordZ);
			NoiseH += 0.1 * m_Noise6.CubicNoise2D(8 * NoiseCoordX, 8 * NoiseCoordZ);
			HumidityMap[x + 17 * z] = NoiseH;
		}  // for x
	}  // for z
	LinearUpscale2DArrayInPlace<17, 17, 8, 8>(TemperatureMap);
	LinearUpscale2DArrayInPlace<17, 17, 8, 8>(HumidityMap);

	// Re-map into integral values in [0 .. 255] range:
	for (size_t idx = 0; idx < ARRAYCOUNT(a_TemperatureMap); idx++)
	{
		a_TemperatureMap[idx] = Clamp(static_cast<int>(128 + TemperatureMap[idx] * 128), 0, 255);
		a_HumidityMap[idx]    = Clamp(static_cast<int>(128 + HumidityMap[idx]    * 128), 0, 255);
	}
}





void cBioGenMultiStepMap::DecideLandBiomes(cChunkDef::BiomeMap & a_BiomeMap, const IntMap & a_TemperatureMap, const IntMap & a_HumidityMap)
{
	static const EMCSBiome BiomeMap[] =
	{
		//       0         1         2               3               4               5               6         7         8         9         10              11              12              13              14             15
		/*  0 */ biTundra, biTundra, biTundra,       biTundra,       biPlains,       biPlains,       biPlains, biPlains, biPlains, biPlains, biDesert,       biDesert,      biDesert,       biDesert,       biDesert,      biDesert,
		/*  1 */ biTundra, biTundra, biTundra,       biTundra,       biPlains,       biPlains,       biPlains, biPlains, biPlains, biPlains, biDesert,       biDesert,      biDesert,       biDesert,       biDesert,      biDesert,
		/*  2 */ biTundra, biTundra, biTundra,       biTundra,       biPlains,       biExtremeHills, biPlains, biPlains, biPlains, biPlains, biDesert,       biDesert,      biDesertHills,  biDesertHills,  biDesert,      biDesert,
		/*  3 */ biTundra, biTundra, biTundra,       biTundra,       biExtremeHills, biExtremeHills, biPlains, biPlains, biPlains, biPlains, biDesert,       biDesert,      biDesertHills,  biDesertHills,  biDesert,      biDesert,
		/*  4 */ biTundra, biTundra, biIceMountains, biIceMountains, biExtremeHills, biExtremeHills, biPlains, biPlains, biPlains, biPlains, biForestHills,  biForestHills, biExtremeHills, biExtremeHills, biDesertHills, biDesert,
		/*  5 */ biTundra, biTundra, biIceMountains, biIceMountains, biExtremeHills, biExtremeHills, biPlains, biPlains, biPlains, biPlains, biForestHills,  biForestHills, biExtremeHills, biExtremeHills, biDesertHills, biDesert,
		/*  6 */ biTundra, biTundra, biIceMountains, biIceMountains, biForestHills,  biForestHills,  biForest, biForest, biForest, biForest, biForest,       biForestHills, biExtremeHills, biExtremeHills, biPlains,      biPlains,
		/*  7 */ biTundra, biTundra, biIceMountains, biIceMountains, biForestHills,  biForestHills,  biForest, biForest, biForest, biForest, biForest,       biForestHills, biExtremeHills, biExtremeHills, biPlains,      biPlains,
		/*  8 */ biTundra, biTundra, biTaiga,        biTaiga,        biForest,       biForest,       biForest, biForest, biForest, biForest, biForest,       biForestHills, biExtremeHills, biExtremeHills, biPlains,      biPlains,
		/*  9 */ biTundra, biTundra, biTaiga,        biTaiga,        biForest,       biForest,       biForest, biForest, biForest, biForest, biForest,       biForestHills, biExtremeHills, biExtremeHills, biPlains,      biPlains,
		/* 10 */ biTaiga,  biTaiga,  biTaiga,        biIceMountains, biForestHills,  biForestHills,  biForest, biForest, biForest, biForest, biJungle,       biJungle,      biSwampland,    biSwampland,    biSwampland,   biSwampland,
		/* 11 */ biTaiga,  biTaiga,  biIceMountains, biIceMountains, biExtremeHills, biForestHills,  biForest, biForest, biForest, biForest, biJungle,       biJungle,      biSwampland,    biSwampland,    biSwampland,   biSwampland,
		/* 12 */ biTaiga,  biTaiga,  biIceMountains, biIceMountains, biExtremeHills, biJungleHills,  biJungle, biJungle, biJungle, biJungle, biJungle,       biJungle,      biSwampland,    biSwampland,    biSwampland,   biSwampland,
		/* 13 */ biTaiga,  biTaiga,  biTaiga,        biIceMountains, biJungleHills,  biJungleHills,  biJungle, biJungle, biJungle, biJungle, biJungle,       biJungle,      biSwampland,    biSwampland,    biSwampland,   biSwampland,
		/* 14 */ biTaiga,  biTaiga,  biTaiga,        biTaiga,        biJungle,       biJungle,       biJungle, biJungle, biJungle, biJungle, biJungle,       biJungle,      biSwampland,    biSwampland,    biSwampland,   biSwampland,
		/* 15 */ biTaiga,  biTaiga,  biTaiga,        biTaiga,        biJungle,       biJungle,       biJungle, biJungle, biJungle, biJungle, biJungle,       biJungle,      biSwampland,    biSwampland,    biSwampland,   biSwampland,
	} ;
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int idxZ = 17 * z;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			if (cChunkDef::GetBiome(a_BiomeMap, x, z) != biInvalidBiome)
			{
				// Already set before
				continue;
			}
			int idx = idxZ + x;
			int Temperature = a_TemperatureMap[idx] / 16;  // -> [0..15] range
			int Humidity    = a_HumidityMap[idx]    / 16;  // -> [0..15] range
			cChunkDef::SetBiome(a_BiomeMap, x, z, BiomeMap[Temperature + 16 * Humidity]);
		}  // for x
	}  // for z
}





void cBioGenMultiStepMap::FreezeWaterBiomes(cChunkDef::BiomeMap & a_BiomeMap, const IntMap & a_TemperatureMap)
{
	int idx = 0;
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++, idx++)
		{
			if (a_TemperatureMap[idx] > 4 * 16)
			{
				// Not frozen
				continue;
			}
			switch (cChunkDef::GetBiome(a_BiomeMap, x, z))
			{
				case biRiver: cChunkDef::SetBiome(a_BiomeMap, x, z, biFrozenRiver); break;
				case biOcean: cChunkDef::SetBiome(a_BiomeMap, x, z, biFrozenOcean); break;
				default: break;
			}
		}  // for x
		idx += 1;
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cBioGenTwoLevel:

cBioGenTwoLevel::cBioGenTwoLevel(int a_Seed) :
	m_VoronoiLarge(a_Seed + 1000),
	m_VoronoiSmall(a_Seed + 2000),
	m_Noise1(a_Seed + 5001),
	m_Noise2(a_Seed + 5002),
	m_Noise3(a_Seed + 5003),
	m_Noise4(a_Seed + 5004),
	m_Noise5(a_Seed + 5005),
	m_Noise6(a_Seed + 5006),
	m_FreqX1(0.0),
	m_AmpX1(0.0),
	m_FreqX2(0.0),
	m_AmpX2(0.0),
	m_FreqX3(0.0),
	m_AmpX3(0.0),
	m_FreqZ1(0.0),
	m_AmpZ1(0.0),
	m_FreqZ2(0.0),
	m_AmpZ2(0.0),
	m_FreqZ3(0.0),
	m_AmpZ3(0.0)
{
}





void cBioGenTwoLevel::GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	int BaseZ = cChunkDef::Width * a_ChunkCoords.m_ChunkZ;
	int BaseX = cChunkDef::Width * a_ChunkCoords.m_ChunkX;

	// Distortions for linear interpolation:
	int DistortX[cChunkDef::Width + 1][cChunkDef::Width + 1];
	int DistortZ[cChunkDef::Width + 1][cChunkDef::Width + 1];
	for (int x = 0; x <= 4; x++) for (int z = 0; z <= 4; z++)
	{
		float BlockX = static_cast<float>(BaseX + x * 4);
		float BlockZ = static_cast<float>(BaseZ + z * 4);
		double NoiseX = m_AmpX1 * m_Noise1.CubicNoise2D(BlockX * m_FreqX1, BlockZ * m_FreqX1);
		NoiseX       += m_AmpX2 * m_Noise2.CubicNoise2D(BlockX * m_FreqX2, BlockZ * m_FreqX2);
		NoiseX       += m_AmpX3 * m_Noise3.CubicNoise2D(BlockX * m_FreqX3, BlockZ * m_FreqX3);
		double NoiseZ = m_AmpZ1 * m_Noise4.CubicNoise2D(BlockX * m_FreqZ1, BlockZ * m_FreqZ1);
		NoiseZ       += m_AmpZ2 * m_Noise5.CubicNoise2D(BlockX * m_FreqZ2, BlockZ * m_FreqZ2);
		NoiseZ       += m_AmpZ3 * m_Noise6.CubicNoise2D(BlockX * m_FreqZ3, BlockZ * m_FreqZ3);

		DistortX[4 * x][4 * z] = static_cast<int>(BlockX + NoiseX);
		DistortZ[4 * x][4 * z] = static_cast<int>(BlockZ + NoiseZ);
	}

	LinearUpscale2DArrayInPlace<cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4>(&DistortX[0][0]);
	LinearUpscale2DArrayInPlace<cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4>(&DistortZ[0][0]);

	// Apply distortion to each block coord, then query the voronoi maps for biome group and biome index and choose biome based on that:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int SeedX, SeedZ, MinDist2;
			int BiomeGroup = m_VoronoiLarge.GetValueAt(DistortX[x][z], DistortZ[x][z], SeedX, SeedZ, MinDist2) / 7;
			size_t BiomeIdx   = static_cast<size_t>(m_VoronoiSmall.GetValueAt(DistortX[x][z], DistortZ[x][z], SeedX, SeedZ, MinDist2) / 11);
			int MinDist1 = (DistortX[x][z] - SeedX) * (DistortX[x][z] - SeedX) + (DistortZ[x][z] - SeedZ) * (DistortZ[x][z] - SeedZ);
			cChunkDef::SetBiome(a_BiomeMap, x, z, SelectBiome(BiomeGroup, BiomeIdx, (MinDist1 < MinDist2 / 4) ? 1 : 0));
		}
	}
}





EMCSBiome cBioGenTwoLevel::SelectBiome(int a_BiomeGroup, size_t a_BiomeIdx, int a_DistLevel)
{
	// TODO: Move this into settings
	struct BiomeLevels
	{
		EMCSBiome InnerBiome;
		EMCSBiome OuterBiome;
	} ;

	static BiomeLevels bgOcean[] =
	{
		{ biOcean, biOcean, },
		{ biOcean, biOcean, },
		{ biOcean, biOcean, },
		{ biOcean, biOcean, },
		{ biOcean, biDeepOcean, },
		{ biOcean, biDeepOcean, },
		{ biDeepOcean, biDeepOcean, },
		{ biDeepOcean, biDeepOcean, },
		{ biDeepOcean, biDeepOcean, },
		{ biDeepOcean, biDeepOcean, },
		{ biMushroomIsland, biMushroomShore, }
	} ;
	static BiomeLevels bgFrozen[] =
	{
		{ biIcePlains,         biIcePlains, },
		{ biIceMountains,      biIceMountains, },
		{ biFrozenOcean,       biFrozenRiver, },
		{ biColdTaiga,         biColdTaiga, },
		{ biColdTaigaHills,    biColdTaigaHills, },
		{ biColdTaigaM,        biColdTaigaM, },
		{ biIcePlainsSpikes,   biIcePlainsSpikes, },
		{ biExtremeHills,      biExtremeHillsEdge, },
		{ biExtremeHillsPlus,  biExtremeHillsEdge, },
		{ biExtremeHillsPlusM, biExtremeHillsPlusM, },
	} ;
	static BiomeLevels bgTemperate[] =
	{
		{ biBirchForestHills,  biBirchForest, },
		{ biBirchForest,       biBirchForest, },
		{ biBirchForestHillsM, biBirchForestM, },
		{ biBirchForestM,      biBirchForestM, },
		{ biForest,            biForestHills, },
		{ biFlowerForest,      biFlowerForest, },
		{ biRoofedForest,      biForest, },
		{ biRoofedForest,      biRoofedForest, },
		{ biRoofedForestM,     biForest, },
		{ biPlains,            biPlains, },
		{ biSunflowerPlains,   biSunflowerPlains, },
		{ biSwampland,         biSwampland, },
		{ biSwamplandM,        biSwamplandM, },
	} ;
	static BiomeLevels bgWarm[] =
	{
		{ biDesertHills,    biDesert, },
		{ biDesert,         biDesert, },
		{ biDesertM,        biDesertM, },
		{ biSavannaPlateau, biSavanna, },
		{ biSavanna,        biSavanna, },
		{ biSavannaM,       biSavannaM, },
	} ;
	static BiomeLevels bgMesa[] =
	{
		{ biMesaPlateau,    biMesa, },
		{ biMesaPlateauF,   biMesa, },
		{ biMesaPlateauFM,  biMesa, },
		{ biMesaPlateauM,   biMesa, },
		{ biMesaBryce,      biMesaBryce, },
		{ biSavanna,        biSavanna, },
		{ biSavannaPlateau, biSavanna, },
	} ;
	static BiomeLevels bgConifers[] =
	{
		{ biTaiga,                biTaiga, },
		{ biTaigaM,               biTaigaM, },
		{ biMegaTaiga,            biMegaTaiga, },
		{ biMegaSpruceTaiga,      biMegaSpruceTaiga, },
		{ biMegaSpruceTaigaHills, biMegaSpruceTaiga, }
	} ;
	static BiomeLevels bgDenseTrees[] =
	{
		{ biJungleHills, biJungle, },
		{ biJungle, biJungleEdge, },
		{ biJungleM, biJungleEdgeM, },
	} ;
	static struct
	{
		BiomeLevels * Biomes;
		size_t        Count;
	} BiomeGroups[] =
	{
		{ bgOcean,      ARRAYCOUNT(bgOcean), },
		{ bgOcean,      ARRAYCOUNT(bgOcean), },
		{ bgFrozen,     ARRAYCOUNT(bgFrozen), },
		{ bgFrozen,     ARRAYCOUNT(bgFrozen), },
		{ bgTemperate,  ARRAYCOUNT(bgTemperate), },
		{ bgTemperate,  ARRAYCOUNT(bgTemperate), },
		{ bgConifers,   ARRAYCOUNT(bgConifers), },
		{ bgConifers,   ARRAYCOUNT(bgConifers), },
		{ bgWarm,       ARRAYCOUNT(bgWarm), },
		{ bgWarm,       ARRAYCOUNT(bgWarm), },
		{ bgMesa,       ARRAYCOUNT(bgMesa), },
		{ bgDenseTrees, ARRAYCOUNT(bgDenseTrees), },
	} ;
	size_t Group = static_cast<size_t>(a_BiomeGroup) % ARRAYCOUNT(BiomeGroups);
	size_t Index = a_BiomeIdx % BiomeGroups[Group].Count;
	return (a_DistLevel > 0) ? BiomeGroups[Group].Biomes[Index].InnerBiome : BiomeGroups[Group].Biomes[Index].OuterBiome;
}





void cBioGenTwoLevel::InitializeBiomeGen(cIniFile & a_IniFile)
{
	m_VoronoiLarge.SetCellSize(a_IniFile.GetValueSetI("Generator", "TwoLevelLargeCellSize", 1024));
	m_VoronoiSmall.SetCellSize(a_IniFile.GetValueSetI("Generator", "TwoLevelSmallCellSize", 128));
	m_FreqX1 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortXOctave1Freq", 0.01));
	m_AmpX1  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortXOctave1Amp",  80));
	m_FreqX2 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortXOctave2Freq", 0.05));
	m_AmpX2  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortXOctave2Amp",  20));
	m_FreqX3 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortXOctave3Freq", 0.1));
	m_AmpX3  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortXOctave3Amp",  8));
	m_FreqZ1 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortZOctave1Freq", 0.01));
	m_AmpZ1  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortZOctave1Amp",  80));
	m_FreqZ2 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortZOctave2Freq", 0.05));
	m_AmpZ2  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortZOctave2Amp",  20));
	m_FreqZ3 = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortZOctave3Freq", 0.1));
	m_AmpZ3  = static_cast<float>(a_IniFile.GetValueSetF("Generator", "TwoLevelDistortZOctave3Amp",  8));
}





////////////////////////////////////////////////////////////////////////////////
// cBioGenGrown:

class cBioGenGrown:
	public cBiomeGen
{
public:
	cBioGenGrown(int a_Seed)
	{
		auto FinalRivers =

			std::make_shared<cIntGenChoice<2, 7>>(a_Seed + 12)
			| MakeIntGen<cIntGenZoom  <10>>(a_Seed + 11)
			| MakeIntGen<cIntGenSmooth<8>>(a_Seed + 6)
			| MakeIntGen<cIntGenSmooth<6>>(a_Seed + 5)
			| MakeIntGen<cIntGenZoom  <8>>(a_Seed + 10)
			| MakeIntGen<cIntGenSmooth<6>>(a_Seed + 5)
			| MakeIntGen<cIntGenZoom  <8>>(a_Seed + 9)
			| MakeIntGen<cIntGenSmooth<6>>(a_Seed + 5)
			| MakeIntGen<cIntGenZoom  <8>>(a_Seed + 8)
			| MakeIntGen<cIntGenSmooth<6>>(a_Seed + 5)
			| MakeIntGen<cIntGenZoom  <9>>(a_Seed + 4)
			| MakeIntGen<cIntGenRiver <7>>(a_Seed + 3)
			| MakeIntGen<cIntGenZoom  <10>>(a_Seed + 2)
			| MakeIntGen<cIntGenSmooth<8>>(a_Seed + 1);

		auto alteration =
			std::make_shared<cIntGenZoom     <8>>(a_Seed,
			std::make_shared<cIntGenLandOcean<6>>(a_Seed, 20
		));

		auto alteration2 =
			std::make_shared<cIntGenZoom     <8>>(a_Seed + 1,
			std::make_shared<cIntGenZoom     <6>>(a_Seed + 2,
			std::make_shared<cIntGenZoom     <5>>(a_Seed + 1,
			std::make_shared<cIntGenZoom     <4>>(a_Seed + 2,
			std::make_shared<cIntGenLandOcean<4>>(a_Seed + 1, 10
		)))));

		auto FinalBiomes =
			std::make_shared<cIntGenSmooth         <8>> (a_Seed + 1,
			std::make_shared<cIntGenZoom           <10>>(a_Seed + 15,
			std::make_shared<cIntGenSmooth         <7>> (a_Seed + 1,
			std::make_shared<cIntGenZoom           <9>> (a_Seed + 16,
			std::make_shared<cIntGenBeaches        <6>> (
			std::make_shared<cIntGenZoom           <8>> (a_Seed + 1,
			std::make_shared<cIntGenAddIslands     <6>> (a_Seed + 2004, 10,
			std::make_shared<cIntGenAddToOcean     <6>> (a_Seed + 10, 500, biDeepOcean,
			std::make_shared<cIntGenReplaceRandomly<8>> (a_Seed + 1, biPlains, biSunflowerPlains, 20,
			std::make_shared<cIntGenMBiomes        <8>> (a_Seed + 5, alteration2,
			std::make_shared<cIntGenAlternateBiomes<8>> (a_Seed + 1, alteration,
			std::make_shared<cIntGenBiomeEdges     <8>> (a_Seed + 3,
			std::make_shared<cIntGenZoom           <10>>(a_Seed + 2,
			std::make_shared<cIntGenZoom           <7>> (a_Seed + 4,
			std::make_shared<cIntGenReplaceRandomly<5>> (a_Seed + 99, biIcePlains, biIcePlainsSpikes, 50,
			std::make_shared<cIntGenZoom           <5>> (a_Seed + 8,
			std::make_shared<cIntGenAddToOcean     <4>> (a_Seed + 10, 300, biDeepOcean,
			std::make_shared<cIntGenAddToOcean     <6>> (a_Seed + 9, 8, biMushroomIsland,
			std::make_shared<cIntGenBiomes         <8>> (a_Seed + 3000,
			std::make_shared<cIntGenAddIslands     <8>> (a_Seed + 2000, 200,
			std::make_shared<cIntGenZoom           <8>> (a_Seed + 5,
			std::make_shared<cIntGenRareBiomeGroups<6>> (a_Seed + 5, 50,
			std::make_shared<cIntGenBiomeGroupEdges<6>> (
			std::make_shared<cIntGenAddIslands     <8>> (a_Seed + 2000, 200,
			std::make_shared<cIntGenZoom           <8>> (a_Seed + 7,
			std::make_shared<cIntGenSetRandomly    <6>> (a_Seed + 8, 50, bgOcean,
			std::make_shared<cIntGenReplaceRandomly<6>> (a_Seed + 101, bgIce, bgTemperate, 150,
			std::make_shared<cIntGenAddIslands     <6>> (a_Seed + 2000, 200,
			std::make_shared<cIntGenSetRandomly    <6>> (a_Seed + 9, 50, bgOcean,
			std::make_shared<cIntGenLandOcean      <5>> (a_Seed + 100, 30)
			| MakeIntGen<cIntGenZoom           <6>> (a_Seed + 10)
		)))))))))))))))))))))))))))));

		m_Gen =
			std::make_shared<cIntGenSmooth   <16>>(a_Seed,
			std::make_shared<cIntGenZoom     <18>>(a_Seed,
			std::make_shared<cIntGenSmooth   <11>>(a_Seed,
			std::make_shared<cIntGenZoom     <13>>(a_Seed,
			std::make_shared<cIntGenMixRivers<8>> (
			FinalBiomes, FinalRivers
		)))));
	}

	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_Biomes) override
	{
		cIntGen<16, 16>::Values vals;
		m_Gen->GetInts(a_ChunkCoords.m_ChunkX * cChunkDef::Width, a_ChunkCoords.m_ChunkZ * cChunkDef::Width, vals);
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				cChunkDef::SetBiome(a_Biomes, x, z, static_cast<EMCSBiome>(vals[x + cChunkDef::Width * z]));
			}
		}
	}

protected:
	std::shared_ptr<cIntGen<16, 16>> m_Gen;
};





////////////////////////////////////////////////////////////////////////////////
// cBioGenProtGrown:

class cBioGenProtGrown:
	public cBiomeGen
{
public:
	cBioGenProtGrown(int a_Seed)
	{
		auto FinalRivers =
			std::make_shared<cProtIntGenSmooth>(a_Seed + 1,
			std::make_shared<cProtIntGenZoom  >(a_Seed + 2,
			std::make_shared<cProtIntGenRiver >(a_Seed + 3,
			std::make_shared<cProtIntGenZoom  >(a_Seed + 4,
			std::make_shared<cProtIntGenSmooth>(a_Seed + 5,
			std::make_shared<cProtIntGenZoom  >(a_Seed + 8,
			std::make_shared<cProtIntGenSmooth>(a_Seed + 5,
			std::make_shared<cProtIntGenZoom  >(a_Seed + 9,
			std::make_shared<cProtIntGenSmooth>(a_Seed + 5,
			std::make_shared<cProtIntGenZoom  >(a_Seed + 10,
			std::make_shared<cProtIntGenSmooth>(a_Seed + 5,
			std::make_shared<cProtIntGenSmooth>(a_Seed + 6,
			std::make_shared<cProtIntGenZoom  >(a_Seed + 11,
			std::make_shared<cProtIntGenChoice>(a_Seed + 12, 2
		))))))))))))));

		auto alteration =
			std::make_shared<cProtIntGenZoom     >(a_Seed,
			std::make_shared<cProtIntGenLandOcean>(a_Seed, 20
		));

		auto alteration2 =
			std::make_shared<cProtIntGenZoom     >(a_Seed + 1,
			std::make_shared<cProtIntGenZoom     >(a_Seed + 2,
			std::make_shared<cProtIntGenZoom     >(a_Seed + 1,
			std::make_shared<cProtIntGenZoom     >(a_Seed + 2,
			std::make_shared<cProtIntGenLandOcean>(a_Seed + 1, 10
		)))));

		auto FinalBiomes =
			std::make_shared<cProtIntGenSmooth         >(a_Seed + 1,
			std::make_shared<cProtIntGenZoom           >(a_Seed + 15,
			std::make_shared<cProtIntGenSmooth         >(a_Seed + 1,
			std::make_shared<cProtIntGenZoom           >(a_Seed + 16,
			std::make_shared<cProtIntGenBeaches        >(
			std::make_shared<cProtIntGenZoom           >(a_Seed + 1,
			std::make_shared<cProtIntGenAddIslands     >(a_Seed + 2004, 10,
			std::make_shared<cProtIntGenAddToOcean     >(a_Seed + 10, 500, biDeepOcean,
			std::make_shared<cProtIntGenReplaceRandomly>(a_Seed + 1, biPlains, biSunflowerPlains, 20,
			std::make_shared<cProtIntGenMBiomes        >(a_Seed + 5, alteration2,
			std::make_shared<cProtIntGenAlternateBiomes>(a_Seed + 1, alteration,
			std::make_shared<cProtIntGenBiomeEdges     >(a_Seed + 3,
			std::make_shared<cProtIntGenZoom           >(a_Seed + 2,
			std::make_shared<cProtIntGenZoom           >(a_Seed + 4,
			std::make_shared<cProtIntGenReplaceRandomly>(a_Seed + 99, biIcePlains, biIcePlainsSpikes, 50,
			std::make_shared<cProtIntGenZoom           >(a_Seed + 8,
			std::make_shared<cProtIntGenAddToOcean     >(a_Seed + 10, 300, biDeepOcean,
			std::make_shared<cProtIntGenAddToOcean     >(a_Seed + 9, 8, biMushroomIsland,
			std::make_shared<cProtIntGenBiomes         >(a_Seed + 3000,
			std::make_shared<cProtIntGenAddIslands     >(a_Seed + 2000, 200,
			std::make_shared<cProtIntGenZoom           >(a_Seed + 5,
			std::make_shared<cProtIntGenRareBiomeGroups>(a_Seed + 5, 50,
			std::make_shared<cProtIntGenBiomeGroupEdges>(
			std::make_shared<cProtIntGenAddIslands     >(a_Seed + 2000, 200,
			std::make_shared<cProtIntGenZoom           >(a_Seed + 7,
			std::make_shared<cProtIntGenSetRandomly    >(a_Seed + 8, 50, bgOcean,
			std::make_shared<cProtIntGenReplaceRandomly>(a_Seed + 101, bgIce, bgTemperate, 150,
			std::make_shared<cProtIntGenAddIslands     >(a_Seed + 2000, 200,
			std::make_shared<cProtIntGenSetRandomly    >(a_Seed + 9, 50, bgOcean,
			std::make_shared<cProtIntGenZoom           >(a_Seed + 10,
			std::make_shared<cProtIntGenLandOcean      >(a_Seed + 100, 30
		)))))))))))))))))))))))))))))));

		m_Gen =
			std::make_shared<cProtIntGenSmooth   >(a_Seed,
			std::make_shared<cProtIntGenZoom     >(a_Seed,
			std::make_shared<cProtIntGenSmooth   >(a_Seed,
			std::make_shared<cProtIntGenZoom     >(a_Seed,
			std::make_shared<cProtIntGenMixRivers>(
			FinalBiomes, FinalRivers
		)))));
	}

	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_Biomes) override
	{
		int vals[16 * 16];
		m_Gen->GetInts(a_ChunkCoords.m_ChunkX * cChunkDef::Width, a_ChunkCoords.m_ChunkZ * cChunkDef::Width, 16, 16, vals);
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				cChunkDef::SetBiome(a_Biomes, x, z, static_cast<EMCSBiome>(vals[x + cChunkDef::Width * z]));
			}
		}
	}

protected:
	std::shared_ptr<cProtIntGen> m_Gen;
};





////////////////////////////////////////////////////////////////////////////////
// cBiomeGen:

cBiomeGenPtr cBiomeGen::CreateBiomeGen(cIniFile & a_IniFile, int a_Seed, bool & a_CacheOffByDefault)
{
	AString BiomeGenName = a_IniFile.GetValue("Generator", "BiomeGen");
	if (BiomeGenName.empty())
	{
		LOGWARN("[Generator] BiomeGen value not set in world.ini, using \"Grown\".");
		BiomeGenName = "Grown";
	}

	cBiomeGen * res = nullptr;
	a_CacheOffByDefault = false;
	if (NoCaseCompare(BiomeGenName, "constant") == 0)
	{
		res = new cBioGenConstant;
		a_CacheOffByDefault = true;  // we're generating faster than a cache would retrieve data :)
	}
	else if (NoCaseCompare(BiomeGenName, "checkerboard") == 0)
	{
		res = new cBioGenCheckerboard;
		a_CacheOffByDefault = true;  // we're (probably) generating faster than a cache would retrieve data
	}
	else if (NoCaseCompare(BiomeGenName, "voronoi") == 0)
	{
		res = new cBioGenVoronoi(a_Seed);
	}
	else if (NoCaseCompare(BiomeGenName, "distortedvoronoi") == 0)
	{
		res = new cBioGenDistortedVoronoi(a_Seed);
	}
	else if (NoCaseCompare(BiomeGenName, "twolevel") == 0)
	{
		res = new cBioGenTwoLevel(a_Seed);
	}
	else if (NoCaseCompare(BiomeGenName, "multistepmap") == 0)
	{
		res = new cBioGenMultiStepMap(a_Seed);
	}
	else if (NoCaseCompare(BiomeGenName, "grownprot") == 0)
	{
		res = new cBioGenProtGrown(a_Seed);
	}
	else
	{
		if (NoCaseCompare(BiomeGenName, "grown") != 0)
		{
			LOGWARNING("Unknown BiomeGen \"%s\", using \"Grown\" instead.", BiomeGenName.c_str());
		}
		res = new cBioGenGrown(a_Seed);
	}
	res->InitializeBiomeGen(a_IniFile);

	return cBiomeGenPtr(res);
}





////////////////////////////////////////////////////////////////////////////////
// Performance tests:

// Change to 1 to enable the perf test:
#if 0

class cBioGenPerfTest
{
public:
	cBioGenPerfTest()
	{
		std::cout << "BioGen performance tests commencing, please wait..." << std::endl;
		TestGen("MultiStepMap", std::make_unique<cBioGenMultiStepMap>(1).get());
		TestGen("Grown",        std::make_unique<cBioGenGrown>(1).get());
		TestGen("GrownProt",    std::make_unique<cBioGenProtGrown>(1).get());
		std::cout << "BioGen performance tests complete." << std::endl;
	}

protected:
	void TestGen(const AString && a_GenName, cBiomeGen * a_BioGen)
	{
		// Initialize the default settings for the generator:
		cIniFile iniFile;
		a_BioGen->InitializeBiomeGen(iniFile);

		// Generate the biomes:
		auto start = std::chrono::system_clock::now();
		for (int z = 0; z < 100; z++)
		{
			for (int x = 0; x < 100; x++)
			{
				cChunkDef::BiomeMap biomes;
				a_BioGen->GenBiomes(x, z, biomes);
			}  // for x
		}  // for z
		auto dur = std::chrono::system_clock::now() - start;
		double milliseconds = static_cast<double>((std::chrono::duration_cast<std::chrono::milliseconds>(dur)).count());

		std::cout << a_GenName << ": " << 1000.0 * 100.0 * 100.0 / milliseconds << " chunks per second" << std::endl;
	}
} g_BioGenPerfTest;

#endif




