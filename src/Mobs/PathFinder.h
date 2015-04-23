#pragma once

/* Note: the __PATHFIND_DEBUG__ is used by Native to debug this class outside of MCServer.
This preprocessor flag is never set when compiling MCServer. */

/* Forward declarations */
struct cPathCell;
class cPathFinder;
class compareHeuristics;





/* MCServer headers */
#ifndef __PATHFIND_DEBUG__
//TODO FILL THESE




/* Debug headers */
#else
#include "si.h"
struct Vector3d
{
	Vector3d() {}
	Vector3d operator+ (const Vector3d & v2) const;
	bool operator== (const Vector3d & v2) const;
	Vector3d(int _x,  int _y,  int _z);
	int x,  y,  z;
};
#endif





/* STL headers */
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;





/* Hashtable */
namespace std
{
template <>
struct hash<Vector3d>
{
	std::size_t operator()(const Vector3d & v2) const
	{
		using std::size_t;
		using std::hash;
		using std::string;
		
		return(v2.x + v2.y  + v2.z);  // TODO this is not the best hash out there
	}
};
}




/* Various minor structs and classes */
#include "Path.h"
enum ePathFinderStatus {IDLE,  CALCULATING,  PATH_FOUND,  PATH_NOT_FOUND};
enum eCellStatus {OPENLIST,  CLOSEDLIST,  NOLIST};
struct cPathCell;
class compareHeuristics
{
public:
	bool operator()(cPathCell * & v1,  cPathCell * &  v2);
};








class cPathFinder
{
	
	/* The interface starts here */
	
public:
	
	/** Creates a pathfinder instance. A Mob will probably need a single pathfinder instance for its entire life.
	
	Note that if you have a man-sized mob (1x1x2, zombies, etc), you are advised to call this function without parameters
	because the declaration might change in later version of the pathFinder, and a parameter-less call always assumes a man-sized mob.
	
	If your mob is not man-sized, you are advised to use cPathFinder(width, height), this would be compatible with future versions,
	but please be aware that as of now those parameters will be ignored and your mob will be assumed to be man sized.
	
	@param a_BoundingBoxWidth the character's boundingbox width in blocks. Currently the parameter is ignored and 1 is assumed.
	@param a_BoundingBoxHeight the character's boundingbox width in blocks. Currently the parameter is ignored and 2 is assumed.
	@param a_MaxUp the character's max jump height in blocks. Currently the parameter is ignored and 1 is assumed.
	@param a_MaxDown How far is the character willing to fall? Currently the parameter is ignored and 1 is assumed. */
	cPathFinder(double a_BoundingBoxWidth = 1, double a_BoundingBoxHeight = 2, int a_MaxUp = 1, int a_MaxDown = 1);
	
	/** Destroys the pathFinder and frees its memory.
	Note that it does not free paths returned by getPath(); */
	~cPathFinder();
	
	/** Attempts to find a path starting from source to destination.
	After calling this, you are expected to call isCalculationFinished once per tick or once per several ticks until it returns true. You should then call getPath().
	@param a_StartingPoint The function expects this position to be the lowest block the mob is in, a rule of thumb: "The block where the Zombie's knees are at".
	@param a_EndingPoint "The block where the Zombie's knees want to be".
	@param a_MaxSearch The maximum nodes to scan, note that this isn't the same as the maximum distance. */
	void findPath(int a_MaxSearch,  const Vector3d & a_StartingPoint,  const Vector3d & a_EndingPoint);
	
	/** Performs part of the path calculation and returns true if the path computation has finished.
	One must call findPath first, one usually calls getPath right after this function returns true.*/
	bool isCalculationFinished();
	
	/** Returns the calculated path. one must call this only after isCalculationFinished returns true.
	Returns null if no path was found or if a_MaxSearch caused the algorithm to give up.
	Note that the user is responsible for deleting the path or storing it for later.
	It is safe to call findPath again after this. */
	cPath * getPath();
	
	/* The interface ends here */
	
	
	
	
	
	
	
	
#ifdef __PATHFIND_DEBUG__
	static void debug_SetSolidBlock(txt texture);
	static void debug_SetOpenBlock(txt texture);
	static void debug_SetClosedBlock(txt texture);
	static void debug_SetUncheckedBlock(txt texture);
#endif
	
	
	
private:
	
#ifdef __PATHFIND_DEBUG__
	static txt debug_solid;
	static txt debug_open;
	static txt debug_closed;
	static txt debug_unchecked;
#endif
	
	/* Misc */
	// Query our hosting world and ask it if there's a solid at a_dpos.
	static bool isSolid(const Vector3d & a_location);
	// The public version just calls this version multiple CALCULATIONS_PER_CALL times.
	bool isCalculationFinished_internal();
	void clearPath();
	
	
	/* Openlist and closedlist management */
	void openListAdd(cPathCell * a_Cell);
	cPathCell * openListPop();
	void closedListAdd(cPathCell * a_Point);
	bool isInOpenList(cPathCell * a_Point);
	bool isInClosedList(cPathCell * a_Point);
	
	
	/* Map management */
	void processCell(const Vector3d & a_Location,  cPathCell * a_Caller,  int a_GDelta);
	cPathCell* getCell(const Vector3d & a_location);
	
	
	/* Pathfinding fields */
	cPath * m_Path;
	priority_queue<cPathCell *,  vector<cPathCell *>,  compareHeuristics> m_OpenList;
	unordered_map<Vector3d,  cPathCell *> m_Map;
	Vector3d m_Destination;
	Vector3d m_Source;
	
	
	/* Control fields */
	bool m_PathFound;
	ePathFinderStatus m_Status;
	
	
	friend class cPathCell;
};


