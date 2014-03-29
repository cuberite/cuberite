
// NetherFortGen.cpp

// Implements the cNetherFortGen class representing the nether fortress generator

#include "Globals.h"
#include "NetherFortGen.h"
#include "Prefabs/NetherFortPrefabs.h"





static const int NEIGHBORHOOD_SIZE = 3;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNetherFortGen::cNetherFort:

class cNetherFortGen::cNetherFort
{
public:
	cNetherFortGen & m_ParentGen;
	int m_BlockX, m_BlockZ;
	int m_GridSize;
	int m_Seed;
	cPlacedPieces m_Pieces;


	cNetherFort(cNetherFortGen & a_ParentGen, int a_BlockX, int a_BlockZ, int a_GridSize, int a_MaxDepth, int a_Seed) :
		m_ParentGen(a_ParentGen),
		m_BlockX(a_BlockX),
		m_BlockZ(a_BlockZ),
		m_GridSize(a_GridSize),
		m_Seed(a_Seed)
	{
		// TODO: Proper Y-coord placement
		int BlockY = 64;
		
		// Generate pieces:
		for (int i = 0; m_Pieces.size() < (size_t)(a_MaxDepth * a_MaxDepth / 16 + a_MaxDepth); i++)
		{
			cBFSPieceGenerator pg(m_ParentGen, a_Seed + 1);
			pg.PlacePieces(a_BlockX, BlockY, a_BlockZ, a_MaxDepth, m_Pieces);
		}
	}

	
	~cNetherFort()
	{
		cPieceGenerator::FreePieces(m_Pieces);
	}
	
		
	/** Carves the system into the chunk data */
	void ProcessChunk(cChunkDesc & a_Chunk)
	{
		for (cPlacedPieces::const_iterator itr = m_Pieces.begin(), end = m_Pieces.end(); itr != end; ++itr)
		{
			const cPrefab & Prefab = (const cPrefab &)((*itr)->GetPiece());
			Prefab.Draw(a_Chunk, *itr);
		}  // for itr - m_PlacedPieces[]
	}
};





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNetherFortGen:

cNetherFortGen::cNetherFortGen(int a_Seed, int a_GridSize, int a_MaxDepth) :
	m_Seed(a_Seed),
	m_Noise(a_Seed),
	m_GridSize(a_GridSize),
	m_MaxDepth(a_MaxDepth)
{
	// Initialize the prefabs:
	for (size_t i = 0; i < g_NetherFortPrefabs1Count; i++)
	{
		cPrefab * Prefab = new cPrefab(g_NetherFortPrefabs1[i]);
		m_AllPieces.push_back(Prefab);
		if (Prefab->HasConnectorType(0))
		{
			m_OuterPieces.push_back(Prefab);
		}
		if (Prefab->HasConnectorType(1))
		{
			m_InnerPieces.push_back(Prefab);
		}
	}
	
	// Initialize the starting piece prefabs:
	for (size_t i = 0; i < g_NetherFortStartingPrefabs1Count; i++)
	{
		m_StartingPieces.push_back(new cPrefab(g_NetherFortStartingPrefabs1[i]));
	}

	// DEBUG: Try one round of placement:
	cPlacedPieces Pieces;
	cBFSPieceGenerator pg(*this, a_Seed);
	pg.PlacePieces(0, 64, 0, a_MaxDepth, Pieces);
}





cNetherFortGen::~cNetherFortGen()
{
	ClearCache();
	for (cPieces::iterator itr = m_AllPieces.begin(), end = m_AllPieces.end(); itr != end; ++itr)
	{
		delete *itr;
	}  // for itr - m_AllPieces[]
	m_AllPieces.clear();
}





void cNetherFortGen::ClearCache(void)
{
	// TODO
}





void cNetherFortGen::GetFortsForChunk(int a_ChunkX, int a_ChunkZ, cNetherForts & a_Forts)
{
	int BaseX = a_ChunkX * cChunkDef::Width / m_GridSize;
	int BaseZ = a_ChunkZ * cChunkDef::Width / m_GridSize;
	if (BaseX < 0)
	{
		--BaseX;
	}
	if (BaseZ < 0)
	{
		--BaseZ;
	}
	BaseX -= NEIGHBORHOOD_SIZE / 2;
	BaseZ -= NEIGHBORHOOD_SIZE / 2;

	// Walk the cache, move each cave system that we want into a_Forts:
	int StartX = BaseX * m_GridSize;
	int EndX = (BaseX + NEIGHBORHOOD_SIZE + 1) * m_GridSize;
	int StartZ = BaseZ * m_GridSize;
	int EndZ = (BaseZ + NEIGHBORHOOD_SIZE + 1) * m_GridSize;
	for (cNetherForts::iterator itr = m_Cache.begin(), end = m_Cache.end(); itr != end;)
	{
		if (
			((*itr)->m_BlockX >= StartX) && ((*itr)->m_BlockX < EndX) &&
			((*itr)->m_BlockZ >= StartZ) && ((*itr)->m_BlockZ < EndZ)
		)
		{
			// want
			a_Forts.push_back(*itr);
			itr = m_Cache.erase(itr);
		}
		else
		{
			// don't want
			++itr;
		}
	}  // for itr - m_Cache[]

	// Create those forts that haven't been in the cache:
	for (int x = 0; x < NEIGHBORHOOD_SIZE; x++)
	{
		int RealX = (BaseX + x) * m_GridSize;
		for (int z = 0; z < NEIGHBORHOOD_SIZE; z++)
		{
			int RealZ = (BaseZ + z) * m_GridSize;
			bool Found = false;
			for (cNetherForts::const_iterator itr = a_Forts.begin(), end = a_Forts.end(); itr != end; ++itr)
			{
				if (((*itr)->m_BlockX == RealX) && ((*itr)->m_BlockZ == RealZ))
				{
					Found = true;
					break;
				}
			}  // for itr - a_Mineshafts
			if (!Found)
			{
				a_Forts.push_back(new cNetherFort(*this, RealX, RealZ, m_GridSize, m_MaxDepth, m_Seed));
			}
		}  // for z
	}  // for x

	// Copy a_Forts into m_Cache to the beginning:
	cNetherForts FortsCopy (a_Forts);
	m_Cache.splice(m_Cache.begin(), FortsCopy, FortsCopy.begin(), FortsCopy.end());

	// Trim the cache if it's too long:
	if (m_Cache.size() > 100)
	{
		cNetherForts::iterator itr = m_Cache.begin();
		std::advance(itr, 100);
		for (cNetherForts::iterator end = m_Cache.end(); itr != end; ++itr)
		{
			delete *itr;
		}
		itr = m_Cache.begin();
		std::advance(itr, 100);
		m_Cache.erase(itr, m_Cache.end());
	}
}





void cNetherFortGen::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	cNetherForts Forts;
	GetFortsForChunk(ChunkX, ChunkZ, Forts);
	for (cNetherForts::const_iterator itr = Forts.begin(); itr != Forts.end(); ++itr)
	{
		(*itr)->ProcessChunk(a_ChunkDesc);
	}  // for itr - Forts[]
}





cPieces cNetherFortGen::GetPiecesWithConnector(int a_ConnectorType)
{
	switch (a_ConnectorType)
	{
		case 0: return m_OuterPieces;
		case 1: return m_InnerPieces;
		default: return cPieces();
	}
}





cPieces cNetherFortGen::GetStartingPieces(void)
{
	return m_StartingPieces;
}





void cNetherFortGen::PiecePlaced(const cPiece & a_Piece)
{
	UNUSED(a_Piece);
}





void cNetherFortGen::Reset(void)
{
	// Nothing needed
}




