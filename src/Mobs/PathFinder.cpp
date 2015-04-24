#ifndef __PATHFIND_DEBUG__
/* MCServer headers */
#include "Globals.h"
#include "../World.h"
#endif

#include "PathFinder.h"
#include "Path.h"

#include <stdio.h>
#include <exception>
#include <cmath>


#define DISTANCE_MANHATTEN 0  // 1: More speed, a bit less accuracy 0: Max accuracy, less speed.
#define HEURISTICS_ONLY 0  // 1: Much more speed, much less accurate.
#define CALCULATIONS_PER_CALL 30  // Higher means more CPU load but faster path calculations.
// The only version which guarantees the shortest path is 0, 0.

#ifdef __PATHFIND_DEBUG__
txt cPathFinder::debug_solid;
txt cPathFinder::debug_open;
txt cPathFinder::debug_closed;
txt cPathFinder::debug_unchecked;
void cPathFinder::debug_SetSolidBlock(txt texture)
{
	debug_solid = texture;
}
void cPathFinder::debug_SetUncheckedBlock(txt texture)
{
	debug_unchecked = texture;
}
void cPathFinder::debug_SetClosedBlock(txt texture)
{
	debug_closed = texture;
}
void cPathFinder::debug_SetOpenBlock(txt texture)
{
	debug_open = texture;
}
#endif





/* Mini structs */
struct pathFindException : public std::exception
{
	const char * s;
	pathFindException(const char * ss) : s(ss) {}
	~pathFindException() throw () {}
	const char * what() const throw()
	{
		return s;
	}
};




// Each cell corresponds to
struct cPathCell
{
	Vector3d m_Location;   // Location of the cell in the world
	int m_F, m_G, m_H;     // F, G, H as defined in regular A*.
	eCellStatus m_Status;  // Which list is the cell in? Either non, open, or closed.
	cPathCell * m_Parent;  // Cell's parent, as defined in regular A*.
	bool m_IsSolid;	       // Is the cell an air or a solid? Partial solids are currently considered solids.
};





bool compareHeuristics::operator()(cPathCell * & a_v1, cPathCell * & a_v2)
{
	return a_v1->m_F > a_v2->m_F;
}





/* cPath implementation */
#ifdef __PATHFIND_DEBUG__
int CACHE_CNT = 0;
bool cPathFinder::isSolid(const Vector3d & a_Location)  // external debugger only. This has nothing to do with MCServer!
{
	bool ret = si::checkTexture(a_Location.x, a_Location.y, a_Location.z, debug_solid);
	printf("Cache #%d\n", ++CACHE_CNT);
	return ret;
}





#else
bool cPathFinder::IsSolid(const Vector3d & a_Location)  // MCServer version of isSolid
{
	return true;
	/* TODO complete this */
	// int ChunkX, ChunkZ;
	// cChunkDef::BlockToChunk(a_Location.x, a_Location.z, ChunkX, ChunkZ);
	// return m_World->DoWithChunk(ChunkX, ChunkZ, *this);
}





bool cPathFinder::Item(cChunk * a_Chunk)
{
	printf("cPathFinder::item Mindelessly returning true\n");
	// TODO intelligently return the right state here.
	// TODO I probably need to store a_Location.x and a_Location.y as fields, is that correct?
	// TODO Maybe I should later queue several blocks and call this at once for all of them for better performance?
	return true;
}





void cPathFinder::consoleCommand()
{
	printf("HELLO WORLD\n");
	printf("cPathFinder::isSolid returned %s.\n", cPathFinder().IsSolid(Vector3d(0, 0, 0)) ? "true" : "false");
}
#endif





cPathFinder::cPathFinder(double a_BoundingBoxWidth, double a_BoundingBoxHeight, int a_MaxUp, int a_MaxDown)
{
	m_PathFound = false;
	m_Path = NULL;
	m_Status = IDLE;
}





cPathFinder::~cPathFinder()
{
	clearPath();
}





void cPathFinder::clearPath()
{
	for (std::unordered_map<Vector3d, cPathCell *>::iterator it = m_Map.begin(); it != m_Map.end(); ++it)
	{
		delete (it->second);
	}
	
	delete m_Path;
	m_Path = NULL;
	m_Map.clear();
	m_Status = IDLE;
}





void cPathFinder::StartPathFinding(int a_MaxSteps, const Vector3d & a_StartingPoint, const Vector3d & a_EndingPoint)
{
	if (m_Status == CALCULATING)
	{
		clearPath();  // Destroy older path and set status to IDLE
	}
	if (GetCell(a_StartingPoint)->m_IsSolid || GetCell(a_EndingPoint)->m_IsSolid)
	{
		m_Status = PATH_NOT_FOUND;
		return;
	}
	
	m_Status = CALCULATING;
	m_Path = new cPath();
	m_Source = a_StartingPoint;
	m_Destination = a_EndingPoint;
	m_StepsLeft = a_MaxSteps;
	ProcessCell(a_StartingPoint, NULL, 0);
}





cPathCell * cPathFinder::GetCell(const Vector3d & a_Location)
{
	// Create the cell in the hash table if it's not already there.
	cPathCell * cell;
	if (m_Map.count(a_Location) == 0)  // Case 1: Cell is not on any list. We've never checked this cell before.
	{
		cell = new cPathCell();
		cell->m_Location = a_Location;
		m_Map[a_Location] = cell;
		cell->m_IsSolid = cPathFinder::IsSolid(a_Location);
		cell->m_Status = NOLIST;
		#ifdef __PATHFIND_DEBUG__
		si::setBlock(a_Location.x, a_Location.y, a_Location.z, debug_unchecked, !cell->m_IsSolid);
		#endif
		return cell;
	}
	else return m_Map[a_Location];
}





void cPathFinder::ProcessCell(const Vector3d & a_Location, cPathCell * a_Caller, int a_GDelta)
{
	cPathCell * cell = GetCell(a_Location);  // Convert a Vector3d to its corresponding cPathCell
	
	// Case 1: Cell is in the closed list, ignore it.
	if (cell->m_Status == CLOSEDLIST)
	{
		return;
	}
	
	if (cell->m_Status == NOLIST)  // Case 2: The cell is not in any list.
	{
		// Cell is walkable, add it to the open list.
		// Note that non-walkable cells are filtered out in Step_internal();
		// Special case: Start cell goes here, gDelta is 0, caller is NULL.
		cell->m_Parent = a_Caller;
		if (a_Caller != NULL)
		{
			cell->m_G = a_Caller->m_G + a_GDelta;
		}
		else
		{
			cell->m_G = 0;
		}
		
		// Calculate m_H. This is A*'s Heuristics value.
		#if DISTANCE_MANHATTEN == 1
		cell->m_H = 10 * (abs(cell->m_Location.x-m_Destination.x) + abs(cell->m_Location.y-m_Destination.y) + abs(cell->m_Location.z-m_Destination.z));
		#else
		cell->m_H = std::sqrt( (cell->m_Location.x-m_Destination.x) * (cell->m_Location.x-m_Destination.x) * 100+ (cell->m_Location.y-m_Destination.y) * (cell->m_Location.y-m_Destination.y) * 100 + (cell->m_Location.z-m_Destination.z) * (cell->m_Location.z-m_Destination.z) * 100);
		#endif
		
		
		#if HEURISTICS_ONLY == 1
		cell->m_F = cell->m_H;  // Depth-first search.
		#else
		cell->m_F = cell->m_H + cell->m_G;  // Regular A*.
		#endif
		
		OpenListAdd(cell);
		return;
	}
	

	
	// Case 3: Cell is in the open list, check if G and H need an update.
	int newG = cell->m_Parent->m_G + a_GDelta;
	if (newG < cell->m_G)
	{
		cell->m_G = newG;
		cell->m_H = cell->m_F + cell->m_G;
		cell->m_Parent = a_Caller;
	}
	
}





bool cPathFinder::Step()
{
	// This merely calls Step_Internal several times
	if (m_StepsLeft-- == 0)
	{
		m_Status = PATH_NOT_FOUND;
		return true;
	}
	
	int i;
	for (i = 0; i < CALCULATIONS_PER_CALL; ++i)
	{
		if (Step_Internal())
		{
			return true;
		}
	}
	return false;
}





bool cPathFinder::Step_Internal()
{
	// If we're not calculating, return true.
	if (m_Status != CALCULATING)
	{
		return true;
	}
	
	cPathCell * currentCell = OpenListPop();
	if (currentCell == NULL)  // If nothing is in the open list, return true.
	{
		clearPath();
		m_Status = PATH_NOT_FOUND;
		return true;
	}
	
	// Calculation finished!
	if (currentCell->m_Location == m_Destination)
	{
		m_Status = PATH_FOUND;
		
		for (;;)
		{
			m_Path->addPoint(currentCell->m_Location);  // Populate the cPath with points.
			currentCell = currentCell->m_Parent;
			if (currentCell == NULL)
			{
				return true;
			}
		}
		
	}
	
	// Calculation not finished yet, process a currentCell by inspecting all 8 neighbors
	int x, y, z;
	for (x = -1; x <= 1; ++x)
	{
		for (y = -1; y <= 1; ++y)
		{
			for (z = -1; z <= 1; ++z)
			{
				// TODO There's room for optimization here
				int cost = std::sqrt(x * x * 100 + y * y * 100 + z * z * 100);
				
				// If this neighbor: A. isn't solid. B. Has ground beneath. C. Has air above.
				if (!GetCell(currentCell->m_Location+Vector3d(x, y, z))->m_IsSolid && GetCell(currentCell->m_Location+Vector3d(x, y, z-1))->m_IsSolid && !GetCell(currentCell->m_Location+Vector3d(x, y, z+1))->m_IsSolid)
				{
					// ...Then we process it.
					ProcessCell(currentCell->m_Location+Vector3d(x, y, z), currentCell, cost);
				}
			}
		}
	}
	return false;
}





cPath * cPathFinder::getPath()
{
	if (m_Status == PATH_NOT_FOUND)
	{
		clearPath();
		return NULL;
	}
	if (m_Status == CALCULATING)
	{
		// TODO ASSERT INSTEAD
		throw pathFindException("cPathFinder::getPath was called before calculation had finished\n");
	}
	if (m_Status == IDLE)
	{
		// TODO ASSERT INSTEAD
		// TODO this might be changed, depending on what we decide regarding cPath.
		throw pathFindException("cPathFinder::getPath was called before cPathFinder::StartPathFinding or twice after path calculation completed.\n");
	}
	
	// m_Status is PATH_FOUND
	cPath * temp = m_Path;
	clearPath();
	return temp;
}





void cPathFinder::OpenListAdd(cPathCell * a_Cell)
{
	a_Cell->m_Status = OPENLIST;
	m_OpenList.push(a_Cell);
	#ifdef __PATHFIND_DEBUG__
	si::setBlock(a_Cell->m_Location.x, a_Cell->m_Location.y, a_Cell->m_Location.z, debug_open, true);
	#endif
}





cPathCell * cPathFinder::OpenListPop()
{
	if (m_OpenList.size() == 0)
	{
		return NULL;
	}
	cPathCell * ret = m_OpenList.top();
	m_OpenList.pop();
	ClosedListAdd(ret);
	#ifdef __PATHFIND_DEBUG__
	si::setBlock((ret)->m_Location.x, (ret)->m_Location.y, (ret)->m_Location.z, debug_closed, true);
	#endif
	return ret;
}





void cPathFinder::ClosedListAdd(cPathCell * point)
{
	point->m_Status = CLOSEDLIST;
}





#ifdef __PATHFIND_DEBUG__
Vector3d::Vector3d(int _x, int _y, int _z)
{
	x = _x;
	y = _y;
	z = _z;
}





Vector3d Vector3d::operator + (const Vector3d & v2) const
{
	return Vector3d(this->x+v2.x, this->y+v2.y, this->z+v2.z);
}





bool Vector3d::operator == (const Vector3d & v2) const
{
	return ((this->x == v2.x) && (this->y == v2.y) && (this->z == v2.z));
}
#endif
