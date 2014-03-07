
// PieceGenerator.cpp

// Implements the cBFSPieceGenerator class and cDFSPieceGenerator class 
// representing base classes for generating structures composed of individual "pieces"

#include "Globals.h"
#include "PieceGenerator.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPiece:

cPiece::cConnector cPiece::RotateMoveConnector(const cConnector & a_Connector, int a_NumCCWRotations, int a_MoveX, int a_MoveY, int a_MoveZ) const
{
	cPiece::cConnector res(a_Connector);
	
	// Rotate the res connector:
	Vector3i Size = GetSize();
	switch (a_NumCCWRotations)
	{
		case 0:
		{
			// No rotation needed
			break;
		}
		case 1:
		{
			// 1 CCW rotation:
			int NewX = Size.z - res.m_X;
			int NewZ = res.m_Z;
			res.m_X = NewX;
			res.m_Z = NewZ;
			res.m_Direction = RotateBlockFaceCCW(res.m_Direction);
			break;
		}
		case 2:
		{
			// 2 rotations ( = axis flip):
			res.m_X = Size.x - res.m_X;
			res.m_Z = Size.z - res.m_Z;
			res.m_Direction = MirrorBlockFaceY(res.m_Direction);
			break;
		}
		case 3:
		{
			// 1 CW rotation:
			int NewX = res.m_Z;
			int NewZ = Size.x - res.m_X;
			res.m_X = NewX;
			res.m_Z = NewZ;
			res.m_Direction = RotateBlockFaceCW(res.m_Direction);
			break;
		}
	}
	
	// Move the res connector:
	res.m_X += a_MoveX;
	res.m_Y += a_MoveY;
	res.m_Z += a_MoveZ;
	
	return res;
}





cCuboid cPiece::RotateHitBoxToConnector(
	const cPiece::cConnector & a_MyConnector,
	const cPiece::cConnector & a_ToConnector,
	int a_NumCCWRotations
) const
{
	cCuboid res = GetHitBox();
	switch (a_NumCCWRotations)
	{
		case 0:
		{
			// No rotation, return the hitbox as-is
			break;
		}
		case 1:
		{
			// 1 CCW rotation:
			// TODO: res.p1.x =
			break; 
		}
	}
	return res;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPlacedPiece:

cPlacedPiece::cPlacedPiece(const cPlacedPiece * a_Parent, const cPiece & a_Piece, const Vector3i & a_Coords, int a_NumCCWRotations) :
	m_Parent(a_Parent),
	m_Piece(&a_Piece),
	m_Coords(a_Coords),
	m_NumCCWRotations(a_NumCCWRotations)
{
	m_Depth = (m_Parent == NULL) ? 0 : (m_Parent->GetDepth() + 1);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPieceGenerator:

cPieceGenerator::cPieceGenerator(cPiecePool & a_PiecePool, int a_Seed) :
	m_PiecePool(a_PiecePool),
	m_Noise(a_Seed),
	m_Seed(a_Seed)
{
}





cPlacedPiece cPieceGenerator::PlaceStartingPiece(int a_BlockX, int a_BlockY, int a_BlockZ, cFreeConnectors & a_OutConnectors)
{
	m_PiecePool.Reset();
	int rnd = m_Noise.IntNoise3DInt(a_BlockX, a_BlockY, a_BlockZ) / 7;
	
	// Choose a random one of the starting pieces:
	cPieces StartingPieces = m_PiecePool.GetStartingPieces();
	cPiece * StartingPiece = StartingPieces[rnd % StartingPieces.size()];
	rnd = rnd >> 16;
	
	// Choose a random supported rotation:
	int Rotations[4] = {0};
	int NumRotations = 1;
	for (int i = 1; i < ARRAYCOUNT(Rotations); i++)
	{
		if (StartingPiece->CanRotateCCW(i))
		{
			Rotations[NumRotations] = i;
			NumRotations += 1;
		}
	}
	int Rotation = Rotations[rnd % NumRotations];
	
	cPlacedPiece res(NULL, *StartingPiece, Vector3i(a_BlockX, a_BlockY, a_BlockZ), Rotation);

	// Place the piece's connectors into a_OutConnectors:
	const cPiece::cConnectors & Conn = StartingPiece->GetConnectors();
	for (cPiece::cConnectors::const_iterator itr = Conn.begin(), end = Conn.end(); itr != end; ++itr)
	{
		a_OutConnectors.push_back(
			cFreeConnector(res, StartingPiece->RotateMoveConnector(*itr, Rotation, a_BlockX, a_BlockY, a_BlockZ))
		);
	}

	return res;
}





bool cPieceGenerator::TryPlacePieceAtConnector(const cPlacedPiece & a_ParentPiece, const cPiece::cConnector & a_Connector, cPlacedPieces & a_OutPieces)
{
	// Translation of direction - direction -> number of CCW rotations needed:
	// You need DirectionRotationTable[rot1][rot2] CCW turns to get from rot1 to rot2
	static const int DirectionRotationTable[6][6] =
	{
		/*         YM, YP, ZM, ZP, XM, XP
		/* YM */ { 0,  0,  0,  0,  0,  0},
		/* YP */ { 0,  0,  0,  0,  0,  0},
		/* ZM */ { 0,  0,  0,  2,  1,  3},
		/* ZP */ { 0,  0,  2,  0,  3,  1},
		/* XM */ { 0,  0,  3,  1,  0,  2},
		/* XP */ { 0,  0,  1,  3,  2,  0},
	};
	
	// Get a list of available connections:
	const int * RotTable = DirectionRotationTable[a_Connector.m_Direction];
	cConnections Connections;
	cPieces AvailablePieces = m_PiecePool.GetPiecesWithConnector(a_Connector.m_Type);
	Connections.reserve(AvailablePieces.size());
	for (cPieces::iterator itrP = AvailablePieces.begin(), endP = AvailablePieces.end(); itrP != endP; ++itrP)
	{
		cPiece::cConnectors Connectors = (*itrP)->GetConnectors();
		for (cPiece::cConnectors::iterator itrC = Connectors.begin(), endC = Connectors.end(); itrC != endC; ++itrC)
		{
			if (itrC->m_Type != a_Connector.m_Type)
			{
				continue;
			}
			// This is a same-type connector, find out how to rotate to it:
			int NumCCWRotations = RotTable[itrC->m_Direction];
			if (!(*itrP)->CanRotateCCW(NumCCWRotations))
			{
				// Doesn't support this rotation
				continue;
			}
			if (!CheckConnection(a_Connector, **itrP, *itrC, NumCCWRotations, a_OutPieces))
			{
				// Doesn't fit in this rotation
				continue;
			}
			Connections.push_back(cConnection(**itrP, *itrC, NumCCWRotations));
		}  // for itrC - Connectors[]
	}  // for itrP - AvailablePieces[]
	if (Connections.empty())
	{
		// No available connections, bail out
		return false;
	}
	
	// Choose a random connection from the list:
	int rnd = m_Noise.IntNoise3DInt(a_Connector.m_X, a_Connector.m_Y, a_Connector.m_Z) / 7;
	cConnection & Conn = Connections[rnd % Connections.size()];
	
	// Place the piece:
	cPiece::cConnector NewConnector = Conn.m_Piece->RotateMoveConnector(*(Conn.m_Connector), Conn.m_NumCCWRotations, 0, 0, 0);
	Vector3i Coords = a_ParentPiece.GetCoords();
	Coords.x -= NewConnector.m_X;
	Coords.y -= NewConnector.m_Y;
	Coords.z -= NewConnector.m_Z;
	a_OutPieces.push_back(cPlacedPiece(&a_ParentPiece, *(Conn.m_Piece), Coords, Conn.m_NumCCWRotations));
	return false;
}





bool cPieceGenerator::CheckConnection(
	const cPiece::cConnector & a_ExistingConnector,
	const cPiece & a_Piece,
	const cPiece::cConnector & a_NewConnector,
	int a_NumCCWRotations,
	const cPlacedPieces & a_OutPieces
)
{
	// For each placed piece, test the hitbox against the new piece:
	cCuboid RotatedHitBox = a_Piece.RotateHitBoxToConnector(a_NewConnector, a_ExistingConnector, a_NumCCWRotations);
	for (cPlacedPieces::const_iterator itr = a_OutPieces.begin(), end = a_OutPieces.end(); itr != end; ++itr)
	{
		if (itr->GetHitBox().DoesIntersect(RotatedHitBox))
		{
			return false;
		}
	}
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPieceGenerator::cConnection:

cPieceGenerator::cConnection::cConnection(cPiece & a_Piece, cPiece::cConnector & a_Connector, int a_NumCCWRotations) :
	m_Piece(&a_Piece),
	m_Connector(&a_Connector),
	m_NumCCWRotations(a_NumCCWRotations)
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPieceGenerator::cFreeConnector:

cPieceGenerator::cFreeConnector::cFreeConnector(cPlacedPiece & a_Piece, const cPiece::cConnector & a_Connector) :
	m_Piece(&a_Piece),
	m_Connector(a_Connector)
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBFSPieceGenerator:

cBFSPieceGenerator::cBFSPieceGenerator(cPiecePool & a_PiecePool, int a_Seed) :
	super(a_PiecePool, a_Seed)
{
}





void cBFSPieceGenerator::PlacePieces(int a_BlockX, int a_BlockY, int a_BlockZ, cPlacedPieces & a_OutPieces)
{
	a_OutPieces.clear();
	cFreeConnectors ConnectorPool;
	
	// Place the starting piece:
	a_OutPieces.push_back(PlaceStartingPiece(a_BlockX, a_BlockY, a_BlockZ, ConnectorPool));
	
	// Place pieces at the available connectors:
	/*
	Instead of removing them one by one from the pool, we process them sequentially and take note of the last
	processed one. To save on memory, once the number of processed connectors reaches a big number, a chunk
	of the connectors is removed.
	*/
	size_t NumProcessed = 0;
	while (ConnectorPool.size() > NumProcessed)
	{
		cFreeConnector & Conn = ConnectorPool[NumProcessed];
		TryPlacePieceAtConnector(*Conn.m_Piece, Conn.m_Connector, a_OutPieces);
		NumProcessed++;
		if (NumProcessed > 1000)
		{
			ConnectorPool.erase(ConnectorPool.begin(), ConnectorPool.begin() + NumProcessed);
			NumProcessed = 0;
		}
	}
}




