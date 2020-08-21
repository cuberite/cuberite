
// PrefabStructure.h

// Declares the cPrefabStructure class representing a cGridStructGen::cStructure descendant based on placed cPrefab instances





#pragma once

#include "GridStructGen.h"
#include "PiecePool.h"
#include "../Item.h"





class cPrefabStructure:
	public cGridStructGen::cStructure
{
	using Super = cGridStructGen::cStructure;

public:

	cPrefabStructure(
		int a_GridX, int a_GridZ,
		int a_OriginX, int a_OriginZ,
		cPlacedPieces && a_Pieces,
		cTerrainHeightGenPtr a_HeightGen
	);

protected:
	/** The pieces placed by the generator. */
	cPlacedPieces m_Pieces;

	/** The height generator used when adjusting pieces onto the ground. */
	cTerrainHeightGenPtr m_HeightGen;


	// cGridStructGen::cStructure overrides:
	virtual void DrawIntoChunk(cChunkDesc & a_Chunk) override;

	/**  Adjusts the Y coord of the given piece so that the piece is on the ground.
	Ground level is assumed to be represented by the first connector in the piece. */
	void PlacePieceOnGround(cPlacedPiece & a_Piece);
};




/** Descendant of cPrefabStructure containing chests that will be filled */
class cPrefabChestStructure:
	public cPrefabStructure
{
	using Super = cPrefabStructure;

public:
	cPrefabChestStructure(
		int a_GridX, int a_GridZ,
		int a_OriginX, int a_OriginZ,
		cPlacedPieces && a_Pieces,
		cTerrainHeightGenPtr a_HeightGen,
		const cLootProbab * a_LootProbab
	);

protected:
	// cGridStructGen::cStructure overrides:
	virtual void DrawIntoChunk(cChunkDesc & a_Chunk) override;

	const cLootProbab * m_LootProbab;
};

