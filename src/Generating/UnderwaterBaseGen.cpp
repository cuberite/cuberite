
// UnderwaterBaseGen.cpp

// Implements the cUnderwaterBaseGen class representing the underwater base generator

#include "Globals.h"
#include "UnderwaterBaseGen.h"
#include "Prefabs/UnderwaterBasePrefabs.h"
#include "PieceGenerator.h"





static cPrefabPiecePool g_UnderwaterBase(g_UnderwaterBasePrefabs, g_UnderwaterBasePrefabsCount, g_UnderwaterBaseStartingPrefabs, g_UnderwaterBaseStartingPrefabsCount);





////////////////////////////////////////////////////////////////////////////////
// cUnderwaterBaseGen::cUnderwaterBase:

class cUnderwaterBaseGen::cUnderwaterBase :
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure super;
	
public:
	cUnderwaterBase(
		int a_Seed,
		int a_GridX, int a_GridZ,
		int a_OriginX, int a_OriginZ,
		int a_MaxDepth,
		int a_MaxSize
	) :
		super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
		m_Seed(a_Seed),
		m_Noise(a_Seed),
		m_MaxSize(a_MaxSize),
		m_Borders(a_OriginX - a_MaxSize, 0, a_OriginZ - a_MaxSize, a_OriginX + a_MaxSize, 255, a_OriginZ + a_MaxSize)
	{
		// Generate the pieces for this base:
		cBFSPieceGenerator pg(g_UnderwaterBase, a_Seed);
		pg.PlacePieces(a_OriginX, 50, a_OriginZ, a_MaxDepth, m_Pieces);
		if (m_Pieces.empty())
		{
			return;
		}
	}
	
	~cUnderwaterBase()
	{
		cPieceGenerator::FreePieces(m_Pieces);
	}
	
protected:
	/** Seed for the random functions */
	int m_Seed;
	
	/** The noise used as a pseudo-random generator */
	cNoise m_Noise;
	
	/** Maximum size, in X/Z blocks, of the village (radius from the origin) */
	int m_MaxSize;
	
	/** Borders of the vilalge - no item may reach out of this cuboid. */
	cCuboid m_Borders;
	
	/** The village pieces, placed by the generator. */
	cPlacedPieces m_Pieces;
	
	
	// cGridStructGen::cStructure overrides:
	virtual void DrawIntoChunk(cChunkDesc & a_Chunk) override
	{
		for (cPlacedPieces::iterator itr = m_Pieces.begin(), end = m_Pieces.end(); itr != end; ++itr)
		{
			cPrefab & Prefab = (cPrefab &)((*itr)->GetPiece());
			Prefab.Draw(a_Chunk, *itr);
		}  // for itr - m_PlacedPieces[]
	}
} ;





////////////////////////////////////////////////////////////////////////////////
// cUnderwaterBaseGen:





cUnderwaterBaseGen::cUnderwaterBaseGen(int a_Seed, int a_GridSize, int a_MaxOffset, int a_MaxDepth, int a_MaxSize, cBiomeGen & a_BiomeGen) :
	super(a_Seed, a_GridSize, a_GridSize, a_MaxOffset, a_MaxOffset, a_MaxSize, a_MaxSize, 100),
	m_Noise(a_Seed + 1000),
	m_MaxDepth(a_MaxDepth),
	m_MaxSize(a_MaxSize),
	m_BiomeGen(a_BiomeGen)
{
}





cGridStructGen::cStructurePtr cUnderwaterBaseGen::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	// Generate the biomes for the chunk surrounding the origin:
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(a_OriginX, a_OriginZ, ChunkX, ChunkZ);
	cChunkDef::BiomeMap Biomes;
	m_BiomeGen.GenBiomes(ChunkX, ChunkZ, Biomes);

	// Check if all the biomes are ocean:
	// If just one is not, no base is created, because it's likely that an unfriendly biome is too close
	for (size_t i = 0; i < ARRAYCOUNT(Biomes); i++)
	{
		switch (Biomes[i])
		{
			case biOcean:
			case biDeepOcean:
			{
				// These biomes allow underwater bases
				break;
			}
			default:
			{
				// base-unfriendly biome, bail out with zero structure:
				return cStructurePtr();
			}
		}  // switch (Biomes[i])
	}  // for i - Biomes[]

	// Create a base based on the chosen prefabs:
	return cStructurePtr(new cUnderwaterBase(m_Seed, a_GridX, a_GridZ, a_OriginX, a_OriginZ, m_MaxDepth, m_MaxSize));
}




