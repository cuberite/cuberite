
// VillageGen.cpp

// Implements the cVillageGen class representing the village generator

#include "Globals.h"
#include "VillageGen.h"
#include "Prefabs/PlainsVillagePrefabs.h"
#include "Prefabs/SandVillagePrefabs.h"





/*
How village generating works:
By descending from a cGridStructGen, a semi-random grid is generated. A village may be generated for each of
the grid's cells. Each cell checks the biomes in an entire chunk around it, only generating a village if all
biomes are village-friendly. If yes, the entire village structure is built for that cell. If not, the cell
is left village-less.

A village is generated starting by its well. The well is placed in the grid's origin point. Then a set of
random lengths roads is generated - 4 roads going from the well, then at the end of each road another set of
roads, crossing them perpendicular, then at the end of those another set, up to a set maximum branching
depth. The roads are placed in a T or L shape, with the old road being the center stem of the T. Roads avoid
crossing each other and going further away from the well than the maximum block size of the village.
Finally, houses are places along the roads, avoiding collisions with already-existing items.

When the village is about to be drawn into a chunk, it queries the heights for each item intersecting the
chunk. The prefabs are shifted so that their pivot points lie on the surface, and the roads are drawn
directly by turning the surface blocks into gravel / sandstone.
*/

class cVillageGen::cVillage :
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure super;
	
public:
	cVillage(
		int a_Seed,
		int a_OriginX, int a_OriginZ,
		int a_MaxRoadDepth,
		int a_MaxSize,
		cPrefabPiecePool & a_Prefabs,
		cTerrainHeightGen & a_HeightGen
	) :
		super(a_OriginX, a_OriginZ),
		m_Seed(a_Seed),
		m_Noise(a_Seed),
		m_MaxSize(a_MaxSize),
		m_Borders(a_OriginX - a_MaxSize, 0, a_OriginZ - a_MaxSize, a_OriginX + a_MaxSize, 255, a_OriginZ + a_MaxSize),
		m_Prefabs(a_Prefabs),
		m_HeightGen(a_HeightGen)
	{
		PlaceWell();
		BuildRoads(a_MaxRoadDepth);
		PlaceHouses();
	}
	
protected:
	class cItem
	{
	public:
		/* The position of the item, X/Z-wise: */
		int m_MinX, m_MaxX, m_MinZ, m_MaxZ;
		
		/** The prefab to use. If NULL, this is a road. */
		cPrefab * m_Prefab;
		
		/** Number of rotations that should be applied to the prefab. */
		int m_NumRotations;
		
		/* The bottom of the prefab. Only valid if the item is a prefab, not valid for roads. */
		int m_BaseY;
		
		/** Creates a new item with the specified parameters.
		m_BaseY is set to -1 and will be adjusted later on when drawing. */
		cItem(int a_MinX, int a_MaxX, int a_MinZ, int a_MaxZ, cPrefab * a_Prefab, int a_NumRotations) :
			m_MinX(a_MinX),
			m_MaxX(a_MaxX),
			m_MinZ(a_MinZ),
			m_MaxZ(a_MaxZ),
			m_Prefab(a_Prefab),
			m_NumRotations(a_NumRotations),
			m_BaseY(-1)
		{
		}
	} ;
	typedef SharedPtr<cItem> cItemPtr;
	typedef std::vector<cItemPtr> cItemPtrs;
	
	
	/** Seed for the random functions */
	int m_Seed;
	
	/** The noise used as a pseudo-random generator */
	cNoise m_Noise;
	
	/** Maximum size, in X/Z blocks, of the village (radius from the origin) */
	int m_MaxSize;
	
	/** Borders of the vilalge - no item may reach out of this cuboid. */
	cCuboid m_Borders;
	
	/** Prefabs to use for buildings */
	cPrefabPiecePool & m_Prefabs;
	
	/** The underlying height generator, used for placing the structures on top of the terrain. */
	cTerrainHeightGen & m_HeightGen;
	
	/** The items that are generated in the village (houses, roads). */
	cItemPtrs m_Items;
	
	
	/** Places the well at the center of the village */
	void PlaceWell(void)
	{
		// Pick a prefab from the starting pieces:
		cPieces StartingPieces = ((cPiecePool &)m_Prefabs).GetStartingPieces();
		ASSERT(!StartingPieces.empty());
		int TotalWeight = 0;
		for (cPieces::const_iterator itr = StartingPieces.begin(), end = StartingPieces.end(); itr != end; ++itr)
		{
			TotalWeight += ((const cPrefab *)(*itr))->GetDefaultWeight();
		}
		ASSERT(TotalWeight > 0);
		int rnd = (m_Noise.IntNoise2DInt(m_OriginX, m_OriginZ) / 7) % TotalWeight;
		cPiece * WellPiece = StartingPieces[0];
		for (cPieces::const_iterator itr = StartingPieces.begin(), end = StartingPieces.end(); itr != end; ++itr)
		{
			rnd -= ((const cPrefab *)(*itr))->GetDefaultWeight();
			if (rnd <= 0)
			{
				WellPiece = *itr;
				break;
			}
		}
		ASSERT(WellPiece != NULL);
		
		// Pick a rotation:
		// TODO
		int NumRotations = 0;
		Vector3i Size = WellPiece->GetSize();
		
		// Put the well in the placed items array:
		m_Items.push_back(cItemPtr(new cItem(m_OriginX, m_OriginX + Size.x, m_OriginZ, m_OriginZ + Size.z, (cPrefab *)WellPiece, NumRotations)));
	}
	
	
	/** Places the roads going from the well outwards. */
	void BuildRoads(int a_MaxRoadDepth)
	{
		/*
		ASSERT(m_Items.size() == 1);
		const cItem & Well = *m_Items[0];
		*/
		// TODO
	}
	
	
	/** Places houses along the roads. */
	void PlaceHouses(void)
	{
		// TODO
	}
	
	
	// cGrdStructGen::cStructure overrides:
	virtual void DrawIntoChunk(cChunkDesc & a_Chunk) override
	{
		// TODO
		// Iterate over all items
		// Each intersecting prefab is placed on ground (if not already placed), then drawn
		// Each intersecting road is drawn by replacing top soil blocks with gravel / sandstone blocks
	}
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cVillageGen:

cPrefabPiecePool cVillageGen::m_SandVillage  (g_SandVillagePrefabs,   g_SandVillagePrefabsCount,   g_SandVillageStartingPrefabs,   g_SandVillageStartingPrefabsCount);
cPrefabPiecePool cVillageGen::m_PlainsVillage(g_PlainsVillagePrefabs, g_PlainsVillagePrefabsCount, g_PlainsVillageStartingPrefabs, g_PlainsVillageStartingPrefabsCount);





cVillageGen::cVillageGen(int a_Seed, int a_GridSize, int a_MaxRoadDepth, int a_MaxSize, cBiomeGen & a_BiomeGen, cTerrainHeightGen & a_HeightGen) :
	super(a_Seed, a_GridSize, a_GridSize, a_MaxSize, a_MaxSize, 100),
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
	return cStructurePtr(new cVillage(m_Seed, a_OriginX, a_OriginZ, m_MaxRoadDepth, m_MaxSize, *VillagePrefabs, m_HeightGen));
}




