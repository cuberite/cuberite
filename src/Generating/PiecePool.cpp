// PiecePool.cpp

// Implements the cPiecePool class representing a pool of cPieces - "parts" of a structure, used in piece-generators
// A cPiece is a single static part of a structure that can rotate around the Y axis, has connectors to other pieces and knows how to draw itself into the world.
// The pool manages the pieces and provides lists of its pieces matching criteria, and provides relative weights for the random distribution of pieces.

#include "Globals.h"
#include "PiecePool.h"
#include "VerticalStrategy.h"
#include "VerticalLimit.h"





////////////////////////////////////////////////////////////////////////////////
// cPiece:

bool cPiece::SetVerticalStrategyFromString(const AString & a_StrategyDesc, bool a_LogWarnings)
{
	auto strategy = CreateVerticalStrategyFromString(a_StrategyDesc, a_LogWarnings);
	if (strategy == nullptr)
	{
		return false;
	}
	m_VerticalStrategy = strategy;
	return true;
}





bool cPiece::SetVerticalLimitFromString(const AString & a_LimitDesc, bool a_LogWarnings)
{
	auto limit = CreateVerticalLimitFromString(a_LimitDesc, a_LogWarnings);
	if (limit == nullptr)
	{
		return false;
	}
	m_VerticalLimit = limit;
	return true;
}





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
			res.m_Direction = cConnector::RotateDirectionCCW(res.m_Direction);
			break;
		}
		case 2:
		{
			// 2 rotations ( = axis flip):
			res.m_Direction = cConnector::RotateDirection(res.m_Direction);
			break;
		}
		case 3:
		{
			// 1 CW rotation:
			res.m_Direction = cConnector::RotateDirectionCW(res.m_Direction);
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

cPiece::cConnector::cConnector(int a_X, int a_Y, int a_Z, int a_Type, eDirection a_Direction) :
	m_Pos(a_X, a_Y, a_Z),
	m_Type(a_Type),
	m_Direction(a_Direction)
{
}





cPiece::cConnector::cConnector(const Vector3i & a_Pos, int a_Type, eDirection a_Direction) :
	m_Pos(a_Pos),
	m_Type(a_Type),
	m_Direction(a_Direction)
{
}





Vector3i cPiece::cConnector::AddDirection(const Vector3i & a_Pos, eDirection a_Direction)
{
	switch (a_Direction)
	{
		case dirXM:       return Vector3i(a_Pos.x - 1, a_Pos.y,     a_Pos.z);
		case dirXP:       return Vector3i(a_Pos.x + 1, a_Pos.y,     a_Pos.z);
		case dirYM:       return Vector3i(a_Pos.x,     a_Pos.y - 1, a_Pos.z);
		case dirYP:       return Vector3i(a_Pos.x,     a_Pos.y + 1, a_Pos.z);
		case dirZM:       return Vector3i(a_Pos.x,     a_Pos.y,     a_Pos.z - 1);
		case dirZP:       return Vector3i(a_Pos.x,     a_Pos.y,     a_Pos.z + 1);
		case dirYM_XM_ZM: return Vector3i(a_Pos.x,     a_Pos.y - 1, a_Pos.z);
		case dirYM_XM_ZP: return Vector3i(a_Pos.x,     a_Pos.y - 1, a_Pos.z);
		case dirYM_XP_ZM: return Vector3i(a_Pos.x,     a_Pos.y - 1, a_Pos.z);
		case dirYM_XP_ZP: return Vector3i(a_Pos.x,     a_Pos.y - 1, a_Pos.z);
		case dirYP_XM_ZM: return Vector3i(a_Pos.x,     a_Pos.y + 1, a_Pos.z);
		case dirYP_XM_ZP: return Vector3i(a_Pos.x,     a_Pos.y + 1, a_Pos.z);
		case dirYP_XP_ZM: return Vector3i(a_Pos.x,     a_Pos.y + 1, a_Pos.z);
		case dirYP_XP_ZP: return Vector3i(a_Pos.x,     a_Pos.y + 1, a_Pos.z);
	}
	UNREACHABLE("Unsupported connector direction");
}





const char * cPiece::cConnector::DirectionToString(eDirection a_Direction)
{
	switch (a_Direction)
	{
		case dirXM:       return "x-";
		case dirXP:       return "x+";
		case dirYM:       return "y-";
		case dirYP:       return "y+";
		case dirZM:       return "z-";
		case dirZP:       return "z+";
		case dirYM_XM_ZM: return "y-x-z-";
		case dirYM_XM_ZP: return "y-x-z+";
		case dirYM_XP_ZM: return "y-x+z-";
		case dirYM_XP_ZP: return "y-x+z+";
		case dirYP_XM_ZM: return "y+x-z-";
		case dirYP_XM_ZP: return "y+x-z+";
		case dirYP_XP_ZM: return "y+x+z-";
		case dirYP_XP_ZP: return "y+x+z+";
	}
	UNREACHABLE("Unsupported connector direction");
}





bool cPiece::cConnector::IsValidDirection(int a_Direction)
{
	switch (a_Direction)
	{
		case dirXM:
		case dirXP:
		case dirYM:
		case dirYP:
		case dirZM:
		case dirZP:
		case dirYM_XM_ZM:
		case dirYM_XM_ZP:
		case dirYM_XP_ZM:
		case dirYM_XP_ZP:
		case dirYP_XM_ZM:
		case dirYP_XM_ZP:
		case dirYP_XP_ZM:
		case dirYP_XP_ZP:
		{
			return true;
		}
	}
	return false;
}





cPiece::cConnector::eDirection cPiece::cConnector::RotateDirection(eDirection a_Direction)
{
	// 180-degree rotation:
	switch (a_Direction)
	{
		case dirXM:       return dirXP;
		case dirXP:       return dirXM;
		case dirYM:       return dirYM;
		case dirYP:       return dirYP;
		case dirZM:       return dirZM;
		case dirZP:       return dirZP;
		case dirYM_XM_ZM: return dirYM_XP_ZP;
		case dirYM_XM_ZP: return dirYM_XP_ZM;
		case dirYM_XP_ZM: return dirYM_XM_ZP;
		case dirYM_XP_ZP: return dirYM_XM_ZM;
		case dirYP_XM_ZM: return dirYP_XP_ZP;
		case dirYP_XM_ZP: return dirYP_XP_ZM;
		case dirYP_XP_ZM: return dirYP_XM_ZP;
		case dirYP_XP_ZP: return dirYP_XM_ZM;
	}
	UNREACHABLE("Unsupported connector direction");
}





cPiece::cConnector::eDirection cPiece::cConnector::RotateDirectionCCW(eDirection a_Direction)
{
	// 90 degrees CCW rotation:
	switch (a_Direction)
	{
		case dirXM:       return dirZP;
		case dirXP:       return dirZM;
		case dirYM:       return dirYM;
		case dirYP:       return dirYP;
		case dirZM:       return dirXM;
		case dirZP:       return dirXP;
		case dirYM_XM_ZM: return dirYM_XM_ZP;
		case dirYM_XM_ZP: return dirYM_XP_ZP;
		case dirYM_XP_ZM: return dirYM_XM_ZM;
		case dirYM_XP_ZP: return dirYM_XP_ZM;
		case dirYP_XM_ZM: return dirYP_XM_ZP;
		case dirYP_XM_ZP: return dirYP_XP_ZP;
		case dirYP_XP_ZM: return dirYP_XM_ZM;
		case dirYP_XP_ZP: return dirYP_XP_ZM;
	}
	UNREACHABLE("Unsupported connector direction");
}





cPiece::cConnector::eDirection cPiece::cConnector::RotateDirectionCW(eDirection a_Direction)
{
	// 90 degrees CW rotation:
	switch (a_Direction)
	{
		case dirXM:       return dirZM;
		case dirXP:       return dirZP;
		case dirYM:       return dirYM;
		case dirYP:       return dirYP;
		case dirZM:       return dirXP;
		case dirZP:       return dirXM;
		case dirYM_XM_ZM: return dirYM_XP_ZM;
		case dirYM_XM_ZP: return dirYM_XM_ZM;
		case dirYM_XP_ZM: return dirYM_XP_ZP;
		case dirYM_XP_ZP: return dirYM_XM_ZP;
		case dirYP_XM_ZM: return dirYP_XP_ZM;
		case dirYP_XM_ZP: return dirYP_XM_ZM;
		case dirYP_XP_ZM: return dirYP_XP_ZP;
		case dirYP_XP_ZP: return dirYP_XM_ZP;
	}
	UNREACHABLE("Unsupported connector direction");
}





int cPiece::cConnector::GetNumCCWRotationsToFit(eDirection a_FixedDir, eDirection a_RotatingDir)
{
	// Translation of direction - direction -> number of CCW rotations needed:
	// You need DirectionRotationTable[fixed][rot] CCW turns to connect rot to fixed (they are opposite)
	// -1 if not possible
	static const int DirectionRotationTable[14][14] =
	{
		/*              YM, YP, ZM, ZP, XM, XP, YM-XM-ZM, YM-XM-ZP, YM-XP-ZM, YM-XP-ZP, YP-XM-ZM, YP-XM-ZP, YP-XP-ZM, YP-XP-ZP */
		/* YM */       { 0, -1, -1, -1, -1, -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1},
		/* YP */       {-1, -1, -1, -1, -1, -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1},
		/* ZM */       {-1, -1,  2,  0,  1,  3,       -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1},
		/* ZP */       {-1, -1,  0,  2,  3,  1,       -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1},
		/* XM */       {-1, -1,  3,  1,  2,  0,       -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1},
		/* XP */       {-1, -1,  1,  3,  0,  2,       -1,       -1,       -1,       -1,       -1,       -1,       -1,       -1},
		/* YM-XM-ZM */ {-1, -1, -1, -1, -1, -1,       -1,       -1,       -1,       -1,        0,        3,        1,        2},
		/* YM-XM-ZP */ {-1, -1, -1, -1, -1, -1,       -1,       -1,       -1,       -1,        1,        0,        2,        3},
		/* YM-XP-ZM */ {-1, -1, -1, -1, -1, -1,       -1,       -1,       -1,       -1,        3,        2,        0,        1},
		/* YM-XP-ZP */ {-1, -1, -1, -1, -1, -1,       -1,       -1,       -1,       -1,        2,        1,        3,        0},
		/* YP-XM-ZM */ {-1, -1, -1, -1, -1, -1,        0,        3,        1,        2,       -1,       -1,       -1,       -1},
		/* YP-XM-ZP */ {-1, -1, -1, -1, -1, -1,        1,        0,        2,        3,       -1,       -1,       -1,       -1},
		/* YP-XP-ZM */ {-1, -1, -1, -1, -1, -1,        3,        2,        0,        1,       -1,       -1,       -1,       -1},
		/* YP-XP-ZP */ {-1, -1, -1, -1, -1, -1,        2,        1,        3,        0,       -1,       -1,       -1,       -1},
	};

	return DirectionRotationTable[a_FixedDir][a_RotatingDir];
}





bool cPiece::cConnector::StringToDirection(const AString & a_Value, eDirection & a_Out)
{
	// First try converting as a number:
	int dirInt;
	if (StringToInteger(a_Value, dirInt))
	{
		if (!IsValidDirection(dirInt))
		{
			return false;
		}
		a_Out = static_cast<eDirection>(dirInt);
		return true;
	}

	// Compare to string representation:
	static const struct
	{
		const char * m_String;
		eDirection m_Value;
	} StringDirections[] =
	{
		{"x-", dirXM},
		{"x+", dirXP},
		{"y-", dirYM},
		{"y+", dirYP},
		{"z-", dirZM},
		{"z+", dirZP},
		{"y-x-z-", dirYM_XM_ZM},
		{"y-x-z+", dirYM_XM_ZP},
		{"y-x+z-", dirYM_XP_ZM},
		{"y-x+z+", dirYM_XP_ZP},
		{"y+x-z-", dirYP_XM_ZM},
		{"y+x-z+", dirYP_XM_ZP},
		{"y+x+z-", dirYP_XP_ZM},
		{"y+x+z+", dirYP_XP_ZP},

		// Alternate names, with slashes:
		{"y-/x-/z-", dirYM_XM_ZM},
		{"y-/x-/z+", dirYM_XM_ZP},
		{"y-/x+/z-", dirYM_XP_ZM},
		{"y-/x+/z+", dirYM_XP_ZP},
		{"y+/x-/z-", dirYP_XM_ZM},
		{"y+/x-/z+", dirYP_XM_ZP},
		{"y+/x+/z-", dirYP_XP_ZM},
		{"y+/x+/z+", dirYP_XP_ZP},
	};
	auto lcValue = StrToLower(a_Value);
	for (size_t i = 0; i < ARRAYCOUNT(StringDirections); i++)
	{
		if (strcmp(lcValue.c_str(), StringDirections[i].m_String) == 0)
		{
			a_Out = StringDirections[i].m_Value;
			return true;
		}
	}

	// Not understood, failure:
	return false;
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
	m_Depth = (m_Parent == nullptr) ? 0 : (m_Parent->GetDepth() + 1);
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




