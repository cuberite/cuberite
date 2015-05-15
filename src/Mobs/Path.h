
#pragma once

/* Wanna use the pathfinder? Put this in your header file:

// Fwd: cPath
enum class ePathFinderStatus;
class cPath;

Put this in your .cpp:
#include "...Path.h"
*/

#ifdef COMPILING_PATHFIND_DEBUGGER
	/* Note: the COMPILING_PATHFIND_DEBUGGER flag is used by Native / WiseOldMan95 to debug
	this class outside of MCServer. This preprocessor flag is never set when compiling MCServer. */
	#include "PathFinderIrrlicht_Head.h"
#endif

#include <unordered_map>

//fwd: ../Chunk.h
class cChunk;

/* Various little structs and classes */
enum class ePathFinderStatus {CALCULATING,  PATH_FOUND,  PATH_NOT_FOUND};
struct cPathCell;  // Defined inside Path.cpp
class compareHeuristics
{
public:
	bool operator()(cPathCell * & a_V1,  cPathCell * &  a_V2);
};

class cPath
{
public:
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
	cPath(
		cChunk & a_Chunk,
		const Vector3i & a_StartingPoint, const Vector3i & a_EndingPoint, int a_MaxSteps,
		double a_BoundingBoxWidth = 1, double a_BoundingBoxHeight = 2,
		int a_MaxUp = 1, int a_MaxDown = 1
	);

	/** Destroys the path and frees its memory. */
	~cPath();

	/** Performs part of the path calculation and returns true if the path computation has finished. */
	ePathFinderStatus Step(cChunk & a_Chunk);

	/* Point retrieval functions, inlined for performance. */
	/** Returns the next point in the path. */
	inline Vector3i GetNextPoint()
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		return m_PathPoints[m_PathPoints.size() - 1 - (++m_CurrentPoint)];
	}
	/** Checks whether this is the last point or not. Never call getnextPoint when this is true. */
	inline bool IsLastPoint()
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		return (m_CurrentPoint == m_PathPoints.size() - 1);
	}
	inline bool IsFirstPoint()
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		return (m_CurrentPoint == 0);
	}
	/** Get the point at a_index. Remark: Internally, the indexes are reversed. */
	inline Vector3i GetPoint(size_t a_index)
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		ASSERT(a_index < m_PathPoints.size());
		return m_PathPoints[m_PathPoints.size() - 1 - a_index];
	}
	/** Returns the total number of points this path has. */
	inline int GetPointCount()
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		return m_PathPoints.size();
	}

	struct VectorHasher
	{
		std::size_t operator()(const Vector3i & a_Vector) const
		{
			// Guaranteed to have no hash collisions for any 128x128x128 area. Suitable for pathfinding.
			int32_t t = 0;
			t += (int8_t)a_Vector.x;
			t = t << 8;
			t += (int8_t)a_Vector.y;
			t = t << 8;
			t += (int8_t)a_Vector.z;
			t = t << 8;
			return (size_t)t;
		}
	};
private:

	/* General */
	bool IsSolid(const Vector3i & a_Location);  // Query our hosting world and ask it if there's a solid at a_location.
	bool Step_Internal();  // The public version just calls this version * CALCULATIONS_PER_CALL times.
	void FinishCalculation();  // Clears the memory used for calculating the path.
	void FinishCalculation(ePathFinderStatus a_NewStatus);  // Clears the memory used for calculating the path and changes the status.

	/* Openlist and closedlist management */
	void OpenListAdd(cPathCell * a_Cell);
	cPathCell * OpenListPop();
	void ProcessIfWalkable(const Vector3i &a_Location, cPathCell * a_Parent, int a_Cost);

	/* Map management */
	void ProcessCell(cPathCell * a_Cell,  cPathCell * a_Caller,  int a_GDelta);
	cPathCell * GetCell(const Vector3i & a_location);

	/* Pathfinding fields */
	std::priority_queue<cPathCell *,  std::vector<cPathCell *>,  compareHeuristics> m_OpenList;
	std::unordered_map<Vector3i,  UniquePtr<cPathCell>, VectorHasher> m_Map;
	Vector3i m_Destination;
	Vector3i m_Source;
	int m_StepsLeft;

	/* Control fields */
	ePathFinderStatus m_Status;

	/* Final path fields */
	size_t m_CurrentPoint;
	std::vector<Vector3i> m_PathPoints;

	/* Interfacing with the world */
	cChunk * m_Chunk;  // Only valid inside Step()!
	#ifdef COMPILING_PATHFIND_DEBUGGER
	#include "../path_irrlicht.cpp"
	#endif
};
