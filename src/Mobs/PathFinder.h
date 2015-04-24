#pragma once
#include <vector>
#include <queue>
#include <unordered_map>
#include "Path.h"
using namespace std;

/* Note: the __PATHFIND_DEBUG__ is used by Native to debug this class outside of MCServer.
This preprocessor flag is never set when compiling MCServer. */


/* MCServer forward declarations */
#ifndef __PATHFIND_DEBUG__
// fwd: Vector3.h
template <typename T> class Vector3;
typedef Vector3<double> Vector3d;

// fwd: cChunkMap.h
typedef cItemCallback<cChunk> cChunkCallback;




/* Debug headers, not used in MCServer */
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
namespace std
{
template <>
struct hash<Vector3d>
{
	std::size_t operator()(const Vector3d & v2) const
	{
		// Guaranteed to have no hash collisions for any 128x128x128 area
		// Suitable for pathfinding
		size_t t=0;
		t+=(char)v2.x;
		t=t << 8;
		t+=(char)v2.y;
		t=t << 8;
		t+=(char)v2.z;
		t=t << 8;
		return t;
	}
};
}
#endif





/* Various minor structs and classes */
enum ePathFinderStatus {IDLE,  CALCULATING,  PATH_FOUND,  PATH_NOT_FOUND};
enum eCellStatus {OPENLIST,  CLOSEDLIST,  NOLIST};
struct cPathCell;
class compareHeuristics
{
public:
	bool operator()(cPathCell * & v1,  cPathCell * &  v2);
};








class cPathFinder
#ifndef __PATHFIND_DEBUG__
: public cChunkCallback
#endif
{
	
	/* The interface starts here */
	
public:
	
	static void consoleCommand();  // Temporary, used for debugging, called when one writes "pathfind" in the console.
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
	After calling this, you are expected to call Step() once per tick or once per several ticks until it returns true. You should then call getPath() to obtain the path.
	Calling this before a path is found resets the current path and starts another search.
	@param a_StartingPoint The function expects this position to be the lowest block the mob is in, a rule of thumb: "The block where the Zombie's knees are at".
	@param a_EndingPoint "The block where the Zombie's knees want to be".
	@param a_MaxSteps The maximum steps before giving up. */
	void StartPathFinding(int a_MaxSteps,  const Vector3d & a_StartingPoint,  const Vector3d & a_EndingPoint);
	
	/** Performs part of the path calculation and returns true if the path computation has finished.
	One must call StartPathFinding first, one usually calls getPath right after this function returns true.*/
	bool Step();
	
	/** Returns the calculated path. one must call this only after isCalculationFinished returns true.
	Returns null if no path was found or if a_MaxSearch caused the algorithm to give up.
	Note that the user is responsible for deleting the path or storing it for later.*/
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
	// Query our hosting world and ask it if there's a solid at a_location.
	static bool IsSolid(const Vector3d & a_Location);
	// The public version just calls this version * CALCULATIONS_PER_CALL times.
	bool Step_Internal();
	void clearPath();
	
	
	/* Openlist and closedlist management */
	void OpenListAdd(cPathCell * a_Cell);
	cPathCell * OpenListPop();
	void ClosedListAdd(cPathCell * a_Point);
	bool IsInOpenList(cPathCell * a_Point);
	bool IsInClosedList(cPathCell * a_Point);
	
	
	/* Map management */
	void ProcessCell(const Vector3d & a_Location,  cPathCell * a_Caller,  int a_GDelta);
	cPathCell* GetCell(const Vector3d & a_location);
	
	
	/* Pathfinding fields */
	cPath * m_Path;
	priority_queue<cPathCell *,  vector<cPathCell *>,  compareHeuristics> m_OpenList;
	unordered_map<Vector3d,  cPathCell *> m_Map;
	Vector3d m_Destination;
	Vector3d m_Source;
	int m_StepsLeft;
	
	
	/* Control fields */
	bool m_PathFound;
	ePathFinderStatus m_Status;

	/* Interfacing with MCServer's world */
#ifndef __PATHFIND_DEBUG__
protected:
	virtual bool Item(cChunk * a_Chunk) override;
#endif
	
	friend class cPathCell;
};
