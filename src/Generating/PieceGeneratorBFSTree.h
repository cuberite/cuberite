
// PieceGeneratorBFSTree.h

// Declares the cPieceGeneratorBFSTree class for generating structures composed of individual "pieces" in a simple tree





#pragma once

#include "PiecePool.h"
#include "../Noise/Noise.h"





class cPieceGeneratorBFSTree
{
public:
	/** Creates a new object tied to the specified PiecePool, using the specified seed. */
	cPieceGeneratorBFSTree(cPiecePool & a_PiecePool, int a_Seed);


	/** Generates a placement for pieces at the specified coords.
	The Y coord is generated automatically based on the starting piece that is chosen. */
	void PlacePieces(int a_BlockX, int a_BlockZ, int a_MaxDepth, cPlacedPieces & a_OutPieces);


protected:

	/** The type used for storing a connection from one piece to another, while building the piece tree. */
	struct cConnection
	{
		cPiece * m_Piece;                  // The piece being connected
		cPiece::cConnector m_Connector;    // The piece's connector being used (relative non-rotated coords)
		int m_NumCCWRotations;             // Number of rotations necessary to match the two connectors
		int m_Weight;                      // Relative chance that this connection will be chosen

		cConnection(cPiece & a_Piece, cPiece::cConnector & a_Connector, int a_NumCCWRotations, int a_Weight);
	};
	typedef std::vector<cConnection> cConnections;


	/** The type used for storing a pool of connectors that will be attempted to expand by another piece. */
	struct cFreeConnector
	{
		cPlacedPiece * m_Piece;
		cPiece::cConnector m_Connector;

		cFreeConnector(cPlacedPiece * a_Piece, const cPiece::cConnector & a_Connector);
	};
	typedef std::vector<cFreeConnector> cFreeConnectors;


	/** The pool from which pieces are taken. */
	cPiecePool & m_PiecePool;

	/** The noise used for random number generation. */
	cNoise m_Noise;

	/** The seed used by this generator. */
	int m_Seed;


	/** Selects a starting piece and places it, including its height and rotation.
	Also puts the piece's connectors in a_OutConnectors. */
	cPlacedPiecePtr PlaceStartingPiece(int a_BlockX, int a_BlockZ, cFreeConnectors & a_OutConnectors);

	/** Tries to place a new piece at the specified (placed) connector. Returns true if successful. */
	bool TryPlacePieceAtConnector(
		const cPlacedPiece & a_ParentPiece,      // The existing piece to a new piece should be placed
		const cPiece::cConnector & a_Connector,  // The existing connector (world-coords) to which a new piece should be placed
		cPlacedPieces & a_OutPieces,             // Already placed pieces, to be checked for intersections
		cFreeConnectors & a_OutConnectors        // List of free connectors to which the new connectors will be placed
	);

	/** Checks if the specified piece would fit with the already-placed pieces, using the specified connector
	and number of CCW rotations.
	a_ExistingConnector is in world-coords and is already rotated properly
	a_ToPos is the world-coords position on which the new connector should be placed (1 block away from a_ExistingConnector, in its Direction)
	a_NewConnector is in the original (non-rotated) coords.
	Returns true if the piece fits, false if not. */
	bool CheckConnection(
		const cPiece::cConnector & a_ExistingConnector,  // The existing connector
		const Vector3i & a_ToPos,                        // The position on which the new connector should be placed
		const cPiece & a_Piece,                          // The new piece
		const cPiece::cConnector & a_NewConnector,       // The connector of the new piece
		int a_NumCCWRotations,                           // Number of rotations for the new piece to align the connector
		const cPlacedPieces & a_OutPieces                // All the already-placed pieces to check
	);

	/** DEBUG: Outputs all the connectors in the pool into stdout.
	a_NumProcessed signals the number of connectors from the pool that should be considered processed (not listed). */
	void DebugConnectorPool(const cFreeConnectors & a_ConnectorPool, size_t a_NumProcessed);
} ;




