
// PrefabStructure.cpp

// Implements the cPrefabStructure class representing a cGridStructGen::cStructure descendant based on placed cPrefab instances

#include "Globals.h"
#include "PrefabStructure.h"
#include "Prefab.h"
#include "../BlockEntities/ChestEntity.h"





cPrefabStructure::cPrefabStructure(
	int a_GridX, int a_GridZ,
	int a_OriginX, int a_OriginZ,
	cPlacedPieces && a_Pieces,
	cTerrainHeightGenPtr a_HeightGen
):
	Super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
	m_Pieces(std::move(a_Pieces)),
	m_HeightGen(std::move(a_HeightGen))
{
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
		Prefab.Draw(a_Chunk, itr->get());
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
	m_HeightGen->GenHeightMap({ChunkX, ChunkZ}, HeightMap);
	int TerrainHeight = cChunkDef::GetHeight(HeightMap, BlockX, BlockZ);
	a_Piece.MoveToGroundBy(TerrainHeight - FirstConnector.m_Pos.y + 1);
}





cPrefabChestStructure::cPrefabChestStructure(
	int a_GridX, int a_GridZ,
	int a_OriginX, int a_OriginZ,
	cPlacedPieces && a_Pieces,
	cTerrainHeightGenPtr a_HeightGen,
	const cLootProbab * a_LootProbab
) :
	Super(a_GridX, a_GridZ, a_OriginX, a_OriginZ, std::move(a_Pieces), std::move(a_HeightGen))
{
	m_LootProbab = a_LootProbab;
}





void cPrefabChestStructure::DrawIntoChunk(cChunkDesc &a_Chunk)
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
		Prefab.Draw(a_Chunk, itr->get());

		// Fills all Chests with loot

		auto Hitbox = itr->get()->GetHitBox();

		// Find all chests in the Hitbox
		for (int X = Hitbox.p1.x; X < Hitbox.p2.x; X++)
		{
			for (int Y = Hitbox.p1.y; Y < Hitbox.p2.y; Y++)
			{
				for (int Z = Hitbox.p1.z; Z < Hitbox.p2.z; Z++)
				{
					if (
						(X >= 0) && (X < cChunkDef::Width) &&
						(Y >= 0) && (Y < cChunkDef::Height) &&
						(Z >= 0) && (Z < cChunkDef::Width)
						)
					{
						cChestEntity * ChestEntity = static_cast<cChestEntity *>(a_Chunk.GetBlockEntity(X, Y, Z));
						ASSERT((ChestEntity != nullptr) && (ChestEntity->GetBlockType() == E_BLOCK_CHEST));
						cNoise Noise(a_Chunk.GetChunkX() ^ a_Chunk.GetChunkZ());
						int NumSlots = 3 + ((Noise.IntNoise3DInt(X, Y, Z) / 11) % 4);
						int Seed = Noise.IntNoise3DInt(X, Y, Z);
						ChestEntity->GetContents().GenerateRandomLootWithBooks(m_LootProbab, ARRAYCOUNT(m_LootProbab), NumSlots, Seed);
					}
				}
			}
		}
	}  // for itr - m_PlacedPieces[]
}
