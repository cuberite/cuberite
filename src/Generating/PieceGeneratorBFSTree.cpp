
// PieceGeneratorBFSTree.cpp

// Implements the cPieceGeneratorBFSTree class for generating structures composed of individual "pieces" in a simple tree
/*
The generator keeps a pool of currently-open connectors, chooses one at random and tries to place a piece on it,
thus possibly extending the pool of open connectors with the new piece's ones (like breadth-first search).
*/

#include "Globals.h"
#include "PieceGeneratorBFSTree.h"
#include "VerticalStrategy.h"
#include "VerticalLimit.h"





////////////////////////////////////////////////////////////////////////////////
// cPieceGeneratorBFSTree:

cPieceGeneratorBFSTree::cPieceGeneratorBFSTree(cPiecePool & a_PiecePool, int a_Seed):
	m_PiecePool(a_PiecePool),
	m_Noise(a_Seed),
	m_Seed(a_Seed)
{
}





cPlacedPiecePtr cPieceGeneratorBFSTree::PlaceStartingPiece(int a_BlockX, int a_BlockZ, cFreeConnectors & a_OutConnectors)
{
	m_PiecePool.Reset();
	int rnd = m_Noise.IntNoise2DInt(a_BlockX, a_BlockZ) / 7;

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
		StartingPiece = StartingPieces[static_cast<size_t>(rnd) % StartingPieces.size()];
	}
	rnd = rnd >> 16;

	// Choose a random supported rotation:
	int Rotations[4] = {0};
	int NumRotations = 1;
	for (size_t i = 1; i < ARRAYCOUNT(Rotations); i++)
	{
		if (StartingPiece->CanRotateCCW(static_cast<int>(i)))
		{
			Rotations[NumRotations] = static_cast<int>(i);
			NumRotations += 1;
		}
	}
	int Rotation = Rotations[rnd % NumRotations];
	int BlockY = StartingPiece->GetStartingPieceHeight(a_BlockX, a_BlockZ);
	ASSERT(BlockY >= 0);  // The vertical strategy should have been provided and should give valid coords

	cPlacedPiece * res = new cPlacedPiece(nullptr, *StartingPiece, Vector3i(a_BlockX, BlockY, a_BlockZ), Rotation);

	// Place the piece's connectors into a_OutConnectors:
	const cPiece::cConnectors & Conn = StartingPiece->GetConnectors();
	for (cPiece::cConnectors::const_iterator itr = Conn.begin(), end = Conn.end(); itr != end; ++itr)
	{
		a_OutConnectors.push_back(
			cFreeConnector(res, StartingPiece->RotateMoveConnector(*itr, Rotation, a_BlockX, BlockY, a_BlockZ))
		);
	}

	return cPlacedPiecePtr(res);
}





bool cPieceGeneratorBFSTree::TryPlacePieceAtConnector(
	const cPlacedPiece & a_ParentPiece,
	const cPiece::cConnector & a_Connector,
	cPlacedPieces & a_OutPieces,
	cPieceGeneratorBFSTree::cFreeConnectors & a_OutConnectors
)
{
	// Get a list of available connections:
	cConnections Connections;
	int WantedConnectorType = -a_Connector.m_Type;
	cPieces AvailablePieces = m_PiecePool.GetPiecesWithConnector(WantedConnectorType);
	Connections.reserve(AvailablePieces.size());
	Vector3i ConnPos = cPiece::cConnector::AddDirection(a_Connector.m_Pos, a_Connector.m_Direction);  // The position at which the new connector should be placed - 1 block away from the current connector
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
		auto verticalLimit = (*itrP)->GetVerticalLimit();
		for (cPiece::cConnectors::iterator itrC = Connectors.begin(), endC = Connectors.end(); itrC != endC; ++itrC)
		{
			if (itrC->m_Type != WantedConnectorType)
			{
				continue;
			}
			// This is a same-type connector, find out how to rotate to it:
			int NumCCWRotations = cPiece::cConnector::GetNumCCWRotationsToFit(a_Connector.m_Direction, itrC->m_Direction);
			if ((NumCCWRotations < 0) || !(*itrP)->CanRotateCCW(NumCCWRotations))
			{
				// Doesn't support this rotation
				continue;
			}

			// Check if the piece's VerticalLimit allows this connection:
			if ((verticalLimit != nullptr) && (!verticalLimit->CanBeAtHeight(ConnPos.x, ConnPos.z, ConnPos.y - itrC->m_Pos.y)))
			{
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
	auto PlacedPiece = std::make_unique<cPlacedPiece>(&a_ParentPiece, *(Conn.m_Piece), ConnPos, Conn.m_NumCCWRotations);

	// Add the new piece's connectors to the list of free connectors:
	cPiece::cConnectors Connectors = Conn.m_Piece->GetConnectors();
	for (cPiece::cConnectors::const_iterator itr = Connectors.begin(), end = Connectors.end(); itr != end; ++itr)
	{
		if (itr->m_Pos.Equals(Conn.m_Connector.m_Pos))
		{
			// This is the connector through which we have been connected to the parent, don't add
			continue;
		}
		a_OutConnectors.push_back(cFreeConnector(PlacedPiece.get(), Conn.m_Piece->RotateMoveConnector(*itr, Conn.m_NumCCWRotations, ConnPos.x, ConnPos.y, ConnPos.z)));
	}
	a_OutPieces.push_back(std::move(PlacedPiece));

	return true;
}





bool cPieceGeneratorBFSTree::CheckConnection(
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





void cPieceGeneratorBFSTree::PlacePieces(int a_BlockX, int a_BlockZ, int a_MaxDepth, cPlacedPieces & a_OutPieces)
{
	a_OutPieces.clear();
	cFreeConnectors ConnectorPool;

	// Place the starting piece:
	a_OutPieces.push_back(PlaceStartingPiece(a_BlockX, a_BlockZ, ConnectorPool));

	/*
	// DEBUG:
	FLOGD("Placed the starting piece at {0}", Vector3i{a_BlockX, a_BlockY, a_BlockZ});
	cCuboid Hitbox = a_OutPieces[0]->GetHitBox();
	Hitbox.Sort();
	FLOGD("  Hitbox: {0} - {1} ({2} * {3} * {4})\n",
		Hitbox.p1, Hitbox.p2,
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
				FLOGD("Placed a new piece at {0}, rotation {1}\n", Coords, NewPiece->GetNumCCWRotations());
				cCuboid Hitbox = NewPiece->GetHitBox();
				Hitbox.Sort();
				FLOGD("  Hitbox: {0} - {1} ({2} * {3} * {4})\n",
					Hitbox.p1, Hitbox.p2,
					Hitbox.DifX() + 1, Hitbox.DifY() + 1, Hitbox.DifZ() + 1
				);
				DebugConnectorPool(ConnectorPool, NumProcessed + 1);
				//*/
			}
		}
		NumProcessed++;
		if (NumProcessed > 1000)
		{
			typedef cPieceGeneratorBFSTree::cFreeConnectors::difference_type difType;
			ConnectorPool.erase(ConnectorPool.begin(), ConnectorPool.begin() + static_cast<difType>(NumProcessed));
			NumProcessed = 0;
		}
	}
}





//*
// DEBUG:
void cPieceGeneratorBFSTree::DebugConnectorPool(const cPieceGeneratorBFSTree::cFreeConnectors & a_ConnectorPool, size_t a_NumProcessed)
{
	fmt::print("  Connector pool: {0} items\n", a_ConnectorPool.size() - a_NumProcessed);
	size_t idx = 0;

	typedef cPieceGeneratorBFSTree::cFreeConnectors::difference_type difType;

	for (auto itr = a_ConnectorPool.cbegin() + static_cast<difType>(a_NumProcessed), end = a_ConnectorPool.cend(); itr != end; ++itr, ++idx)
	{
		fmt::print("    {0}: {{{1}, {2}, {3}}}, type {4}, direction {5}, depth {6}\n",
			idx,
			itr->m_Connector.m_Pos.x, itr->m_Connector.m_Pos.y, itr->m_Connector.m_Pos.z,
			itr->m_Connector.m_Type,
			cPiece::cConnector::DirectionToString(itr->m_Connector.m_Direction),
			itr->m_Piece->GetDepth()
		);
	}  // for itr - a_ConnectorPool[]
}
//*/





////////////////////////////////////////////////////////////////////////////////
// cPieceGeneratorBFSTree::cConnection:

cPieceGeneratorBFSTree::cConnection::cConnection(cPiece & a_Piece, cPiece::cConnector & a_Connector, int a_NumCCWRotations, int a_Weight) :
	m_Piece(&a_Piece),
	m_Connector(a_Connector),
	m_NumCCWRotations(a_NumCCWRotations),
	m_Weight(a_Weight)
{
}





////////////////////////////////////////////////////////////////////////////////
// cPieceGeneratorBFSTree::cFreeConnector:

cPieceGeneratorBFSTree::cFreeConnector::cFreeConnector(cPlacedPiece * a_Piece, const cPiece::cConnector & a_Connector) :
	m_Piece(a_Piece),
	m_Connector(a_Connector)
{
}




