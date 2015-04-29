#include "Globals.h"
#ifndef COMPILING_PATHFIND_DEBUGGER
	/* MCServer headers */
	#include "../World.h"
	#include "../Chunk.h"
#endif

#include <cmath>
#include "Path.h"

#define DISTANCE_MANHATTAN 0  // 1: More speed, a bit less accuracy 0: Max accuracy, less speed.
#define HEURISTICS_ONLY 0  // 1: Much more speed, much less accurate.
#define CALCULATIONS_PER_STEP 5  // Higher means more CPU load but faster path calculations.
// The only version which guarantees the shortest path is 0, 0.

enum class eCellStatus {OPENLIST,  CLOSEDLIST,  NOLIST};
struct cPathCell
{
	Vector3d m_Location;   // Location of the cell in the world.
	int m_F, m_G, m_H;  // F, G, H as defined in regular A*.
	eCellStatus m_Status;  // Which list is the cell in? Either non, open, or closed.
	cPathCell * m_Parent;  // Cell's parent, as defined in regular A*.
	bool m_IsSolid;	   // Is the cell an air or a solid? Partial solids are currently considered solids.
};





bool compareHeuristics::operator()(cPathCell * & a_Cell1, cPathCell * & a_Cell2)
{
	return a_Cell1->m_F > a_Cell2->m_F;
}





/* cPath implementation */
cPath::cPath(
	cWorld * a_World,
	const Vector3d & a_StartingPoint, const Vector3d & a_EndingPoint, int a_MaxSteps,
	double a_BoundingBoxWidth, double a_BoundingBoxHeight,
	int a_MaxUp, int a_MaxDown
)
{
	// TODO: if src not walkable OR dest not walkable, then abort.
	// Borrow a new "isWalkable" from ProcessIfWalkable, make ProcessIfWalkable also call isWalkable

	m_World = a_World;
	// m_World = cRoot::Get()->GetDefaultWorld();

	m_Source = a_StartingPoint.Floor();
	m_Destination = a_EndingPoint.Floor();

	if (GetCell(m_Source)->m_IsSolid || GetCell(m_Destination)->m_IsSolid)
	{
		m_Status = ePathFinderStatus::PATH_NOT_FOUND;
		return;
	}

	m_Status = ePathFinderStatus::CALCULATING;

	m_StepsLeft = a_MaxSteps;
	m_PointCount = 0;

	ProcessCell(GetCell(a_StartingPoint), nullptr, 0);
}





cPath::~cPath()
{
	if (m_Status == ePathFinderStatus::CALCULATING)
	{
		FinishCalculation();
	}
}





ePathFinderStatus cPath::Step()
{
	if (m_Status != ePathFinderStatus::CALCULATING)
	{
		return m_Status;
	}

	if (m_StepsLeft == 0)
	{
		FinishCalculation(ePathFinderStatus::PATH_NOT_FOUND);
	}
	else
	{
		--m_StepsLeft;
		int i;
		for (i = 0; i < CALCULATIONS_PER_STEP; ++i)
		{
			if (Step_Internal())  // Step_Internal returns true when no more calculation is needed.
			{
				break;  // if we're here, m_Status must have changed either to PATH_FOUND or PATH_NOT_FOUND.
			}
		}
	}
	return m_Status;
}





#ifndef COMPILING_PATHFIND_DEBUGGER
bool cPath::IsSolid(const Vector3d & a_Location)
{
	int ChunkX, ChunkZ;
	m_Item_CurrentBlock = a_Location;
	cChunkDef::BlockToChunk(a_Location.x, a_Location.z, ChunkX, ChunkZ);
	return !m_World->DoWithChunk(ChunkX, ChunkZ, * this);
}
#endif





bool cPath::Step_Internal()
{
	cPathCell * CurrentCell = OpenListPop();

	// Path not reachable, open list exauhsted.
	if (CurrentCell == nullptr)
	{
		FinishCalculation(ePathFinderStatus::PATH_NOT_FOUND);
		ASSERT(m_Status == ePathFinderStatus::PATH_NOT_FOUND);
		return true;
	}

	// Path found.
	if (CurrentCell->m_Location == m_Destination)
	{
		do
		{
			AddPoint(CurrentCell->m_Location + Vector3d(0.5, 0, 0.5));  // Populate the cPath with points.
			CurrentCell = CurrentCell->m_Parent;
		} while (CurrentCell != nullptr);

		m_CurrentPoint = -1;
		FinishCalculation(ePathFinderStatus::PATH_FOUND);
		return true;
	}

	// Calculation not finished yet, process a currentCell by inspecting all neighbors.

	// Check North, South, East, West on all 3 different heights.
	int i;
	for (i = -1; i <= 1; ++i)
	{
		ProcessIfWalkable(CurrentCell->m_Location + Vector3d(1, i, 0),  CurrentCell, 10);
		ProcessIfWalkable(CurrentCell->m_Location + Vector3d(-1, i, 0), CurrentCell, 10);
		ProcessIfWalkable(CurrentCell->m_Location + Vector3d(0, i, 1),  CurrentCell, 10);
		ProcessIfWalkable(CurrentCell->m_Location + Vector3d(0, i, -1), CurrentCell, 10);
	}

	// Check diagonals on mob's height only.
	int x, z;
	for (x = -1; x <= 1; x += 2)
	{
		for (z = -1; z <= 1; z += 2)
		{
			// This condition prevents diagonal corner cutting.
			if (!GetCell(CurrentCell->m_Location + Vector3d(x, 0, 0))->m_IsSolid && !GetCell(CurrentCell->m_Location + Vector3d(0, 0, z))->m_IsSolid)
			{
				// This prevents falling of "sharp turns" e.g. a 1x1x20 rectangle in the air which breaks in a right angle suddenly.
				if (GetCell(CurrentCell->m_Location + Vector3d(x, -1, 0))->m_IsSolid && GetCell(CurrentCell->m_Location + Vector3d(0, -1, z))->m_IsSolid)
				{
					ProcessIfWalkable(CurrentCell->m_Location + Vector3d(x, 0, z), CurrentCell, 14);  // 14 is a good enough approximation of sqrt(10 + 10).
				}
			}
		}
	}


	return false;
}





void cPath::FinishCalculation()
{
	for (auto && pair : m_Map)
	{
		delete pair.second;
	}

	m_Map.clear();
	m_OpenList.empty();
}





void cPath::FinishCalculation(ePathFinderStatus a_NewStatus)
{
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
		return nullptr;  // We've exhausted the search space and nothing was found, this will trigger a PATH_NOT_FOUND status.
	}

	cPathCell * Ret = m_OpenList.top();
	m_OpenList.pop();
	Ret->m_Status = eCellStatus::CLOSEDLIST;
	#ifdef COMPILING_PATHFIND_DEBUGGER
si::setBlock((Ret)->m_Location.x, (Ret)->m_Location.y, (Ret)->m_Location.z, debug_closed, SetMini(Ret));
	#endif
	return Ret;
}





void cPath::ProcessIfWalkable(const Vector3d & a_Location, cPathCell * a_Parent, int a_Cost)
{
	cPathCell * cell =  GetCell(a_Location);
	if (!cell->m_IsSolid && GetCell(a_Location + Vector3d(0, -1, 0))->m_IsSolid && !GetCell(a_Location + Vector3d(0, 1, 0))->m_IsSolid)
	{
		ProcessCell(cell, a_Parent, a_Cost);
	}
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
			a_Cell->m_H = std::sqrt((a_Cell->m_Location.x - m_Destination.x) * (a_Cell->m_Location.x - m_Destination.x) * 100 + (a_Cell->m_Location.y - m_Destination.y) * (a_Cell->m_Location.y - m_Destination.y) * 100 + (a_Cell->m_Location.z - m_Destination.z) * (a_Cell->m_Location.z - m_Destination.z) * 100);
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





cPathCell * cPath::GetCell(const Vector3d & a_Location)
{
	// Create the cell in the hash table if it's not already there.
	cPathCell * Cell;
	if (m_Map.count(a_Location) == 0)  // Case 1: Cell is not on any list. We've never checked this cell before.
	{
		Cell = new cPathCell();
		Cell->m_Location = a_Location;
		m_Map[a_Location] = Cell;
		Cell->m_IsSolid = IsSolid(a_Location);
		Cell->m_Status = eCellStatus::NOLIST;
		#ifdef COMPILING_PATHFIND_DEBUGGER
			#ifdef COMPILING_PATHFIND_DEBUGGER_MARK_UNCHECKED
				si::setBlock(a_Location.x, a_Location.y, a_Location.z, debug_unchecked, Cell->m_IsSolid ? NORMAL : MINI);
			#endif
		#endif
		return Cell;
	}
	else
	{
		return m_Map[a_Location];
	}
}





// Add the next point in the final path.
void cPath::AddPoint(Vector3d a_Vector)
{
	m_PathPoints.push_back(a_Vector);
	++m_PointCount;
}





#ifndef COMPILING_PATHFIND_DEBUGGER
bool cPath::Item(cChunk * a_Chunk)  // returns FALSE if there's a solid or if we failed.
{
	int RelX = m_Item_CurrentBlock.x - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = m_Item_CurrentBlock.z - a_Chunk->GetPosZ() * cChunkDef::Width;

	if (!a_Chunk->IsValid())
	{
		return false;
	}
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	a_Chunk->GetBlockTypeMeta(RelX, m_Item_CurrentBlock.y, RelZ, BlockType, BlockMeta);
	return (!cBlockInfo::IsSolid(BlockType));

	// TODO Maybe I should queue several blocks and call item() at once for all of them for better performance?
	// I think Worktycho said each item() call needs 2 locks.

}
#endif
