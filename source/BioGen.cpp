
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

void cBioGenCheckerboard::GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	// The list of biomes we will generate in the checkerboard:
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
	
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int Base = cChunkDef::Width * a_ChunkZ + z;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Add = cChunkDef::Width * a_ChunkX + x;
			a_BiomeMap[x + cChunkDef::Width * z] = Biomes[(Base / m_BiomeSize + Add / m_BiomeSize) % ARRAYCOUNT(Biomes)];
		}
	}
}




