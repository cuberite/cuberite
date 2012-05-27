
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
// cBioGenDistortedVoronoi:

void cBioGenDistortedVoronoi::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	// TODO: Replace this placeholder
	ASSERT(!"Not implemented yet");
	for (int i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		a_BiomeMap[i] = (EMCSBiome)(a_ChunkX + a_ChunkZ);
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBioGenCheckerboard:

void cBioGenCheckerboard::InitializeBiomes(const AString & a_Biomes)
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




