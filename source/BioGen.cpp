
// BioGen.cpp

// Implements the various biome generators

#include "Globals.h"
#include "BioGen.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBioGenConstant:

void cBioGenConstant::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	for (int i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		a_BiomeMap[i] = m_Biome;
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBiomeGenList:

void cBiomeGenList::InitializeBiomes(const AString & a_Biomes)
{
	AStringVector Split = StringSplit(a_Biomes, ",");

	// Convert each string in the list into biome:
	for (AStringVector::const_iterator itr = Split.begin(); itr != Split.end(); ++itr)
	{
		EMCSBiome Biome = StringToBiome(*itr);
		if (Biome != -1)
		{
			m_Biomes.push_back(Biome);
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





EMCSBiome cBioGenVoronoi::VoronoiBiome(int a_BlockX, int a_BlockZ)
{
	int CellX = a_BlockX / m_CellSize;
	int CellZ = a_BlockZ / m_CellSize;
	
	// Note that Noise values need to be divided by 8 to gain a uniform modulo-2^n distribution
	
	// Get 5x5 neighboring cell seeds, compare distance to each. Return the biome in the minumim-distance cell
	double MinDist = m_CellSize * m_CellSize;  // There has to be a cell closer than this
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
			
			double Dist = sqrt((double)((SeedX - a_BlockX) * (SeedX - a_BlockX) + (SeedZ - a_BlockZ) * (SeedZ - a_BlockZ)));
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
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int AbsoluteZ = BaseZ + z;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int DistX, DistZ;
			Distort(BaseX + x, AbsoluteZ, DistX, DistZ);
			cChunkDef::SetBiome(a_BiomeMap, x, z, VoronoiBiome(DistX, DistZ));
		}  // for x
	}  // for z
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





