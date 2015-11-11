
// PrefabStructure.h

// Declares the cPrefabStructure class representing a cGridStructGen::cStructure descendant based on placed cPrefab instances





#pragma once

#include "GridStructGen.h"
#include "PieceGenerator.h"





class cPrefabStructure:
	public cGridStructGen::cStructure
{
	typedef cGridStructGen::cStructure Super;

public:
	cPrefabStructure(
		int a_GridX, int a_GridZ,
		int a_OriginX, int a_OriginZ,
		cPlacedPieces & a_Pieces,
		cTerrainHeightGenPtr a_HeightGen
	);

	virtual ~cPrefabStructure();

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




