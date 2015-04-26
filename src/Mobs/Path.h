#pragma once
#ifdef COMPILING_PATHFIND_DEBUGGER
#include "PathFinderIrrlicht_Head.h"
#endif
#include <vector>
#include <queue>
#include <unordered_map>
#include "Path.h"
using namespace std;

/* Note: the COMPILING_PATHFIND_DEBUGGER flag is used by Native to debug this class outside of MCServer.
This preprocessor flag is never set when compiling MCServer. */

/* MCServer forward declarations */
#ifndef COMPILING_PATHFIND_DEBUGGER

// fwd: Vector3.h
template <typename T> class Vector3;
typedef Vector3<double> Vector3d;

// fwd: cChunkMap.h
typedef cItemCallback<cChunk> cChunkCallback;

#endif





/* Various minor structs and classes */
enum ePathFinderStatus {CALCULATING,  PATH_FOUND,  PATH_NOT_FOUND};
enum eCellStatus {OPENLIST,  CLOSEDLIST,  NOLIST};
struct cPathCell;
class compareHeuristics
{
public:
	bool operator()(cPathCell * & v1,  cPathCell * &  v2);
};





class cPath
#ifndef COMPILING_PATHFIND_DEBUGGER
: public cChunkCallback
#endif
{
	
	/* The interface starts here */
	
public:
	
	static void consoleCommand();  // Temporary, used for debugging, called when one writes "pathfind" in the console.
	/** Creates a pathfinder instance. A Mob will probably need a single pathfinder instance for its entire life.
	
	Note that if you have a man-sized mob (1x1x2, zombies, etc), you are advised to call this function without parameters
	because the declaration might change in later version of the pathFinder, and a parameter-less call always assumes a man-sized mob.
	
	If your mob is not man-sized, you are advised to use cPath(width, height), this would be compatible with future versions,
	but please be aware that as of now those parameters will be ignored and your mob will be assumed to be man sized.
	
	@param a_BoundingBoxWidth the character's boundingbox width in blocks. Currently the parameter is ignored and 1 is assumed.
	@param a_BoundingBoxHeight the character's boundingbox width in blocks. Currently the parameter is ignored and 2 is assumed.
	@param a_MaxUp the character's max jump height in blocks. Currently the parameter is ignored and 1 is assumed.
	@param a_MaxDown How far is the character willing to fall? Currently the parameter is ignored and 1 is assumed. */
	/** Attempts to find a path starting from source to destination.
	After calling this, you are expected to call Step() once per tick or once per several ticks until it returns true. You should then call getPath() to obtain the path.
	Calling this before a path is found resets the current path and starts another search.
	@param a_StartingPoint The function expects this position to be the lowest block the mob is in, a rule of thumb: "The block where the Zombie's knees are at".
	@param a_EndingPoint "The block where the Zombie's knees want to be".
	@param a_MaxSteps The maximum steps before giving up. */
	cPath(const Vector3d & a_StartingPoint, const Vector3d & a_EndingPoint, int a_MaxSteps, double a_BoundingBoxWidth = 1, double a_BoundingBoxHeight = 2, int a_MaxUp = 1, int a_MaxDown = 1);
	
	/** Destroys the path and frees its memory. */
	~cPath();
	
	/** Performs part of the path calculation and returns true if the path computation has finished. */
	ePathFinderStatus Step();
	
	Vector3d getFirstPoint()
	{
		ASSERT(m_Status == PATH_FOUND);
		m_CurrentPoint = m_PointCount - 1;
		return m_PathPoints[m_CurrentPoint];
	}
	
	/** Returns the next point in the path. Assumes you called getFirstPoint at least once before. */
	inline Vector3d getnextPoint()
	{
		ASSERT(m_Status == PATH_FOUND);
		return m_PathPoints[m_PointCount - 1 - (--m_CurrentPoint)];
	}
	
	/** Checks whether this is the last point or not. Never call getnextPoint when this is true. */
	inline bool isLastPoint()
	{
		ASSERT(m_Status == PATH_FOUND);
		return m_CurrentPoint == 0;
	}
	
	/** Get the point at a_index. Remark: Internally, the indexes are reversed. */
	inline Vector3d getPoint(int a_index)
	{
		ASSERT(m_Status == PATH_FOUND);
		ASSERT(a_index < m_PointCount);
		return m_PathPoints[m_PointCount - 1 - a_index];
	}
	
	/** Returns the total number of points this path has. */
	inline int getPointCount()
	{
		ASSERT(m_Status == PATH_FOUND);
		return m_PointCount;
	}
	
	/* The interface ends here */
	
	
	
	
	
	
	
	
	#ifdef COMPILING_PATHFIND_DEBUGGER
	static void debug_SetSolidBlock(txt texture);
	static void debug_SetOpenBlock(txt texture);
	static void debug_SetClosedBlock(txt texture);
	static void debug_SetUncheckedBlock(txt texture);
	#endif
	
	
	
	
private:
	
	#ifdef COMPILING_PATHFIND_DEBUGGER
	static txt debug_solid;
	static txt debug_open;
	static txt debug_closed;
	static txt debug_unchecked;
	#endif
	
	/* Misc */
	static bool IsSolid(const Vector3d & a_Location);  // Query our hosting world and ask it if there's a solid at a_location.
	bool Step_Internal();  // The public version just calls this version * CALCULATIONS_PER_CALL times.
	void FinishCalculation(ePathFinderStatus newStatus);  // Clears the memory used for calculating the path.
	
	
	/* Openlist and closedlist management */
	void OpenListAdd(cPathCell * a_Cell);
	cPathCell * OpenListPop();
	void processIfWalkable(const Vector3d &a_Location, cPathCell *a_Parent, int a_Cost);
	void ClosedListAdd(cPathCell * a_Point);
	bool IsInOpenList(cPathCell * a_Point);
	bool IsInClosedList(cPathCell * a_Point);
	
	
	/* Map management */
	void ProcessCell(cPathCell * a_Cell,  cPathCell * a_Caller,  int a_GDelta);
	cPathCell* GetCell(const Vector3d & a_location);
	
	
	/* Pathfinding fields */
	priority_queue<cPathCell *,  vector<cPathCell *>,  compareHeuristics> m_OpenList;
	unordered_map<Vector3d,  cPathCell *> m_Map;
	Vector3d m_Destination;
	Vector3d m_Source;
	int m_StepsLeft;
	
	/* Control fields */
	ePathFinderStatus m_Status;

	/* Final path fields */
	int m_PointCount;
	int m_CurrentPoint;
	vector<Vector3d> m_PathPoints;
	void addPoint(Vector3d a_Vector);
	
	/* Interfacing with MCServer's world */
	#ifndef COMPILING_PATHFIND_DEBUGGER
protected:
	virtual bool Item(cChunk * a_Chunk) override;
	#endif
};
