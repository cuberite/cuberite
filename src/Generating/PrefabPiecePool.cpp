
// PrefabPiecePool.cpp

// Implements the cPrefabPiecePool class that represents a cPiecePool descendant that uses cPrefab instances as the pieces

#include "Globals.h"
#include "PrefabPiecePool.h"





cPrefabPiecePool::cPrefabPiecePool(
	const cPrefab::sDef * a_PieceDefs,         size_t a_NumPieceDefs,
	const cPrefab::sDef * a_StartingPieceDefs, size_t a_NumStartingPieceDefs
)
{
	AddPieceDefs(a_PieceDefs, a_NumPieceDefs);
	if (a_StartingPieceDefs != NULL)
	{
		AddStartingPieceDefs(a_StartingPieceDefs, a_NumStartingPieceDefs);
	}
}





cPrefabPiecePool::~cPrefabPiecePool()
{
	Clear();
}





void cPrefabPiecePool::Clear(void)
{
	m_PiecesByConnector.clear();
	for (cPieces::iterator itr = m_AllPieces.begin(), end = m_AllPieces.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_AllPieces.clear();
	for (cPieces::iterator itr = m_StartingPieces.begin(), end = m_StartingPieces.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_StartingPieces.clear();
}





void cPrefabPiecePool::AddPieceDefs(const cPrefab::sDef * a_PieceDefs, size_t a_NumPieceDefs)
{
	ASSERT(a_PieceDefs != NULL);
	for (size_t i = 0; i < a_NumPieceDefs; i++)
	{
		cPrefab * Prefab = new cPrefab(a_PieceDefs[i]);
		m_AllPieces.push_back(Prefab);
		AddToPerConnectorMap(Prefab);
	}
}





void cPrefabPiecePool::AddStartingPieceDefs(const cPrefab::sDef * a_StartingPieceDefs, size_t a_NumStartingPieceDefs)
{
	ASSERT(a_StartingPieceDefs != NULL);
	for (size_t i = 0; i < a_NumStartingPieceDefs; i++)
	{
		cPrefab * Prefab = new cPrefab(a_StartingPieceDefs[i]);
		m_StartingPieces.push_back(Prefab);
	}
}





void cPrefabPiecePool::AddToPerConnectorMap(cPrefab * a_Prefab)
{
	cPiece::cConnectors Connectors = ((const cPiece *)a_Prefab)->GetConnectors();
	for (cPiece::cConnectors::const_iterator itr = Connectors.begin(), end = Connectors.end(); itr != end; ++itr)
	{
		m_PiecesByConnector[itr->m_Type].push_back(a_Prefab);
	}
}




cPieces cPrefabPiecePool::GetPiecesWithConnector(int a_ConnectorType)
{
	return m_PiecesByConnector[a_ConnectorType];
}





cPieces cPrefabPiecePool::GetStartingPieces(void)
{
	if (m_StartingPieces.empty())
	{
		return m_AllPieces;
	}
	else
	{
		return m_StartingPieces;
	}
}





int cPrefabPiecePool::GetPieceWeight(const cPlacedPiece & a_PlacedPiece, const cPiece::cConnector & a_ExistingConnector, const cPiece & a_NewPiece)
{
	return ((const cPrefab &)a_NewPiece).GetPieceWeight(a_PlacedPiece, a_ExistingConnector);
}





int cPrefabPiecePool::GetStartingPieceWeight(const cPiece & a_NewPiece)
{
	return ((const cPrefab &)a_NewPiece).GetDefaultWeight();
}





void cPrefabPiecePool::PiecePlaced(const cPiece & a_Piece)
{
	// Do nothing
	UNUSED(a_Piece);
}





void cPrefabPiecePool::Reset(void)
{
	// Do nothing
}




