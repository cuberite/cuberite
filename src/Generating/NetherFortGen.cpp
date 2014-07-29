
// NetherFortGen.cpp

// Implements the cNetherFortGen class representing the nether fortress generator

#include "Globals.h"
#include "NetherFortGen.h"
#include "Prefabs/NetherFortPrefabs.h"





////////////////////////////////////////////////////////////////////////////////
// cNetherFortGen::cNetherFort:

class cNetherFortGen::cNetherFort :
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure super;
	
public:
	cNetherFortGen & m_ParentGen;
	int m_GridSize;
	int m_Seed;
	cPlacedPieces m_Pieces;


	cNetherFort(cNetherFortGen & a_ParentGen, int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ, int a_GridSize, int a_MaxDepth, int a_Seed) :
		super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
		m_ParentGen(a_ParentGen),
		m_GridSize(a_GridSize),
		m_Seed(a_Seed)
	{
		// TODO: Proper Y-coord placement
		int BlockY = 64;
		
		// Generate pieces:
		for (int i = 0; m_Pieces.size() < (size_t)(a_MaxDepth * a_MaxDepth / 8 + a_MaxDepth); i++)
		{
			cBFSPieceGenerator pg(cNetherFortGen::m_PiecePool, a_Seed + i);
			pg.PlacePieces(a_OriginX, BlockY, a_OriginZ, a_MaxDepth, m_Pieces);
		}
	}

	
	~cNetherFort()
	{
		cPieceGenerator::FreePieces(m_Pieces);
	}
	
		
	/** Carves the system into the chunk data */
	virtual void DrawIntoChunk(cChunkDesc & a_Chunk)
	{
		for (cPlacedPieces::const_iterator itr = m_Pieces.begin(), end = m_Pieces.end(); itr != end; ++itr)
		{
			const cPrefab & Prefab = (const cPrefab &)((*itr)->GetPiece());
			Prefab.Draw(a_Chunk, *itr);
		}  // for itr - m_PlacedPieces[]
	}
};





////////////////////////////////////////////////////////////////////////////////
// Performance test of the NetherFort generator:

/*
#include "OSSupport/Timer.h"
static class cNetherFortPerfTest
{
public:
	cNetherFortPerfTest(void)
	{
		cTimer Timer;
		long long StartTime = Timer.GetNowTime();
		
		const int GridSize = 512;
		const int MaxDepth = 12;
		const int NumIterations = 100;
		for (int i = 0; i < NumIterations; i++)
		{
			cNetherFortGen FortGen(i, GridSize, MaxDepth);
			delete new cNetherFortGen::cNetherFort(FortGen, 0, 0, GridSize, MaxDepth, i);
		}
		
		long long EndTime = Timer.GetNowTime();
		printf("%d forts took %lld msec (%f sec) to generate\n", NumIterations, EndTime - StartTime, ((double)(EndTime - StartTime)) / 1000);
		exit(0);
	}
	
} g_PerfTest;
//*/





////////////////////////////////////////////////////////////////////////////////
// cNetherFortGen:

cPrefabPiecePool cNetherFortGen::m_PiecePool(g_NetherFortPrefabs, g_NetherFortPrefabsCount, g_NetherFortStartingPrefabs, g_NetherFortStartingPrefabsCount);





cNetherFortGen::cNetherFortGen(int a_Seed, int a_GridSize, int a_MaxOffset, int a_MaxDepth) :
	super(a_Seed, a_GridSize, a_GridSize, a_MaxOffset, a_MaxOffset, a_MaxDepth * 10, a_MaxDepth * 10, 200),
	m_MaxDepth(a_MaxDepth)
{
	/*
	// DEBUG: Try one round of placement:
	cPlacedPieces Pieces;
	cBFSPieceGenerator pg(m_PiecePool, a_Seed);
	pg.PlacePieces(0, 64, 0, a_MaxDepth, Pieces);
	//*/
}





cGridStructGen::cStructurePtr cNetherFortGen::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	return cStructurePtr(new cNetherFort(*this, a_GridX, a_GridZ, a_OriginX, a_OriginZ, m_GridSizeX, m_MaxDepth, m_Seed));
}




