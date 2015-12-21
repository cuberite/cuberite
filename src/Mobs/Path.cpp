
#include "Globals.h"

#include <cmath>

#include "Path.h"
#include "../Chunk.h"

#define JUMP_G_COST 20

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
	double a_BoundingBoxWidth, double a_BoundingBoxHeight,
	int a_MaxUp, int a_MaxDown
) :
	m_StepsLeft(a_MaxSteps),
	m_IsValid(true),
	m_CurrentPoint(0),  // GetNextPoint increments this to 1, but that's fine, since the first cell is always a_StartingPoint
	m_Chunk(&a_Chunk),
	m_BadChunkFound(false)
{
	// TODO: if src not walkable OR dest not walkable, then abort.
	// Borrow a new "isWalkable" from ProcessIfWalkable, make ProcessIfWalkable also call isWalkable

	a_BoundingBoxWidth = 1;  // Treat all mobs width as 1 until physics is improved. This would also require changes to stepOnce to work.

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

	if (!IsWalkable(m_Source))
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





bool cPath::IsSolid(const Vector3i & a_Location)
{
	ASSERT(m_Chunk != nullptr);

	if (!cChunkDef::IsValidHeight(a_Location.y))
	{
		return false;
	}
	auto Chunk = m_Chunk->GetNeighborChunk(a_Location.x, a_Location.z);
	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		m_BadChunkFound = true;
		return true;
	}
	m_Chunk = Chunk;

	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	int RelX = a_Location.x - m_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_Location.z - m_Chunk->GetPosZ() * cChunkDef::Width;

	m_Chunk->GetBlockTypeMeta(RelX, a_Location.y, RelZ, BlockType, BlockMeta);
	if (
			(BlockType == E_BLOCK_FENCE) ||
			(BlockType == E_BLOCK_OAK_FENCE_GATE) ||
			(BlockType == E_BLOCK_NETHER_BRICK_FENCE) ||
			(BlockType == E_BLOCK_COBBLESTONE_WALL) ||
			((BlockType >= E_BLOCK_SPRUCE_FENCE_GATE) && (BlockType <= E_BLOCK_ACACIA_FENCE))
	)
	{
		// TODO move this out of IsSolid to a proper place.
		GetCell(a_Location + Vector3i(0, 1, 0))->m_IsSolid = true;  // Mobs will always think that the fence is 2 blocks high and therefore won't jump over.
	}
	if (BlockType == E_BLOCK_STATIONARY_WATER)
	{
		GetCell(a_Location + Vector3i(0, -1, 0))->m_IsSolid = true;
	}

	return cBlockInfo::IsSolid(BlockType);
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
		if (m_Rand.NextInt(4) == 0)
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


	bool done_east = false,
	done_west = false,
	done_north = false,
	done_south = false;  // If true, no need to do more checks in that direction

	// If we can jump without hitting the ceiling
	if (BodyFitsIn(CurrentCell->m_Location + Vector3i(0, 1, 0)))
	{
		// Check east-up
		if (GetCell(CurrentCell->m_Location + Vector3i(1, 0, 0))->m_IsSolid)
		{
			ProcessIfWalkable(CurrentCell->m_Location + Vector3i(1, 1, 0), CurrentCell, JUMP_G_COST);
			done_east = true;
		}

		// Check west-up
		if (GetCell(CurrentCell->m_Location + Vector3i(-1, 0, 0))->m_IsSolid)
		{
			ProcessIfWalkable(CurrentCell->m_Location + Vector3i(-1, 1, 0), CurrentCell, JUMP_G_COST);
			done_west = true;
		}

		// Check north-up
		if (GetCell(CurrentCell->m_Location + Vector3i(0, 0, -1))->m_IsSolid)
		{
			ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, 1, -1), CurrentCell, JUMP_G_COST);
			done_north = true;
		}

		// Check south-up
		if (GetCell(CurrentCell->m_Location + Vector3i(0, 0, 1))->m_IsSolid)
		{
			ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, 1, 1), CurrentCell, JUMP_G_COST);
			done_south = true;
		}

	}


	// Check North, South, East, West at our own height or below. We are willing to jump up to 3 blocks down.


	if (!done_east)
	{
		for (int i = 0; i >= -3; --i)
		{
			if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(1, i, 0),  CurrentCell, 10))
			{
				done_east = true;
				break;
			}
		}
	}

	if (!done_west)
	{
		for (int i = 0; i >= -3; --i)
		{
			if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(-1, i, 0),  CurrentCell, 10))
			{
				done_west = true;
				break;
			}
		}
	}

	if (!done_south)
	{
		for (int i = 0; i >= -3; --i)
		{
			if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, i, 1),  CurrentCell, 10))
			{
				done_west = true;
				break;
			}
		}
	}

	if (!done_north)
	{
		for (int i = 0; i >= -3; --i)
		{
			if (ProcessIfWalkable(CurrentCell->m_Location + Vector3i(0, i, -1),  CurrentCell, 10))
			{
				done_north = true;
				break;
			}
		}
	}


	// Check diagonals


	for (int x = -1; x <= 1; x += 2)
	{
		for (int z = -1; z <= 1; z += 2)
		{
			// This condition prevents diagonal corner cutting.
			if (!GetCell(CurrentCell->m_Location + Vector3i(x, 0, 0))->m_IsSolid && !GetCell(CurrentCell->m_Location + Vector3i(0, 0, z))->m_IsSolid)
			{
				// This prevents falling of "sharp turns" e.g. a 1x1x20 rectangle in the air which breaks in a right angle suddenly.
				if (GetCell(CurrentCell->m_Location + Vector3i(x, -1, 0))->m_IsSolid && GetCell(CurrentCell->m_Location + Vector3i(0, -1, z))->m_IsSolid)
				{
					ProcessIfWalkable(CurrentCell->m_Location + Vector3i(x, 0, z), CurrentCell, 14);  // 14 is a good enough approximation of sqrt(10 + 10).
				}
			}
		}
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
	if (IsWalkable(a_Location))
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





cPathCell * cPath::GetCell(const Vector3i & a_Location)
{
	// Create the cell in the hash table if it's not already there.
	if (m_Map.count(a_Location) == 0)  // Case 1: Cell is not on any list. We've never checked this cell before.
	{
		m_Map[a_Location].m_Location = a_Location;
		m_Map[a_Location].m_IsSolid = IsSolid(a_Location);
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





bool cPath::IsWalkable(const Vector3i & a_Location)
{
	return (HasSolidBelow(a_Location) && BodyFitsIn(a_Location));
}





bool cPath::BodyFitsIn(const Vector3i & a_Location)
{
	int x, y, z;
	for (y = 0; y < m_BoundingBoxHeight; ++y)
	{
		for (x = 0; x < m_BoundingBoxWidth; ++x)
		{
			for (z = 0; z < m_BoundingBoxWidth; ++z)
			{
				if (GetCell(a_Location + Vector3i(x, y, z))->m_IsSolid)
				{
					return false;
				}
			}
		}
	}
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
