#include "PathFinder.h"
#include "Path.h"
#include <stdio.h>
#include <exception>

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





struct cPathCell
{
	Vector3d m_Location;
	int m_f, m_g, m_h;
	eCellStatus m_Status;
	cPathCell * m_Parent;
	bool m_IsSolid;
};





bool compareHeuristics::operator()(cPathCell * & a_v1, cPathCell * & a_v2)
{
	return a_v1->m_f > a_v2->m_f;
}





/* cPath implementation */
#ifdef __PATHFIND_DEBUG__
int CACHE_CNT = 0;
#endif
bool cPathFinder::isSolid(const Vector3d & a_Location)
{
#ifdef __PATHFIND_DEBUG__
	bool ret = si::checkTexture(a_Location.x, a_Location.y, a_Location.z, debug_solid);
	printf("Cache #%d\n", ++CACHE_CNT);
	return ret;
#else
	IMPLEMENT_ME_OR_ELSE_I_WONT_COMPILE
		#endif
}





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
	for (std::unordered_map<Vector3d, cPathCell *>::iterator it = m_Map.begin() ; it != m_Map.end(); ++it)
	{
		delete (it->second);
	}
	
	delete m_Path;
	m_Path = NULL;
	m_Map.clear();
}





void cPathFinder::findPath(int a_MaxSearch, const Vector3d & a_StartingPoint, const Vector3d & a_EndingPoint)
{
	if (m_Path != NULL)
	{
		clearPath();
	}
	m_Path = new cPath();
	m_Source = a_StartingPoint;
	m_Destination = a_EndingPoint;
	if (getCell(a_StartingPoint)->m_IsSolid || getCell(a_EndingPoint)->m_IsSolid)
	{
		char str[100];
		sprintf(str, "Pathfinding into a solid\n");
		throw pathFindException(str);
	}
	m_Status = CALCULATING;
	
	processCell(a_StartingPoint, NULL, 0);
}





cPathCell * cPathFinder::getCell(const Vector3d & a_Location)
{
	// Create the cell in the hash table if it's not already there.
	cPathCell * cell;
	if (m_Map.count(a_Location) == 0)  // Case 1: Cell is not on any list. We've never checked this cell before.
	{
		cell = new cPathCell();
		cell->m_Location = a_Location;
		m_Map[a_Location] = cell;
		cell->m_IsSolid = cPathFinder::isSolid(a_Location);
		cell->m_Status = NOLIST;
#ifdef __PATHFIND_DEBUG__
		si::setBlock(a_Location.x, a_Location.y, a_Location.z, debug_unchecked, !cell->m_IsSolid);
#endif
		return cell;
	}
	else return m_Map[a_Location];
}





void cPathFinder::processCell(const Vector3d & a_Location, cPathCell * a_Caller, int a_GDelta)
{
	
	cPathCell * cell = getCell(a_Location);
	if (cell->m_Status == NOLIST)
	{
		
		// Case 1: Cell is walkable, add it to the open list.
		// Special case: Start cell goes here, gDelta is 0, caller is NULL.
		cell->m_Parent = a_Caller;
		if (a_Caller != NULL)
		{
			cell->m_g = a_Caller->m_g + a_GDelta;
		}
		else cell->m_g = 0;
		
#if DISTANCE_MANHATTEN == 1
		cell->m_h = 10 *(abs(cell->m_Location.x-m_Destination.x) + abs(cell->m_Location.y-m_Destination.y)+ abs(cell->m_Location.z-m_Destination.z));
#else
		cell->m_h = sqrt( (cell->m_Location.x-m_Destination.x) * (cell->m_Location.x-m_Destination.x) * 100+ (cell->m_Location.y-m_Destination.y) *(cell->m_Location.y-m_Destination.y) * 100 + (cell->m_Location.z-m_Destination.z) * (cell->m_Location.z-m_Destination.z) * 100);
#endif
		
#if HEURISTICS_ONLY == 1
		cell->m_f = cell->m_h;
#else
		cell->m_f = cell->m_h + cell->m_g;
#endif
		
		openListAdd(cell);
		return;
	}
	
	
	cell = m_Map[a_Location];
	
	// Case 2: Cell is in the closed list, ignore it.
	if (cell->m_Status == CLOSEDLIST)
	{
		return;
	}
	
	// Case 4: Cell is in the open list, check if G and H need an update.
	int newG = cell->m_Parent->m_g + a_GDelta;
	if (newG<cell->m_g)
	{
		cell->m_g = newG;
		cell->m_h = cell->m_f + cell->m_g;  // TODO fix
		cell->m_Parent = a_Caller;
	}
	
}





bool cPathFinder::isCalculationFinished()
{
	int i;
	for (i = 0; i<CALCULATIONS_PER_CALL; ++i)
	{
		if (isCalculationFinished_internal())
		{
			return true;
		}
	}
	return false;
}


bool cPathFinder::isCalculationFinished_internal()
{
	if (m_Status == PATH_FOUND)
	{
		return true;
	}
	
	cPathCell * currentCell = openListPop();
	if (currentCell == NULL)
	{
		m_Status = PATH_NOT_FOUND;
		clearPath();
		return true;
	}
	
	if (currentCell->m_Location == m_Destination)
	{
		m_Status = PATH_FOUND;
		for (;;)
		{
			// si::setBlock(currentCell->m_Location.x, currentCell->m_Location.y, currentCell->m_Location.z, debug_closed, false);
			m_Path->addPoint(currentCell->m_Location);
			currentCell = currentCell->m_Parent;
			if (currentCell == NULL)
			{
				return true;
			}
		}
		
	}
	
	int x, y, z;
	for (x = -1; x <= 1; ++x)
	{
		for (y = -1; y <= 1; ++y)
		{
			for (z = -1; z <= 1; ++z)
			{
				// TODO There's room for optimization here
				int cost = sqrt(x * x * 100 + y * y * 100 + z * z * 100);
				
				if (!getCell(currentCell->m_Location+Vector3d(x, y, z))->m_IsSolid && getCell(currentCell->m_Location+Vector3d(x, y, z-1))->m_IsSolid && !getCell(currentCell->m_Location+Vector3d(x, y, z+1))->m_IsSolid)
				{
					processCell(currentCell->m_Location+Vector3d(x, y, z), currentCell, cost);
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
		return NULL;
	}
	if (m_Status == CALCULATING)
	{
		throw pathFindException("cPathFinder::getPath was called before calculation had finished\n");
	}
	if (m_Status == IDLE)
	{
		throw pathFindException("cPathFinder::getPath was called before cPathFinder::findPath.\n");
	}
	cPath * temp = m_Path;
	m_Path = NULL;
	m_Status = IDLE;
	m_Map.clear();  // Todo clearing in two places is not tidy, I should to re-think this.
	return temp;
}





void cPathFinder::openListAdd(cPathCell * a_Cell)
{
	a_Cell->m_Status = OPENLIST;
	m_OpenList.push(a_Cell);
#ifdef __PATHFIND_DEBUG__
	si::setBlock(a_Cell->m_Location.x, a_Cell->m_Location.y, a_Cell->m_Location.z, debug_open, true);
#endif
}





cPathCell * cPathFinder::openListPop()
{
	if (m_OpenList.size() == 0)
	{
		return NULL;
	}
	cPathCell * ret = m_OpenList.top();
	m_OpenList.pop();
	closedListAdd(ret);
#ifdef __PATHFIND_DEBUG__
	si::setBlock((ret)->m_Location.x, (ret)->m_Location.y, (ret)->m_Location.z, debug_closed, true);
#endif
	return ret;
	
	
	
}





void cPathFinder::closedListAdd(cPathCell * point)
{
	point->m_Status = CLOSEDLIST;
}





Vector3d::Vector3d(int _x, int _y, int _z)
{
	x = _x;
	y = _y;
	z = _z;
}





Vector3d Vector3d::operator+ (const Vector3d & v2) const
{
	return Vector3d(this->x+v2.x, this->y+v2.y, this->z+v2.z);
}





bool Vector3d::operator == (const Vector3d & v2) const
{
	return ((this->x == v2.x) && (this->y == v2.y) && (this->z == v2.z));
}


