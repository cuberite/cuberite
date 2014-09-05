
// PieceGenerator.cpp

// Implements the cBFSPieceGenerator class and cDFSPieceGenerator class
// representing base classes for generating structures composed of individual "pieces"

#include "Globals.h"
#include "PieceGenerator.h"





#ifdef SELF_TEST

////////////////////////////////////////////////////////////////////////////////
// Self-test:

static class cPieceGeneratorSelfTest :
	public cPiecePool
{
public:
	cPieceGeneratorSelfTest(void)
	{
		// Prepare the internal state:
		InitializePieces();
		
		// Generate:
		cBFSPieceGenerator Gen(*this, 0);
		cPlacedPieces OutPieces;
		Gen.PlacePieces(500, 50, 500, 3, OutPieces);
		
		// Print out the pieces:
		printf("OutPieces.size() = " SIZE_T_FMT "\n", OutPieces.size());
		size_t idx = 0;
		for (cPlacedPieces::const_iterator itr = OutPieces.begin(), end = OutPieces.end(); itr != end; ++itr, ++idx)
		{
			const Vector3i & Coords = (*itr)->GetCoords();
			cCuboid Hitbox = (*itr)->GetHitBox();
			Hitbox.Sort();
			printf(SIZE_T_FMT ": {%d, %d, %d}, rot %d, hitbox {%d, %d, %d} - {%d, %d, %d} (%d * %d * %d)\n", idx,
				Coords.x, Coords.y, Coords.z,
				(*itr)->GetNumCCWRotations(),
				Hitbox.p1.x, Hitbox.p1.y, Hitbox.p1.z,
				Hitbox.p2.x, Hitbox.p2.y, Hitbox.p2.z,
				Hitbox.DifX() + 1, Hitbox.DifY() + 1, Hitbox.DifZ() + 1
			);
		}  // itr - OutPieces[]
		printf("Done.\n");
		
		// Free the placed pieces properly:
		Gen.FreePieces(OutPieces);
	}
	
	~cPieceGeneratorSelfTest()
	{
		// Dealloc all the pieces:
		for (cPieces::iterator itr = m_Pieces.begin(), end = m_Pieces.end(); itr != end; ++itr)
		{
			delete *itr;
		}
		m_Pieces.clear();
	}
	
protected:
	class cTestPiece :
		public cPiece
	{
		int m_Size;
	public:
		cTestPiece(int a_Size) :
			m_Size(a_Size)
		{
		}
		
		virtual cConnectors GetConnectors(void) const override
		{
			// Each piece has 4 connectors, one of each type, plus one extra, at the center of its walls:
			cConnectors res;
			res.push_back(cConnector(m_Size / 2, 1, 0,          0, BLOCK_FACE_ZM));
			res.push_back(cConnector(m_Size / 2, 1, m_Size - 1, 1, BLOCK_FACE_ZP));
			res.push_back(cConnector(0,          1, m_Size / 2, 2, BLOCK_FACE_XM));
			res.push_back(cConnector(m_Size - 1, 1, m_Size / 2, m_Size % 3, BLOCK_FACE_XP));
			return res;
		}
		
		virtual Vector3i GetSize(void) const override
		{
			return Vector3i(m_Size, 5, m_Size);
		}
		
		virtual cCuboid GetHitBox(void) const override
		{
			return cCuboid(0, 0, 0, m_Size - 1, 4, m_Size - 1);
		}
		
		virtual bool CanRotateCCW(int a_NumCCWRotations) const override
		{
			return true;
		}
	};
	
	cPieces m_Pieces;
	
	virtual cPieces GetPiecesWithConnector(int a_ConnectorType) override
	{
		// Each piece contains each connector
		return m_Pieces;
	}
	
	
	virtual cPieces GetStartingPieces(void) override
	{
		return m_Pieces;
	}
	
	
	virtual void PiecePlaced(const cPiece & a_Piece) override
	{
		UNUSED(a_Piece);
	}
	
	
	virtual void Reset(void) override
	{
	}
	
	
	void InitializePieces(void)
	{
		m_Pieces.push_back(new cTestPiece(5));
		m_Pieces.push_back(new cTestPiece(7));
		m_Pieces.push_back(new cTestPiece(9));
	}
} g_Test;

#endif  // SELF_TEST





////////////////////////////////////////////////////////////////////////////////
// cPiece:

	
Vector3i cPiece::RotatePos(const Vector3i & a_Pos, int a_NumCCWRotations) const
{
	Vector3i Size = GetSize();
	switch (a_NumCCWRotations)
	{
		case 0:
		{
			// No rotation needed
			return a_Pos;
		}
		case 1:
		{
			// 1 CCW rotation:
			return Vector3i(a_Pos.z, a_Pos.y, Size.x - a_Pos.x - 1);
		}
		case 2:
		{
			// 2 rotations ( = axis flip):
			return Vector3i(Size.x - a_Pos.x - 1, a_Pos.y, Size.z - a_Pos.z - 1);
		}
		case 3:
		{
			// 1 CW rotation:
			return Vector3i(Size.z - a_Pos.z - 1, a_Pos.y, a_Pos.x);
		}
	}
	ASSERT(!"Unhandled rotation");
	return a_Pos;
}





cPiece::cConnector cPiece::RotateMoveConnector(const cConnector & a_Connector, int a_NumCCWRotations, int a_MoveX, int a_MoveY, int a_MoveZ) const
{
	cPiece::cConnector res(a_Connector);
	
	// Rotate the res connector:
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
			res.m_Direction = RotateBlockFaceCCW(res.m_Direction);
			break;
		}
		case 2:
		{
			// 2 rotations ( = axis flip):
			res.m_Direction = MirrorBlockFaceY(res.m_Direction);
			break;
		}
		case 3:
		{
			// 1 CW rotation:
			res.m_Direction = RotateBlockFaceCW(res.m_Direction);
			break;
		}
	}
	res.m_Pos = RotatePos(a_Connector.m_Pos, a_NumCCWRotations);
	
	// Move the res connector:
	res.m_Pos.x += a_MoveX;
	res.m_Pos.y += a_MoveY;
	res.m_Pos.z += a_MoveZ;
	
	return res;
}





cCuboid cPiece::RotateHitBoxToConnector(
	const cPiece::cConnector & a_MyConnector,
	const Vector3i & a_ToConnectorPos,
	int a_NumCCWRotations
) const
{
	ASSERT(a_NumCCWRotations == (a_NumCCWRotations % 4));
	Vector3i ConnPos = RotatePos(a_MyConnector.m_Pos, a_NumCCWRotations);
	ConnPos = a_ToConnectorPos - ConnPos;
	return RotateMoveHitBox(a_NumCCWRotations, ConnPos.x, ConnPos.y, ConnPos.z);
}





cCuboid cPiece::RotateMoveHitBox(int a_NumCCWRotations, int a_MoveX, int a_MoveY, int a_MoveZ) const
{
	ASSERT(a_NumCCWRotations == (a_NumCCWRotations % 4));
	cCuboid res = GetHitBox();
	res.p1 = RotatePos(res.p1, a_NumCCWRotations);
	res.p2 = RotatePos(res.p2, a_NumCCWRotations);
	res.p1.Move(a_MoveX, a_MoveY, a_MoveZ);
	res.p2.Move(a_MoveX, a_MoveY, a_MoveZ);
	return res;
}





////////////////////////////////////////////////////////////////////////////////
// cPiece::cConnector:

cPiece::cConnector::cConnector(int a_X, int a_Y, int a_Z, int a_Type, eBlockFace a_Direction) :
	m_Pos(a_X, a_Y, a_Z),
	m_Type(a_Type),
	m_Direction(a_Direction)
{
}





cPiece::cConnector::cConnector(const Vector3i & a_Pos, int a_Type, eBlockFace a_Direction) :
	m_Pos(a_Pos),
	m_Type(a_Type),
	m_Direction(a_Direction)
{
}





////////////////////////////////////////////////////////////////////////////////
// cPlacedPiece:

cPlacedPiece::cPlacedPiece(const cPlacedPiece * a_Parent, const cPiece & a_Piece, const Vector3i & a_Coords, int a_NumCCWRotations) :
	m_Parent(a_Parent),
	m_Piece(&a_Piece),
	m_Coords(a_Coords),
	m_NumCCWRotations(a_NumCCWRotations),
	m_HasBeenMovedToGround(false)
{
	m_Depth = (m_Parent == NULL) ? 0 : (m_Parent->GetDepth() + 1);
	m_HitBox = a_Piece.RotateMoveHitBox(a_NumCCWRotations, a_Coords.x, a_Coords.y, a_Coords.z);
	m_HitBox.Sort();
}





cPiece::cConnector cPlacedPiece::GetRotatedConnector(size_t a_Index) const
{
	cPiece::cConnectors Connectors = m_Piece->GetConnectors();
	ASSERT(Connectors.size() >= a_Index);
	return m_Piece->RotateMoveConnector(Connectors[a_Index], m_NumCCWRotations, m_Coords.x, m_Coords.y, m_Coords.z);
}





cPiece::cConnector cPlacedPiece::GetRotatedConnector(const cPiece::cConnector & a_Connector) const
{
	return m_Piece->RotateMoveConnector(a_Connector, m_NumCCWRotations, m_Coords.x, m_Coords.y, m_Coords.z);
}





void cPlacedPiece::MoveToGroundBy(int a_OffsetY)
{
	m_Coords.y += a_OffsetY;
	m_HasBeenMovedToGround = true;
}





////////////////////////////////////////////////////////////////////////////////
// cPieceGenerator:

cPieceGenerator::cPieceGenerator(cPiecePool & a_PiecePool, int a_Seed) :
	m_PiecePool(a_PiecePool),
	m_Noise(a_Seed),
	m_Seed(a_Seed)
{
}





void cPieceGenerator::FreePieces(cPlacedPieces & a_PlacedPieces)
{
	for (cPlacedPieces::iterator itr = a_PlacedPieces.begin(), end = a_PlacedPieces.end(); itr != end; ++itr)
	{
		delete *itr;
	}  // for itr - a_PlacedPieces[]
	a_PlacedPieces.clear();
}





cPlacedPiece * cPieceGenerator::PlaceStartingPiece(int a_BlockX, int a_BlockY, int a_BlockZ, cFreeConnectors & a_OutConnectors)
{
	m_PiecePool.Reset();
	int rnd = m_Noise.IntNoise3DInt(a_BlockX, a_BlockY, a_BlockZ) / 7;
	
	// Choose a random one of the starting pieces:
	cPieces StartingPieces = m_PiecePool.GetStartingPieces();
	int Total = 0;
	for (cPieces::const_iterator itr = StartingPieces.begin(), end = StartingPieces.end(); itr != end; ++itr)
	{
		Total += m_PiecePool.GetStartingPieceWeight(**itr);
	}
	cPiece * StartingPiece;
	if (Total > 0)
	{
		int Chosen = rnd % Total;
		StartingPiece = StartingPieces.front();
		for (cPieces::const_iterator itr = StartingPieces.begin(), end = StartingPieces.end(); itr != end; ++itr)
		{
			Chosen -= m_PiecePool.GetStartingPieceWeight(**itr);
			if (Chosen <= 0)
			{
				StartingPiece = *itr;
				break;
			}
		}
	}
	else
	{
		// All pieces returned zero weight, but we need one to start. Choose with equal chance:
		StartingPiece = StartingPieces[rnd % StartingPieces.size()];
	}
	rnd = rnd >> 16;
	
	// Choose a random supported rotation:
	int Rotations[4] = {0};
	int NumRotations = 1;
	for (size_t i = 1; i < ARRAYCOUNT(Rotations); i++)
	{
		if (StartingPiece->CanRotateCCW((int)i))
		{
			Rotations[NumRotations] = (int)i;
			NumRotations += 1;
		}
	}
	int Rotation = Rotations[rnd % NumRotations];
	
	cPlacedPiece * res = new cPlacedPiece(NULL, *StartingPiece, Vector3i(a_BlockX, a_BlockY, a_BlockZ), Rotation);

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





bool cPieceGenerator::TryPlacePieceAtConnector(
	const cPlacedPiece & a_ParentPiece,
	const cPiece::cConnector & a_Connector,
	cPlacedPieces & a_OutPieces,
	cPieceGenerator::cFreeConnectors & a_OutConnectors
)
{
	// Translation of direction - direction -> number of CCW rotations needed:
	// You need DirectionRotationTable[rot1][rot2] CCW turns to connect rot1 to rot2 (they are opposite)
	static const int DirectionRotationTable[6][6] =
	{
		/*         YM, YP, ZM, ZP, XM, XP */
		/* YM */ { 0,  0,  0,  0,  0,  0},
		/* YP */ { 0,  0,  0,  0,  0,  0},
		/* ZM */ { 0,  0,  2,  0,  1,  3},
		/* ZP */ { 0,  0,  0,  2,  3,  1},
		/* XM */ { 0,  0,  3,  1,  2,  0},
		/* XP */ { 0,  0,  1,  3,  0,  2},
	};
	
	// Get a list of available connections:
	const int * RotTable = DirectionRotationTable[a_Connector.m_Direction];
	cConnections Connections;
	int WantedConnectorType = -a_Connector.m_Type;
	cPieces AvailablePieces = m_PiecePool.GetPiecesWithConnector(WantedConnectorType);
	Connections.reserve(AvailablePieces.size());
	Vector3i ConnPos = a_Connector.m_Pos;  // The position at which the new connector should be placed - 1 block away from the connector
	AddFaceDirection(ConnPos.x, ConnPos.y, ConnPos.z, a_Connector.m_Direction);
	int WeightTotal = 0;
	for (cPieces::iterator itrP = AvailablePieces.begin(), endP = AvailablePieces.end(); itrP != endP; ++itrP)
	{
		// Get the relative chance of this piece being generated in this path:
		int Weight = m_PiecePool.GetPieceWeight(a_ParentPiece, a_Connector, **itrP);
		if (Weight <= 0)
		{
			continue;
		}
		
		// Try fitting each of the piece's connector:
		cPiece::cConnectors Connectors = (*itrP)->GetConnectors();
		for (cPiece::cConnectors::iterator itrC = Connectors.begin(), endC = Connectors.end(); itrC != endC; ++itrC)
		{
			if (itrC->m_Type != WantedConnectorType)
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
			if (!CheckConnection(a_Connector, ConnPos, **itrP, *itrC, NumCCWRotations, a_OutPieces))
			{
				// Doesn't fit in this rotation
				continue;
			}
			// Fits, add it to list of possibile connections:
			Connections.push_back(cConnection(**itrP, *itrC, NumCCWRotations, Weight));
			WeightTotal += Weight;
		}  // for itrC - Connectors[]
	}  // for itrP - AvailablePieces[]
	if (Connections.empty())
	{
		// No available connections, bail out
		return false;
	}
	ASSERT(WeightTotal > 0);
	
	// Choose a random connection from the list, based on the weights:
	int rnd = (m_Noise.IntNoise3DInt(a_Connector.m_Pos.x, a_Connector.m_Pos.y, a_Connector.m_Pos.z) / 7) % WeightTotal;
	size_t ChosenIndex = 0;
	for (cConnections::const_iterator itr = Connections.begin(), end = Connections.end(); itr != end; ++itr, ++ChosenIndex)
	{
		rnd -= itr->m_Weight;
		if (rnd <= 0)
		{
			// This is the piece to choose
			break;
		}
	}
	cConnection & Conn = Connections[ChosenIndex];
	
	// Place the piece:
	Vector3i NewPos = Conn.m_Piece->RotatePos(Conn.m_Connector.m_Pos, Conn.m_NumCCWRotations);
	ConnPos -= NewPos;
	cPlacedPiece * PlacedPiece = new cPlacedPiece(&a_ParentPiece, *(Conn.m_Piece), ConnPos, Conn.m_NumCCWRotations);
	a_OutPieces.push_back(PlacedPiece);
	
	// Add the new piece's connectors to the list of free connectors:
	cPiece::cConnectors Connectors = Conn.m_Piece->GetConnectors();
	for (cPiece::cConnectors::const_iterator itr = Connectors.begin(), end = Connectors.end(); itr != end; ++itr)
	{
		if (itr->m_Pos.Equals(Conn.m_Connector.m_Pos))
		{
			// This is the connector through which we have been connected to the parent, don't add
			continue;
		}
		a_OutConnectors.push_back(cFreeConnector(PlacedPiece, Conn.m_Piece->RotateMoveConnector(*itr, Conn.m_NumCCWRotations, ConnPos.x, ConnPos.y, ConnPos.z)));
	}
	
	return true;
}





bool cPieceGenerator::CheckConnection(
	const cPiece::cConnector & a_ExistingConnector,
	const Vector3i & a_ToPos,
	const cPiece & a_Piece,
	const cPiece::cConnector & a_NewConnector,
	int a_NumCCWRotations,
	const cPlacedPieces & a_OutPieces
)
{
	// For each placed piece, test the hitbox against the new piece:
	cCuboid RotatedHitBox = a_Piece.RotateHitBoxToConnector(a_NewConnector, a_ToPos, a_NumCCWRotations);
	RotatedHitBox.Sort();
	for (cPlacedPieces::const_iterator itr = a_OutPieces.begin(), end = a_OutPieces.end(); itr != end; ++itr)
	{
		if ((*itr)->GetHitBox().DoesIntersect(RotatedHitBox))
		{
			return false;
		}
	}
	return true;
}





//*
// DEBUG:
void cPieceGenerator::DebugConnectorPool(const cPieceGenerator::cFreeConnectors & a_ConnectorPool, size_t a_NumProcessed)
{
	printf("  Connector pool: " SIZE_T_FMT " items\n", a_ConnectorPool.size() - a_NumProcessed);
	size_t idx = 0;
	for (cPieceGenerator::cFreeConnectors::const_iterator itr = a_ConnectorPool.begin() + a_NumProcessed, end = a_ConnectorPool.end(); itr != end; ++itr, ++idx)
	{
		printf("    " SIZE_T_FMT ": {%d, %d, %d}, type %d, direction %s, depth %d\n",
			idx,
			itr->m_Connector.m_Pos.x, itr->m_Connector.m_Pos.y, itr->m_Connector.m_Pos.z,
			itr->m_Connector.m_Type,
			BlockFaceToString(itr->m_Connector.m_Direction).c_str(),
			itr->m_Piece->GetDepth()
		);
	}  // for itr - a_ConnectorPool[]
}
//*/





////////////////////////////////////////////////////////////////////////////////
// cPieceGenerator::cConnection:

cPieceGenerator::cConnection::cConnection(cPiece & a_Piece, cPiece::cConnector & a_Connector, int a_NumCCWRotations, int a_Weight) :
	m_Piece(&a_Piece),
	m_Connector(a_Connector),
	m_NumCCWRotations(a_NumCCWRotations),
	m_Weight(a_Weight)
{
}





////////////////////////////////////////////////////////////////////////////////
// cPieceGenerator::cFreeConnector:

cPieceGenerator::cFreeConnector::cFreeConnector(cPlacedPiece * a_Piece, const cPiece::cConnector & a_Connector) :
	m_Piece(a_Piece),
	m_Connector(a_Connector)
{
}





////////////////////////////////////////////////////////////////////////////////
// cBFSPieceGenerator:

cBFSPieceGenerator::cBFSPieceGenerator(cPiecePool & a_PiecePool, int a_Seed) :
	super(a_PiecePool, a_Seed)
{
}





void cBFSPieceGenerator::PlacePieces(int a_BlockX, int a_BlockY, int a_BlockZ, int a_MaxDepth, cPlacedPieces & a_OutPieces)
{
	a_OutPieces.clear();
	cFreeConnectors ConnectorPool;
	
	// Place the starting piece:
	a_OutPieces.push_back(PlaceStartingPiece(a_BlockX, a_BlockY, a_BlockZ, ConnectorPool));
	
	/*
	// DEBUG:
	printf("Placed the starting piece at {%d, %d, %d}\n", a_BlockX, a_BlockY, a_BlockZ);
	cCuboid Hitbox = a_OutPieces[0]->GetHitBox();
	Hitbox.Sort();
	printf("  Hitbox: {%d, %d, %d} - {%d, %d, %d} (%d * %d * %d)\n",
		Hitbox.p1.x, Hitbox.p1.y, Hitbox.p1.z,
		Hitbox.p2.x, Hitbox.p2.y, Hitbox.p2.z,
		Hitbox.DifX() + 1, Hitbox.DifY() + 1, Hitbox.DifZ() + 1
	);
	DebugConnectorPool(ConnectorPool, 0);
	//*/
	
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
		if (Conn.m_Piece->GetDepth() < a_MaxDepth)
		{
			if (TryPlacePieceAtConnector(*Conn.m_Piece, Conn.m_Connector, a_OutPieces, ConnectorPool))
			{
				/*
				// DEBUG:
				const cPlacedPiece * NewPiece = a_OutPieces.back();
				const Vector3i & Coords = NewPiece->GetCoords();
				printf("Placed a new piece at {%d, %d, %d}, rotation %d\n", Coords.x, Coords.y, Coords.z, NewPiece->GetNumCCWRotations());
				cCuboid Hitbox = NewPiece->GetHitBox();
				Hitbox.Sort();
				printf("  Hitbox: {%d, %d, %d} - {%d, %d, %d} (%d * %d * %d)\n",
					Hitbox.p1.x, Hitbox.p1.y, Hitbox.p1.z,
					Hitbox.p2.x, Hitbox.p2.y, Hitbox.p2.z,
					Hitbox.DifX() + 1, Hitbox.DifY() + 1, Hitbox.DifZ() + 1
				);
				DebugConnectorPool(ConnectorPool, NumProcessed + 1);
				//*/
			}
		}
		NumProcessed++;
		if (NumProcessed > 1000)
		{
			ConnectorPool.erase(ConnectorPool.begin(), ConnectorPool.begin() + NumProcessed);
			NumProcessed = 0;
		}
	}
}




