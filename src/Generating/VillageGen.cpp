
// VillageGen.cpp

// Implements the cVillageGen class representing the village generator

#include "Globals.h"
#include "VillageGen.h"
#include "Prefabs/PlainsVillagePrefabs.h"
#include "Prefabs/SandVillagePrefabs.h"





class cVillageGen::cVillage :
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure super;
	
public:
	cVillage(int a_Seed, int a_OriginX, int a_OriginZ, cPrefabPiecePool & a_Prefabs) :
		super(a_OriginX, a_OriginZ),
		m_Seed(a_Seed),
		m_Prefabs(a_Prefabs)
	{
	}
	
protected:
	/** Seed for the random functions */
	int m_Seed;
	
	/** Prefabs to use for buildings */
	cPrefabPiecePool & m_Prefabs;
	
	// cGrdStructGen::cStructure overrides:
	virtual void DrawIntoChunk(cChunkDesc & a_Chunk) override
	{
		// TODO
	}
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cVillageGen:

cPrefabPiecePool cVillageGen::m_SandVillage  (g_SandVillagePrefabs,   g_SandVillagePrefabsCount,   g_SandVillageStartingPrefabs,   g_SandVillageStartingPrefabsCount);
cPrefabPiecePool cVillageGen::m_PlainsVillage(g_PlainsVillagePrefabs, g_PlainsVillagePrefabsCount, g_PlainsVillageStartingPrefabs, g_PlainsVillageStartingPrefabsCount);





cVillageGen::cVillageGen(int a_Seed, int a_GridSize, cBiomeGen & a_BiomeGen, cTerrainHeightGen & a_HeightGen) :
	super(a_Seed, a_GridSize, a_GridSize, 128, 128, 100),
	m_BiomeGen(a_BiomeGen),
	m_HeightGen(a_HeightGen)
{
}





cGridStructGen::cStructurePtr cVillageGen::CreateStructure(int a_OriginX, int a_OriginZ)
{
	// Generate the biomes for the chunk surrounding the origin:
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_OriginX, a_OriginZ, ChunkX, ChunkZ);
	cChunkDef::BiomeMap Biomes;
	m_BiomeGen.GenBiomes(ChunkX, ChunkZ, Biomes);

	// Check if all the biomes are village-friendly:
	// If just one is not, no village is created, because it's likely that an unfriendly biome is too close
	cPrefabPiecePool * VillagePrefabs = NULL;
	for (size_t i = 0; i < ARRAYCOUNT(Biomes); i++)
	{
		switch (Biomes[i])
		{
			case biDesert:
			case biDesertM:
			{
				// These biomes allow sand villages
				VillagePrefabs = &m_SandVillage;
				break;
			}
			case biPlains:
			case biSavanna:
			case biSavannaM:
			case biSunflowerPlains:
			{
				// These biomes allow plains-style villages
				VillagePrefabs = &m_PlainsVillage;
				break;
			}
			default:
			{
				// Village-unfriendly biome, bail out with zero structure:
				return cStructurePtr();
			}
		}  // switch (Biomes[i])
	}  // for i - Biomes[]

	// Create a village based on the chosen prefabs:
	if (VillagePrefabs == NULL)
	{
		return cStructurePtr();
	}
	return cStructurePtr(new cVillage(m_Seed, a_OriginX, a_OriginZ, *VillagePrefabs));
}




