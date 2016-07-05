
// PrefabStructure.cpp

// Implements the cPrefabStructure class representing a cGridStructGen::cStructure descendant based on placed cPrefab instances

#include "Globals.h"
#include "PrefabStructure.h"
#include "Prefab.h"





cPrefabStructure::cPrefabStructure(
	int a_GridX, int a_GridZ,
	int a_OriginX, int a_OriginZ,
	cPlacedPieces & a_Pieces,
	cTerrainHeightGenPtr a_HeightGen
):
	Super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
	m_Pieces(a_Pieces),
	m_HeightGen(a_HeightGen)
{
}





cPrefabStructure::~cPrefabStructure()
{
	cPieceGenerator::FreePieces(m_Pieces);
}





void cPrefabStructure::DrawIntoChunk(cChunkDesc & a_Chunk)
{
	// Iterate over all items
	// Each intersecting prefab is placed on ground, if requested, then drawn
	for (cPlacedPieces::iterator itr = m_Pieces.begin(), end = m_Pieces.end(); itr != end; ++itr)
	{
		const cPrefab & Prefab = static_cast<const cPrefab &>((*itr)->GetPiece());
		if (Prefab.ShouldMoveToGround() && !(*itr)->HasBeenMovedToGround())
		{
			PlacePieceOnGround(**itr);
		}
		Prefab.Draw(a_Chunk, *itr);
	}  // for itr - m_PlacedPieces[]
}





void cPrefabStructure::PlacePieceOnGround(cPlacedPiece & a_Piece)
{
	cPiece::cConnector FirstConnector = a_Piece.GetRotatedConnector(0);
	int ChunkX, ChunkZ;
	int BlockX = FirstConnector.m_Pos.x;
	int BlockZ = FirstConnector.m_Pos.z;
	int BlockY;
	cChunkDef::AbsoluteToRelative(BlockX, BlockY, BlockZ, ChunkX, ChunkZ);
	cChunkDef::HeightMap HeightMap;
	m_HeightGen->GenHeightMap(ChunkX, ChunkZ, HeightMap);
	int TerrainHeight = cChunkDef::GetHeight(HeightMap, BlockX, BlockZ);
	a_Piece.MoveToGroundBy(TerrainHeight - FirstConnector.m_Pos.y + 1);
}




