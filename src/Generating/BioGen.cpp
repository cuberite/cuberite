
// BioGen.cpp

// Implements the various biome generators

#include "Globals.h"
#include "BioGen.h"
#include "../lib/inifile/iniFile.h"
#include "../LinearUpscale.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBioGenConstant:

void cBioGenConstant::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	for (int i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		a_BiomeMap[i] = m_Biome;
	}
}





void cBioGenConstant::InitializeBiomeGen(cIniFile & a_IniFile)
{
	AString Biome = a_IniFile.GetValueSet("Generator", "ConstantBiome", "Plains");
	m_Biome = StringToBiome(Biome);
	if (m_Biome == -1)
	{
		LOGWARN("[Generator]::ConstantBiome value \"%s\" not recognized, using \"Plains\".", Biome.c_str());
		m_Biome = biPlains;
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBioGenCache:

cBioGenCache::cBioGenCache(cBiomeGen * a_BioGenToCache, int a_CacheSize) :
	m_BioGenToCache(a_BioGenToCache),
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





cBioGenCache::~cBioGenCache()
{
	delete[] m_CacheData;
	delete[] m_CacheOrder;
}





void cBioGenCache::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	if (((m_NumHits + m_NumMisses) % 1024) == 10)
	{
		LOGD("BioGenCache: %d hits, %d misses, saved %.2f %%", m_NumHits, m_NumMisses, 100.0 * m_NumHits / (m_NumHits + m_NumMisses));
		LOGD("BioGenCache: Avg cache chain length: %.2f", (float)m_TotalChain / m_NumHits);
	}
	
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
		memcpy(a_BiomeMap, m_CacheData[Idx].m_BiomeMap, sizeof(a_BiomeMap));
		
		m_NumHits++;
		m_TotalChain += i;
		return;
	}  // for i - cache
	
	// Not in the cache:
	m_NumMisses++;
	m_BioGenToCache->GenBiomes(a_ChunkX, a_ChunkZ, a_BiomeMap);
	
	// Insert it as the first item in the MRU order:
	int Idx = m_CacheOrder[m_CacheSize - 1];
	for (int i = m_CacheSize - 1; i > 0; i--)
	{
		m_CacheOrder[i] = m_CacheOrder[i - 1];
	}  // for i - m_CacheOrder[]
	m_CacheOrder[0] = Idx;
	memcpy(m_CacheData[Idx].m_BiomeMap, a_BiomeMap, sizeof(a_BiomeMap));
	m_CacheData[Idx].m_ChunkX = a_ChunkX;
	m_CacheData[Idx].m_ChunkZ = a_ChunkZ;
}





void cBioGenCache::InitializeBiomeGen(cIniFile & a_IniFile)
{
	super::InitializeBiomeGen(a_IniFile);
	m_BioGenToCache->InitializeBiomeGen(a_IniFile);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBiomeGenList:

void cBiomeGenList::InitializeBiomes(const AString & a_Biomes)
{
	AStringVector Split = StringSplit(a_Biomes, ",");

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
			Count = atol(Split2[1].c_str());
			if (Count <= 0)
			{
				LOGWARNING("Cannot decode biome count: \"%s\"; using 1.", Split2[1].c_str());
				Count = 1;
			}
		}
		EMCSBiome Biome = StringToBiome(Split2[0]);
		if (Biome != -1)
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
		m_BiomesCount = (int)m_Biomes.size();
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
	for (int i = 0; i < ARRAYCOUNT(Biomes); i++)
	{
		m_Biomes.push_back(Biomes[i]);
	}
	m_BiomesCount = (int)m_Biomes.size();
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBioGenCheckerboard:

void cBioGenCheckerboard::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int Base = cChunkDef::Width * a_ChunkZ + z;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Add = cChunkDef::Width * a_ChunkX + x;
			a_BiomeMap[x + cChunkDef::Width * z] = m_Biomes[(Base / m_BiomeSize + Add / m_BiomeSize) % m_BiomesCount];
		}
	}
}





void cBioGenCheckerboard::InitializeBiomeGen(cIniFile & a_IniFile)
{
	super::InitializeBiomeGen(a_IniFile);
	AString Biomes = a_IniFile.GetValueSet ("Generator", "CheckerBoardBiomes",    "");
	m_BiomeSize    = a_IniFile.GetValueSetI("Generator", "CheckerboardBiomeSize", 64);
	m_BiomeSize = (m_BiomeSize < 8) ? 8 : m_BiomeSize;
	InitializeBiomes(Biomes);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBioGenVoronoi :

void cBioGenVoronoi::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	int BaseZ = cChunkDef::Width * a_ChunkZ;
	int BaseX = cChunkDef::Width * a_ChunkX;
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int AbsoluteZ = BaseZ + z;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			cChunkDef::SetBiome(a_BiomeMap, x, z, VoronoiBiome(BaseX + x, AbsoluteZ));
		}  // for x
	}  // for z
}





void cBioGenVoronoi::InitializeBiomeGen(cIniFile & a_IniFile)
{
	super::InitializeBiomeGen(a_IniFile);
	m_CellSize     = a_IniFile.GetValueSetI("Generator", "VoronoiCellSize", 64);
	AString Biomes = a_IniFile.GetValueSet ("Generator", "VoronoiBiomes",   "");
	InitializeBiomes(Biomes);
}





EMCSBiome cBioGenVoronoi::VoronoiBiome(int a_BlockX, int a_BlockZ)
{
	int CellX = a_BlockX / m_CellSize;
	int CellZ = a_BlockZ / m_CellSize;
	
	// Note that Noise values need to be divided by 8 to gain a uniform modulo-2^n distribution
	
	// Get 5x5 neighboring cell seeds, compare distance to each. Return the biome in the minumim-distance cell
	int MinDist = m_CellSize * m_CellSize * 16;  // There has to be a cell closer than this
	EMCSBiome res = biPlains;  // Will be overriden
	for (int x = CellX - 2; x <= CellX + 2; x++)
	{
		int BaseX = x * m_CellSize;
		for (int z = CellZ - 2; z < CellZ + 2; z++)
		{
			int OffsetX = (m_Noise.IntNoise3DInt(x, 16 * x + 32 * z, z) / 8) % m_CellSize;
			int OffsetZ = (m_Noise.IntNoise3DInt(x, 32 * x - 16 * z, z) / 8) % m_CellSize;
			int SeedX = BaseX + OffsetX;
			int SeedZ = z * m_CellSize + OffsetZ;
			
			int Dist = (SeedX - a_BlockX) * (SeedX - a_BlockX) + (SeedZ - a_BlockZ) * (SeedZ - a_BlockZ);
			if (Dist < MinDist)
			{
				MinDist = Dist;
				res = m_Biomes[(m_Noise.IntNoise3DInt(x, x - z + 1000, z) / 8) % m_BiomesCount];
			}
		}  // for z
	}  // for x

	return res;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBioGenDistortedVoronoi:

void cBioGenDistortedVoronoi::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	int BaseZ = cChunkDef::Width * a_ChunkZ;
	int BaseX = cChunkDef::Width * a_ChunkX;
	
	// Distortions for linear interpolation:
	int DistortX[cChunkDef::Width + 1][cChunkDef::Width + 1];
	int DistortZ[cChunkDef::Width + 1][cChunkDef::Width + 1];
	for (int x = 0; x <= 4; x++) for (int z = 0; z <= 4; z++)
	{
		Distort(BaseX + x * 4, BaseZ + z * 4, DistortX[4 * x][4 * z], DistortZ[4 * x][4 * z]);
	}
	
	LinearUpscale2DArrayInPlace(&DistortX[0][0], cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4);
	LinearUpscale2DArrayInPlace(&DistortZ[0][0], cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4);
	
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int AbsoluteZ = BaseZ + z;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			// Distort(BaseX + x, AbsoluteZ, DistX, DistZ);
			cChunkDef::SetBiome(a_BiomeMap, x, z, VoronoiBiome(DistortX[x][z], DistortZ[x][z]));
		}  // for x
	}  // for z
}





void cBioGenDistortedVoronoi::InitializeBiomeGen(cIniFile & a_IniFile)
{
	// Do NOT call super::InitializeBiomeGen(), as it would try to read Voronoi params instead of DistortedVoronoi params
	m_CellSize     = a_IniFile.GetValueSetI("Generator", "DistortedVoronoiCellSize", 96);
	AString Biomes = a_IniFile.GetValueSet ("Generator", "DistortedVoronoiBiomes",   "");
	InitializeBiomes(Biomes);
}




void cBioGenDistortedVoronoi::Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ)
{
	double NoiseX = m_Noise.CubicNoise3D((float)a_BlockX / m_CellSize, (float)a_BlockZ / m_CellSize, 1000);
	NoiseX += 0.5 * m_Noise.CubicNoise3D(2 *  (float)a_BlockX / m_CellSize, 2 *  (float)a_BlockZ / m_CellSize, 2000);
	NoiseX += 0.08 * m_Noise.CubicNoise3D(16 * (float)a_BlockX / m_CellSize, 16 * (float)a_BlockZ / m_CellSize, 3000);
	double NoiseZ = m_Noise.CubicNoise3D((float)a_BlockX / m_CellSize, (float)a_BlockZ / m_CellSize, 4000);
	NoiseZ += 0.5 * m_Noise.CubicNoise3D(2 *  (float)a_BlockX / m_CellSize, 2 *  (float)a_BlockZ / m_CellSize, 5000);
	NoiseZ += 0.08 * m_Noise.CubicNoise3D(16 * (float)a_BlockX / m_CellSize, 16 * (float)a_BlockZ / m_CellSize, 6000);
	
	a_DistortedX = a_BlockX + (int)(m_CellSize * 0.5 * NoiseX);
	a_DistortedZ = a_BlockZ + (int)(m_CellSize * 0.5 * NoiseZ);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
	m_LandBiomesSize      = (float)a_IniFile.GetValueSetI("Generator", "MultiStepMapLandBiomeSize",      (int)m_LandBiomesSize);
}





void cBioGenMultiStepMap::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	DecideOceanLandMushroom(a_ChunkX, a_ChunkZ, a_BiomeMap);
	AddRivers(a_ChunkX, a_ChunkZ, a_BiomeMap);
	ApplyTemperatureHumidity(a_ChunkX, a_ChunkZ, a_BiomeMap);
}





void cBioGenMultiStepMap::DecideOceanLandMushroom(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	// Distorted Voronoi over 3 biomes, with mushroom having only a special occurence.
	
	// Prepare a distortion lookup table, by distorting a 5x5 area and using that as 1:4 zoom (linear interpolate):
	int BaseZ = cChunkDef::Width * a_ChunkZ;
	int BaseX = cChunkDef::Width * a_ChunkX;
	int DistortX[cChunkDef::Width + 1][cChunkDef::Width + 1];
	int DistortZ[cChunkDef::Width + 1][cChunkDef::Width + 1];
	int DistortSize = m_OceanCellSize / 2;
	for (int x = 0; x <= 4; x++) for (int z = 0; z <= 4; z++)
	{
		Distort(BaseX + x * 4, BaseZ + z * 4, DistortX[4 * x][4 * z], DistortZ[4 * x][4 * z], DistortSize);
	}
	LinearUpscale2DArrayInPlace(&DistortX[0][0], cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4);
	LinearUpscale2DArrayInPlace(&DistortZ[0][0], cChunkDef::Width + 1, cChunkDef::Width + 1, 4, 4);
	
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
			SeedV[xc][zc] = (((m_Noise6.IntNoise3DInt(RealCellX, RealCellX - RealCellZ + 1000, RealCellZ) / 11) % 256) > 90) ? biOcean : ((EMCSBiome)(-1));
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





void cBioGenMultiStepMap::AddRivers(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		float NoiseCoordZ = (float)(a_ChunkZ * cChunkDef::Width + z) / m_RiverCellSize;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			if (cChunkDef::GetBiome(a_BiomeMap, x, z) != -1)
			{
				// Biome already set, skip this column
				continue;
			}
			
			float NoiseCoordX = (float)(a_ChunkX * cChunkDef::Width + x) / m_RiverCellSize;
		
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





void cBioGenMultiStepMap::ApplyTemperatureHumidity(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	IntMap TemperatureMap;
	IntMap HumidityMap;
	BuildTemperatureHumidityMaps(a_ChunkX, a_ChunkZ, TemperatureMap, HumidityMap);
	
	FreezeWaterBiomes(a_BiomeMap, TemperatureMap);
	DecideLandBiomes(a_BiomeMap, TemperatureMap, HumidityMap);
}





void cBioGenMultiStepMap::Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ, int a_CellSize)
{
	double NoiseX = m_Noise3.CubicNoise2D(     (float)a_BlockX / a_CellSize,      (float)a_BlockZ / a_CellSize);
	NoiseX += 0.5 * m_Noise2.CubicNoise2D(2 *  (float)a_BlockX / a_CellSize, 2 *  (float)a_BlockZ / a_CellSize);
	NoiseX += 0.1 * m_Noise1.CubicNoise2D(16 * (float)a_BlockX / a_CellSize, 16 * (float)a_BlockZ / a_CellSize);
	double NoiseZ = m_Noise6.CubicNoise2D(     (float)a_BlockX / a_CellSize,      (float)a_BlockZ / a_CellSize);
	NoiseZ += 0.5 * m_Noise5.CubicNoise2D(2 *  (float)a_BlockX / a_CellSize, 2 *  (float)a_BlockZ / a_CellSize);
	NoiseZ += 0.1 * m_Noise4.CubicNoise2D(16 * (float)a_BlockX / a_CellSize, 16 * (float)a_BlockZ / a_CellSize);
	
	a_DistortedX = a_BlockX + (int)(a_CellSize * 0.5 * NoiseX);
	a_DistortedZ = a_BlockZ + (int)(a_CellSize * 0.5 * NoiseZ);
}





void cBioGenMultiStepMap::BuildTemperatureHumidityMaps(int a_ChunkX, int a_ChunkZ, IntMap & a_TemperatureMap, IntMap & a_HumidityMap)
{
	// Linear interpolation over 8x8 blocks; use double for better precision:
	DblMap TemperatureMap;
	DblMap HumidityMap;
	for (int z = 0; z < 17; z += 8)
	{
		float NoiseCoordZ = (float)(a_ChunkZ * cChunkDef::Width + z) / m_LandBiomesSize;
		for (int x = 0; x < 17; x += 8)
		{
			float NoiseCoordX = (float)(a_ChunkX * cChunkDef::Width + x) / m_LandBiomesSize;
		
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
	LinearUpscale2DArrayInPlace(TemperatureMap, 17, 17, 8, 8);
	LinearUpscale2DArrayInPlace(HumidityMap,    17, 17, 8, 8);
	
	// Re-map into integral values in [0 .. 255] range:
	for (int idx = 0; idx < ARRAYCOUNT(a_TemperatureMap); idx++)
	{
		a_TemperatureMap[idx] = std::max(0, std::min(255, (int)(128 + TemperatureMap[idx] * 128)));
		a_HumidityMap[idx]    = std::max(0, std::min(255, (int)(128 + HumidityMap[idx]    * 128)));
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
			if (cChunkDef::GetBiome(a_BiomeMap, x, z) != -1)
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
			}
		}  // for x
		idx += 1;
	}  // for z
}




