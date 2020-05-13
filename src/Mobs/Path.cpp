
#include "Globals.h"

#include "Path.h"
#include "../BlockInfo.h"
#include "../Chunk.h"

#define JUMP_G_COST 20
#define NORMAL_G_COST 10
#define DIAGONAL_G_COST 14

#define DISTANCE_MANHATTAN 0  // 1: More speed, a bit less accuracy 0: Max accuracy, less speed.
#define HEURISTICS_ONLY 0  // 1: Much more speed, much less accurate.
#define CALCULATIONS_PER_STEP 10  // Higher means more CPU load but faster path calculations.
// The only version which guarantees the shortest path is 0, 0.





bool compareHeuristics::operator()(cPathCell * & a_Cell1, cPathCell * & a_Cell2)
{
	return a_Cell1->m_F > a_Cell2->m_F;
}





/* cPath implementation */
cPath::cPath(
	cChunk & a_Chunk,
	const Vector3d & a_StartingPoint, const Vector3d & a_EndingPoint, int a_MaxSteps,
	double a_BoundingBoxWidth, double a_BoundingBoxHeight
) :
	m_StepsLeft(a_MaxSteps),
	m_IsValid(true),
	m_CurrentPoint(0),  // GetNextPoint increments this to 1, but that's fine, since the first cell is always a_StartingPoint
	m_Chunk(&a_Chunk),
	m_BadChunkFound(false)
{

	a_BoundingBoxWidth = 1;  // Treat all mobs width as 1 until physics is improved.

	m_BoundingBoxWidth = CeilC(a_BoundingBoxWidth);
	m_BoundingBoxHeight = CeilC(a_BoundingBoxHeight);
	m_HalfWidth = a_BoundingBoxWidth / 2;

	int HalfWidthInt = FloorC(a_BoundingBoxWidth / 2);
	m_Source.x = FloorC(a_StartingPoint.x - HalfWidthInt);
	m_Source.y = FloorC(a_StartingPoint.y);
	m_Source.z = FloorC(a_StartingPoint.z - HalfWidthInt);

	m_Destination.x = FloorC(a_EndingPoint.x - HalfWidthInt);
	m_Destination.y = FloorC(a_EndingPoint.y);
	m_Destination.z = FloorC(a_EndingPoint.z - HalfWidthInt);

	if (!IsWalkable(m_Source, m_Source))
	{
		m_Status = ePathFinderStatus::PATH_NOT_FOUND;
		return;
	}

	m_NearestPointToTarget = GetCell(m_Source);
	m_Status = ePathFinderStatus::CALCULATING;

	ProcessCell(GetCell(m_Source), nullptr, 0);
}





cPath::cPath() : m_IsValid(false)
{

}





ePathFinderStatus cPath::CalculationStep(cChunk & a_Chunk)
{
	m_Chunk = &a_Chunk;
	if (m_Status != ePathFinderStatus::CALCULATING)
	{
		return m_Status;
	}

	if (m_BadChunkFound)
	{
		FinishCalculation(ePathFinderStatus::PATH_NOT_FOUND);
		return m_Status;
	}

	if (m_StepsLeft == 0)
	{
		AttemptToFindAlternative();
	}
	else
	{
		--m_StepsLeft;
		int i;
		for (i = 0; i < CALCULATIONS_PER_STEP; ++i)
		{
			if (StepOnce())  // StepOnce returns true when no more calculation is needed.
			{
				break;  // if we're here, m_Status must have changed either to PATH_FOUND or PATH_NOT_FOUND.
			}
		}

		m_Chunk = nullptr;
	}
	return m_Status;
}





Vector3i cPath::AcceptNearbyPath()
{
	ASSERT(m_Status == ePathFinderStatus::NEARBY_FOUND);
	m_Status = ePathFinderStatus::PATH_FOUND;
	return m_Destination;
}





bool cPath::StepOnce()
{
	cPathCell * CurrentCell = OpenListPop();

	// Path not reachable.
	if (CurrentCell == nullptr)
	{
		AttemptToFindAlternative();
		return true;
	}

	// Path found.
	if (CurrentCell->m_Location == m_Destination)
	{
		BuildPath();
		FinishCalculation(ePathFinderStatus::PATH_FOUND);
		return true;
	}

	// Calculation not finished yet
	// Check if we have a new NearestPoint.
	if ((m_Destination - CurrentCell->m_Location).Length() < 5)
	{
		if (GetRandomProvider().RandBool(0.25))
		{
			m_NearestPointToTarget = CurrentCell;
		}
	}
	else if (CurrentCell->m_H < m_NearestPointToTarget->m_H)
	{
		m_NearestPointToTarget = CurrentCell;
	}
	// process a currentCell by inspecting all neighbors.


	// Now we start checking adjacent cells.


	// If true, no need to do more checks in that direction
	bool DoneEast = false,
	DoneWest = false,
	DoneNorth = false,
	DoneSouth = false;

	// If true, we can walk in that direction without changing height
	// This is used for deciding if to calculate diagonals
	bool WalkableEast = false,
	WalkableWest = false,
	WalkableNorth = false,
	WalkableSouth = false;

	// If we can jump without hitting the ceiling
	if (BodyFitsIn(CurrentCell->m_Location + Vector3i(0, 1, 0), CurrentCell->m_Location))
	{
		// For ladder climbing
		ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, 1, 0), CurrentCell, JUMP_G_COST);

		// Check east-up
		if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(1, 1, 0), CurrentCell, JUMP_G_COST))
		{
			DoneEast = true;
		}

		// Check west-up
		if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(-1, 1, 0), CurrentCell, JUMP_G_COST))
		{
			DoneWest = true;
		}

		// Check north-up
		if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, 1, -1), CurrentCell, JUMP_G_COST))
		{
			DoneNorth = true;
		}

		// Check south-up
		if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, 1, 1), CurrentCell, JUMP_G_COST))
		{
			DoneSouth = true;
		}

	}


	// Check North, South, East, West at our own height or below. We are willing to jump up to 3 blocks down.


	if (!DoneEast)
	{
		for (int y = 0; y >= -3; --y)
		{
			if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(1, y, 0),  CurrentCell, NORMAL_G_COST))
			{
				DoneEast = true;
				if (y == 0)
				{
					WalkableEast = true;
				}
				break;
			}
		}
	}

	if (!DoneWest)
	{
		for (int y = 0; y >= -3; --y)
		{
			if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(-1, y, 0),  CurrentCell, NORMAL_G_COST))
			{
				DoneWest = true;
				if (y == 0)
				{
					WalkableWest = true;
				}
				break;
			}
		}
	}

	if (!DoneSouth)
	{
		for (int y = 0; y >= -3; --y)
		{
			if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, y, 1),  CurrentCell, NORMAL_G_COST))
			{
				DoneSouth = true;
				if (y == 0)
				{
					WalkableSouth = true;
				}
				break;
			}
		}
	}

	if (!DoneNorth)
	{
		for (int y = 0; y >= -3; --y)
		{
			if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, y, -1), CurrentCell, NORMAL_G_COST))
			{
				DoneNorth = true;
				if (y == 0)
				{
					WalkableNorth = true;
				}
				break;
			}
		}
	}

	// Check diagonals

	if (WalkableNorth && WalkableEast)
	{
		ProcessIfWalkable(CurrentCell->m_Location + Vector3i(1, 0, -1), CurrentCell, DIAGONAL_G_COST);
	}
	if (WalkableNorth && WalkableWest)
	{
		ProcessIfWalkable(CurrentCell->m_Location + Vector3i(-1, 0, -1), CurrentCell, DIAGONAL_G_COST);
	}
	if (WalkableSouth && WalkableEast)
	{
		ProcessIfWalkable(CurrentCell->m_Location + Vector3i(1, 0, 1), CurrentCell, DIAGONAL_G_COST);
	}
	if (WalkableSouth && WalkableWest)
	{
		ProcessIfWalkable(CurrentCell->m_Location + Vector3i(-1, 0, 1), CurrentCell, DIAGONAL_G_COST);
	}

	return false;
}





void cPath::AttemptToFindAlternative()
{
	if (m_NearestPointToTarget == GetCell(m_Source))
	{
		FinishCalculation(ePathFinderStatus::PATH_NOT_FOUND);
	}
	else
	{
		m_Destination = m_NearestPointToTarget->m_Location;
		BuildPath();
		FinishCalculation(ePathFinderStatus::NEARBY_FOUND);
	}
}





void cPath::BuildPath()
{
	cPathCell * CurrentCell = GetCell(m_Destination);
	while (CurrentCell->m_Parent != nullptr)
	{
		// Waypoints are cylinders that start at some particular x, y, z and have infinite height.
		// Submerging water waypoints allows swimming mobs to be able to touch them.
		if (IsBlockWater(GetCell(CurrentCell->m_Location + Vector3i(0, -1, 0))->m_BlockType))
		{
			CurrentCell->m_Location.y -= 30;
		}
		m_PathPoints.push_back(CurrentCell->m_Location);  // Populate the cPath with points. All midpoints are added. Destination is added. Source is excluded.
		CurrentCell = CurrentCell->m_Parent;
	}

}





void cPath::FinishCalculation()
{
	m_Map.clear();
	m_OpenList = std::priority_queue<cPathCell *, std::vector<cPathCell *>, compareHeuristics>{};
}





void cPath::FinishCalculation(ePathFinderStatus a_NewStatus)
{
	if (m_BadChunkFound)
	{
		a_NewStatus = ePathFinderStatus::PATH_NOT_FOUND;
	}
	m_Status = a_NewStatus;
	FinishCalculation();
}





void cPath::OpenListAdd(cPathCell * a_Cell)
{
	a_Cell->m_Status = eCellStatus::OPENLIST;
	m_OpenList.push(a_Cell);
	#ifdef COMPILING_PATHFIND_DEBUGGER
	si::setBlock(a_Cell->m_Location.x, a_Cell->m_Location.y, a_Cell->m_Location.z, debug_open, SetMini(a_Cell));
	#endif
}





cPathCell * cPath::OpenListPop()  // Popping from the open list also means adding to the closed list.
{
	if (m_OpenList.size() == 0)
	{
		return nullptr;  // We've exhausted the search space and nothing was found, this will trigger a PATH_NOT_FOUND or NEARBY_FOUND status.
	}

	cPathCell * Ret = m_OpenList.top();
	m_OpenList.pop();
	Ret->m_Status = eCellStatus::CLOSEDLIST;
	#ifdef COMPILING_PATHFIND_DEBUGGER
	si::setBlock((Ret)->m_Location.x, (Ret)->m_Location.y, (Ret)->m_Location.z, debug_closed, SetMini(Ret));
	#endif
	return Ret;
}





bool cPath::ProcessIfWalkable(const Vector3i & a_Location, cPathCell * a_Parent, int a_Cost)
{
	if (IsWalkable(a_Location, a_Parent->m_Location))
	{
		ProcessCell(GetCell(a_Location), a_Parent, a_Cost);
		return true;
	}
	return false;
}





void cPath::ProcessCell(cPathCell * a_Cell, cPathCell * a_Caller, int a_GDelta)
{
	// Case 1: Cell is in the closed list, ignore it.
	if (a_Cell->m_Status == eCellStatus::CLOSEDLIST)
	{
		return;
	}
	if (a_Cell->m_Status == eCellStatus::NOLIST)  // Case 2: The cell is not in any list.
	{
		// Cell is walkable, add it to the open list.
		// Note that non-walkable cells are filtered out in Step_internal();
		// Special case: Start cell goes here, gDelta is 0, caller is NULL.
		a_Cell->m_Parent = a_Caller;
		if (a_Caller != nullptr)
		{
			a_Cell->m_G = a_Caller->m_G + a_GDelta;
		}
		else
		{
			a_Cell->m_G = 0;
		}

		// Calculate H. This is A*'s Heuristics value.
		#if DISTANCE_MANHATTAN == 1
			// Manhattan distance. DeltaX + DeltaY + DeltaZ.
			a_Cell->m_H = 10 * (abs(a_Cell->m_Location.x-m_Destination.x) + abs(a_Cell->m_Location.y-m_Destination.y) + abs(a_Cell->m_Location.z-m_Destination.z));
		#else
			// Euclidian distance. sqrt(DeltaX^2 + DeltaY^2 + DeltaZ^2), more precise.
			a_Cell->m_H = static_cast<decltype(a_Cell->m_H)>((a_Cell->m_Location - m_Destination).Length() * 10);
		#endif

		#if HEURISTICS_ONLY == 1
			a_Cell->m_F = a_Cell->m_H;  // Greedy search. https://en.wikipedia.org/wiki/Greedy_search
		#else
			a_Cell->m_F = a_Cell->m_H + a_Cell->m_G;  // Regular A*.
		#endif

		OpenListAdd(a_Cell);
		return;
	}

	// Case 3: Cell is in the open list, check if G and H need an update.
	int NewG = a_Caller->m_G + a_GDelta;
	if (NewG < a_Cell->m_G)
	{
		a_Cell->m_G = NewG;
		a_Cell->m_H = a_Cell->m_F + a_Cell->m_G;
		a_Cell->m_Parent = a_Caller;
	}

}





void cPath::FillCellAttributes(cPathCell & a_Cell)
{
	const Vector3i & Location = a_Cell.m_Location;

	ASSERT(m_Chunk != nullptr);

	if (!cChunkDef::IsValidHeight(Location.y))
	{
		// Players can't build outside the game height, so it must be air
		a_Cell.m_IsSolid = false;
		a_Cell.m_IsSpecial = false;
		a_Cell.m_BlockType = E_BLOCK_AIR;
		return;
	}
	auto Chunk = m_Chunk->GetNeighborChunk(Location.x, Location.z);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		m_BadChunkFound = true;
		a_Cell.m_IsSolid = true;
		a_Cell.m_IsSpecial = false;
		a_Cell.m_BlockType = E_BLOCK_AIR;  // m_BlockType is never used when m_IsSpecial is false, but it may be used if we implement dijkstra
		return;
	}
	m_Chunk = Chunk;

	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	int RelX = Location.x - m_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = Location.z - m_Chunk->GetPosZ() * cChunkDef::Width;

	m_Chunk->GetBlockTypeMeta(RelX, Location.y, RelZ, BlockType, BlockMeta);
	a_Cell.m_BlockType = BlockType;
	a_Cell.m_BlockMeta = BlockMeta;


	if (BlockTypeIsSpecial(BlockType))
	{
		a_Cell.m_IsSpecial = true;
		a_Cell.m_IsSolid = true;  // Specials are solids only from a certain direction. But their m_IsSolid is always true
	}
	else if ((!cBlockInfo::IsSolid(a_Cell.m_BlockType)) && IsBlockFence(GetCell(Location + Vector3i(0, -1, 0))->m_BlockType))
	{
		// Nonsolid blocks with fences below them are consider Special Solids. That is, they sometimes behave as solids.
		a_Cell.m_IsSpecial = true;
		a_Cell.m_IsSolid = true;
	}
	else
	{

		a_Cell.m_IsSpecial = false;
		a_Cell.m_IsSolid = cBlockInfo::IsSolid(BlockType);
	}

}





cPathCell * cPath::GetCell(const Vector3i & a_Location)
{
	// Create the cell in the hash table if it's not already there.
	if (m_Map.count(a_Location) == 0)  // Case 1: Cell is not on any list. We've never checked this cell before.
	{
		m_Map[a_Location].m_Location = a_Location;
		FillCellAttributes(m_Map[a_Location]);
		m_Map[a_Location].m_Status = eCellStatus::NOLIST;
		#ifdef COMPILING_PATHFIND_DEBUGGER
			#ifdef COMPILING_PATHFIND_DEBUGGER_MARK_UNCHECKED
				si::setBlock(a_Location.x, a_Location.y, a_Location.z, debug_unchecked, Cell->m_IsSolid ? NORMAL : MINI);
			#endif
		#endif
		return &m_Map[a_Location];
	}
	else
	{
		return &m_Map[a_Location];
	}
}





bool cPath::IsWalkable(const Vector3i & a_Location, const Vector3i & a_Source)
{
	return (HasSolidBelow(a_Location) && BodyFitsIn(a_Location, a_Source));
}




// We need the source because some special blocks are solid only from a certain direction e.g. doors
bool cPath::BodyFitsIn(const Vector3i & a_Location, const Vector3i & a_Source)
{
	int x, y, z;
	for (y = 0; y < m_BoundingBoxHeight; ++y)
	{
		for (x = 0; x < m_BoundingBoxWidth; ++x)
		{
			for (z = 0; z < m_BoundingBoxWidth; ++z)
			{
				cPathCell * CurrentCell = GetCell(a_Location + Vector3i(x, y, z));
				if (CurrentCell->m_IsSolid)
				{
					if (CurrentCell->m_IsSpecial)
					{
						if (SpecialIsSolidFromThisDirection(CurrentCell->m_BlockType, CurrentCell->m_BlockMeta, a_Location - a_Source))
						{
							return false;
						}
					}
					else
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}





bool cPath::BlockTypeIsSpecial(BLOCKTYPE a_Type)
{
	if (IsBlockFence(a_Type))
	{
		return true;
	}

	switch (a_Type)
	{
		case E_BLOCK_OAK_DOOR:
		case E_BLOCK_DARK_OAK_DOOR:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_WATER:
		case E_BLOCK_STATIONARY_WATER:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool cPath::SpecialIsSolidFromThisDirection(BLOCKTYPE a_Type, NIBBLETYPE a_Meta,  const Vector3i & a_Direction)
{
	if (a_Direction == Vector3i(0, 0, 0))
	{
		return false;
	}


	// If there is a nonsolid above a fence
	if (!cBlockInfo::IsSolid(a_Type))
	{
		// Only treat as solid when we're coming from below
		return (a_Direction.y > 0);
	}

	/* switch (a_Type)
	{
		case E_BLOCK_ETC:
		{
			Decide if solid from this direction and return either true or false.
		}

		// TODO Fill this with the other specials after physics is fixed
	} */



	return true;
}





bool cPath::HasSolidBelow(const Vector3i & a_Location)
{
	int x, z;
	for (x = 0; x < m_BoundingBoxWidth; ++x)
	{
		for (z = 0; z < m_BoundingBoxWidth; ++z)
		{
			if (GetCell(a_Location + Vector3i(x, -1, z))->m_IsSolid)
			{
				return true;
			}
		}
	}
	return false;
}
