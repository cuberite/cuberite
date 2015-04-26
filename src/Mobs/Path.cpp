#ifndef COMPILING_PATHFIND_DEBUGGER
/* MCServer headers */
#include "Globals.h"
#include "../World.h"
#endif

#include "Path.h"

#include <stdio.h>
#include <exception>
#include <cmath>

#define DISTANCE_MANHATTEN 0  // 1: More speed, a bit less accuracy 0: Max accuracy, less speed.
#define HEURISTICS_ONLY 0  // 1: Much more speed, much less accurate.
#define CALCULATIONS_PER_CALL 1  // Higher means more CPU load but faster path calculations.
// The only version which guarantees the shortest path is 0, 0.

enum eCellStatus {OPENLIST,  CLOSEDLIST,  NOLIST};
struct cPathCell
{
	Vector3d m_Location;   // Location of the cell in the world
	int m_F, m_G, m_H;     // F, G, H as defined in regular A*.
	eCellStatus m_Status;  // Which list is the cell in? Either non, open, or closed.
	cPathCell * m_Parent;  // Cell's parent, as defined in regular A*.
	bool m_IsSolid;	       // Is the cell an air or a solid? Partial solids are currently considered solids.
};





bool compareHeuristics::operator()(cPathCell * & a_V1, cPathCell * & a_V2)
{
	return a_V1->m_F > a_V2->m_F;
}





/* cPath implementation */
#ifndef COMPILING_PATHFIND_DEBUGGER
// Incomplete
bool cPath::IsSolid(const Vector3d & a_Location)
{
	return true;
	// int ChunkX, ChunkZ;
	// cChunkDef::BlockToChunk(a_Location.x, a_Location.z, ChunkX, ChunkZ);
	// return m_World->DoWithChunk(ChunkX, ChunkZ, *this);
}




// Incomplete
bool cPath::Item(cChunk * a_Chunk)
{
	printf("cPath::item Mindelessly returning true\n");
	// TODO intelligently return the right state here.
	// TODO I probably need to store a_Location.x and a_Location.y as fields, is that correct?
	// TODO Maybe I should later queue several blocks and call this at once for all of them for better performance?
	return true;
}



// Incomplete
// For testing only, will eventually be removed along with the changes made to server.cpp
void cPath::consoleCommand()
{
	printf("HELLO WORLD\n");
	// printf("cPath::isSolid returned %s.\n", cPath().IsSolid(Vector3d(0, 0, 0)) ? "true" : "false");
}
#endif





cPath::cPath(const Vector3d & a_StartingPoint, const Vector3d & a_EndingPoint, int a_MaxSteps, double a_BoundingBoxWidth, double a_BoundingBoxHeight, int a_MaxUp, int a_MaxDown)
{
	if (GetCell(a_StartingPoint)->m_IsSolid || GetCell(a_EndingPoint)->m_IsSolid)
	{
		m_Status = PATH_NOT_FOUND;
		return;
	}
	
	m_Status = CALCULATING;
	m_Source = a_StartingPoint;
	m_Destination = a_EndingPoint;
	m_StepsLeft = a_MaxSteps;
	m_PointCount = 0;
	processIfWalkable(a_StartingPoint, NULL, 0);
}





cPath::~cPath()
{
	// FinishCalculation();  // TODO: Needed? STD is supposed to clean it all.
}





void cPath::FinishCalculation(ePathFinderStatus a_NewStatus)
{
	m_Status = a_NewStatus;
	for (std::unordered_map<Vector3d, cPathCell *>::iterator it = m_Map.begin(); it != m_Map.end(); ++it)
	{
		delete (it->second);
	}
	
	m_Map.clear();
	m_OpenList.empty();
}





cPathCell * cPath::GetCell(const Vector3d & a_Location)
{
	// Create the cell in the hash table if it's not already there.
	cPathCell * cell;
	if (m_Map.count(a_Location) == 0)  // Case 1: Cell is not on any list. We've never checked this cell before.
	{
		cell = new cPathCell();
		cell->m_Location = a_Location;
		m_Map[a_Location] = cell;
		cell->m_IsSolid = cPath::IsSolid(a_Location);
		cell->m_Status = NOLIST;
		#ifdef COMPILING_PATHFIND_DEBUGGER
		#ifdef COMPILING_PATHFIND_DEBUGGER_MARK_UNCHECKED
		si::setBlock(a_Location.x, a_Location.y, a_Location.z, debug_unchecked, !cell->m_IsSolid);
		#endif
		#endif
		return cell;
	}
	else return m_Map[a_Location];
}





void cPath::ProcessCell(cPathCell * a_Cell, cPathCell * a_Caller, int a_GDelta)
{
	// Case 1: Cell is in the closed list, ignore it.
	if (a_Cell->m_Status == CLOSEDLIST)
	{
		return;
	}
	
	if (a_Cell->m_Status == NOLIST)  // Case 2: The cell is not in any list.
	{
		// Cell is walkable, add it to the open list.
		// Note that non-walkable cells are filtered out in Step_internal();
		// Special case: Start cell goes here, gDelta is 0, caller is NULL.
		a_Cell->m_Parent = a_Caller;
		if (a_Caller != NULL)
		{
			a_Cell->m_G = a_Caller->m_G + a_GDelta;
		}
		else
		{
			a_Cell->m_G = 0;
		}
		
		// Calculate m_H. This is A*'s Heuristics value.
		#if DISTANCE_MANHATTEN == 1
		a_Cell->m_H = 10 * (abs(a_Cell->m_Location.x-m_Destination.x) + abs(a_Cell->m_Location.y-m_Destination.y) + abs(a_Cell->m_Location.z-m_Destination.z));
		#else
		a_Cell->m_H = std::sqrt( (a_Cell->m_Location.x-m_Destination.x) * (a_Cell->m_Location.x-m_Destination.x) * 100+ (a_Cell->m_Location.y-m_Destination.y) * (a_Cell->m_Location.y-m_Destination.y) * 100 + (a_Cell->m_Location.z-m_Destination.z) * (a_Cell->m_Location.z-m_Destination.z) * 100);
		#endif
		
		
		#if HEURISTICS_ONLY == 1
		a_Cell->m_F = a_Cell->m_H;  // Depth-first search.
		#else
		a_Cell->m_F = a_Cell->m_H + a_Cell->m_G;  // Regular A*.
		#endif
		
		OpenListAdd(a_Cell);
		return;
	}
	

	
	// Case 3: Cell is in the open list, check if G and H need an update.
	int newG = a_Caller->m_G + a_GDelta;
	if (newG < a_Cell->m_G)
	{
		a_Cell->m_G = newG;
		a_Cell->m_H = a_Cell->m_F + a_Cell->m_G;
		a_Cell->m_Parent = a_Caller;
	}
	
}





ePathFinderStatus cPath::Step()
{
	if (m_Status == CALCULATING)
	{
		if (m_StepsLeft == 0)
		{
			FinishCalculation(PATH_NOT_FOUND);
		}
		else
		{
			--m_StepsLeft;
			int i;
			for (i = 0; i < CALCULATIONS_PER_CALL; ++i)
			{
				if (Step_Internal())  // Step_Internal returns true when no more calculation is needed.
				{
					break;  // if we're here, m_Status must have changed either to PATH_FOUND or PATH_NOT_FOUND.
				}
			}
		}
	}
	return m_Status;
}



void cPath::processIfWalkable(const Vector3d & a_Location, cPathCell * a_Parent, int a_Cost)
{
	cPathCell * cell =  GetCell(a_Location);
	if (!cell->m_IsSolid && GetCell(a_Location+Vector3d(0, -1, 0))->m_IsSolid && !GetCell(a_Location+Vector3d(0, 1, 0))->m_IsSolid)
	{
		ProcessCell(cell, a_Parent, a_Cost);
	}
}

bool cPath::Step_Internal()
{
	cPathCell * currentCell = OpenListPop();
	
	// Path not reachable, open list exauhsted.
	if (currentCell == NULL)
	{
		FinishCalculation(PATH_NOT_FOUND);
		ASSERT(m_Status == PATH_NOT_FOUND);
		return true;
	}
	
	// Path found.
	if (currentCell->m_Location == m_Destination)
	{
		do
		{
			addPoint(currentCell->m_Location);  // Populate the cPath with points.
			currentCell = currentCell->m_Parent;
		}
		while (currentCell != NULL);
		m_CurrentPoint = -1;
		FinishCalculation(PATH_FOUND);
		return true;
	}
	
	// Calculation not finished yet, process a currentCell by inspecting all 8 neighbors.
	
	// Forward, backward, left, right. on all 3 different heights.
	int i;
	for (i=-1; i<=1; ++i)
	{
		processIfWalkable(currentCell->m_Location + Vector3d(1, i, 0), currentCell, 10);
		processIfWalkable(currentCell->m_Location + Vector3d(-1, i, 0), currentCell, 10);
		processIfWalkable(currentCell->m_Location + Vector3d(0, i, 1), currentCell, 10);
		processIfWalkable(currentCell->m_Location + Vector3d(0, i, -1), currentCell, 10);
	}
	
	// Diagonals
	int x, z;
	for (x=-1; x<=1; x+=2)
	{
		for (z=-1; z<=1; z+=2)
		{
			// This condition prevents diagonal corner cutting.
			if (!GetCell(currentCell->m_Location + Vector3d(x, 0, 0))->m_IsSolid && !GetCell(currentCell->m_Location + Vector3d(0, 0, z))->m_IsSolid)
			{
				processIfWalkable(currentCell->m_Location + Vector3d(x, 0, z), currentCell, 14);  // 14 is a good enough approximation of sqrt(10 + 10).
			}
		}
	}
	
	
	return false;
}





void cPath::OpenListAdd(cPathCell * a_Cell)
{
	a_Cell->m_Status = OPENLIST;
	m_OpenList.push(a_Cell);
	#ifdef COMPILING_PATHFIND_DEBUGGER
	si::setBlock(a_Cell->m_Location.x, a_Cell->m_Location.y, a_Cell->m_Location.z, debug_open, SetMini(a_Cell));
	#endif
}





cPathCell * cPath::OpenListPop()  // Popping from the open list also means adding to the closed list.
{
	if (m_OpenList.size() == 0)
	{
		return NULL;  // We've exhausted the search space and nothing was found, this will trigger a PATH_NOT_FOUND status.
	}
	
	cPathCell * ret = m_OpenList.top();
	m_OpenList.pop();
	ClosedListAdd(ret);
	#ifdef COMPILING_PATHFIND_DEBUGGER
	si::setBlock((ret)->m_Location.x, (ret)->m_Location.y, (ret)->m_Location.z, debug_closed, SetMini(ret));
	#endif
	return ret;
}





void cPath::ClosedListAdd(cPathCell * a_Point)
{
	a_Point->m_Status = CLOSEDLIST;
}




// Add the next point in the final path.
void cPath::addPoint(Vector3d a_Vector)
{
	m_PathPoints.push_back(a_Vector);
	++m_PointCount;
}
