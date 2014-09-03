
// TestRailsGen.cpp

// Implements the cTestRailsGen class representing the testing rails generator

#include "Globals.h"
#include "TestRailsGen.h"
#include "Prefabs/TestRailsPrefabs.h"
#include "PieceGenerator.h"





static cPrefabPiecePool g_TestRails(g_TestRailsPrefabs, g_TestRailsPrefabsCount, g_TestRailsStartingPrefabs, g_TestRailsStartingPrefabsCount);





////////////////////////////////////////////////////////////////////////////////
// cTestRailsGen::cTestRails:

class cTestRailsGen::cTestRails :
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure super;
	
public:
	cTestRails(
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
		// Generate the pieces for this test:
		cBFSPieceGenerator pg(g_TestRails, a_Seed);
		pg.PlacePieces(a_OriginX, 150, a_OriginZ, a_MaxDepth, m_Pieces);
		if (m_Pieces.empty())
		{
			return;
		}
	}
	
	~cTestRails()
	{
		cPieceGenerator::FreePieces(m_Pieces);
	}
	
protected:
	/** Seed for the random functions */
	int m_Seed;
	
	/** The noise used as a pseudo-random generator */
	cNoise m_Noise;
	
	/** Maximum size, in X/Z blocks, of the structure (radius from the origin) */
	int m_MaxSize;
	
	/** Borders of the structure - no item may reach out of this cuboid. */
	cCuboid m_Borders;
	
	/** The rails pieces, placed by the generator. */
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
// cTestRailsGen:





cTestRailsGen::cTestRailsGen(int a_Seed, int a_GridSize, int a_MaxOffset, int a_MaxDepth, int a_MaxSize) :
	super(a_Seed, a_GridSize, a_GridSize, a_MaxOffset, a_MaxOffset, a_MaxSize, a_MaxSize, 100),
	m_Noise(a_Seed + 1000),
	m_MaxDepth(a_MaxDepth),
	m_MaxSize(a_MaxSize)
{
}





cGridStructGen::cStructurePtr cTestRailsGen::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	// Create a base based on the chosen prefabs:
	return cStructurePtr(new cTestRails(m_Seed, a_GridX, a_GridZ, a_OriginX, a_OriginZ, m_MaxDepth, m_MaxSize));
}




