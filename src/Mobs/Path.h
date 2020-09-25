
#pragma once

#include "ChunkDef.h"

/*
// Needed Fwds: cPath
enum class ePathFinderStatus;
class cPath;
*/


#include "../FastRandom.h"
#ifdef COMPILING_PATHFIND_DEBUGGER
	/* Note: the COMPILING_PATHFIND_DEBUGGER flag is used by Native / WiseOldMan95 to debug
	this class outside of Cuberite. This preprocessor flag is never set when compiling Cuberite. */
	#include "PathFinderIrrlicht_Head.h"
#endif


//fwd: ../Chunk.h
class cChunk;


/* Various little structs and classes */
enum class ePathFinderStatus {CALCULATING,  PATH_FOUND,  PATH_NOT_FOUND, NEARBY_FOUND};
enum class eCellStatus {OPENLIST,  CLOSEDLIST,  NOLIST};
/** The pathfinder has 3 types of cells (cPathCell).
1 - empty. m_IsSolid is false, m_IsSpecial is false.  Air cells are always traversable by A*.
2 - occupied / solid. m_IsSolid is true, m_IsSpecial is false.  Air cells are never traversable by A*.
3 - Special. m_IsSolid is true, m_IsSpecial is true. These cells are special: They may either behave as empty
or as occupied / solid, depending on the mob's direction of movement. For instance, an airblock above a fence is a special cell,
because when mobs attempt to travel to it by jumping, it acts as a solid. But when mobs fall and land on top of a fence,
it acts as air. Special cells include: Doors, ladders, trapdoors, water, gates.

The main function which handles special blocks is SpecialIsSolidFromThisDirection.
This function receives a BlockType, a meta, and a direction of travel,
then it uses those 3 parameters to decide whether the special block should behave as a solid or as air in this
particular direction of travel.

Currently, only fences and water are handled properly. The function always returns "true" (meaning: treat as occuiped/solid) for
the rest of the blocks. This will be fixed once the physics engine issues are fixed. */
struct cPathCell
{
	Vector3i m_Location;   // Location of the cell in the world.
	int m_F, m_G, m_H;  // F, G, H as defined in regular A*.
	eCellStatus m_Status;  // Which list is the cell in? Either non, open, or closed.
	cPathCell * m_Parent;  // Cell's parent, as defined in regular A*.
	bool m_IsSolid;	   // Is the cell an air or a solid? Partial solids are considered solids. If m_IsSpecial is true, this is always true.
	bool m_IsSpecial;  // The cell is special - it acts as "solid" or "air" depending on direction, e.g. door or top of fence.
	BLOCKTYPE m_BlockType;
	NIBBLETYPE m_BlockMeta;
};




class compareHeuristics
{
public:
	bool operator()(cPathCell * & a_V1,  cPathCell * &  a_V2);
};





class cPath
{
public:
	/** Creates a pathfinder instance.
	After calling this, you are expected to call CalculationStep() once per tick or once per several ticks
	until it returns something other than CALCULATING.

	@param a_StartingPoint The function expects this position to be the lowest block the mob is in, a rule of thumb: "The block where the Zombie's knees are at".
	@param a_EndingPoint "The block where the Zombie's knees want to be".
	@param a_MaxSteps The maximum steps before giving up.
	@param a_BoundingBoxWidth the character's boundingbox width in blocks. Currently the parameter is ignored and 1 is assumed.
	@param a_BoundingBoxHeight the character's boundingbox width in blocks. Currently the parameter is ignored and 2 is assumed. */
	cPath(
		cChunk & a_Chunk,
		const Vector3d & a_StartingPoint, const Vector3d & a_EndingPoint, int a_MaxSteps,
		double a_BoundingBoxWidth, double a_BoundingBoxHeight
	);

	/** Creates an invalid path which is not usable. You shouldn't call any method other than isValid on such a path. */
	cPath();

	/** delete default constructors */
	cPath(const cPath & a_other) = delete;
	cPath(cPath && a_other) = delete;

	cPath & operator=(const cPath & a_other) = delete;
	cPath & operator=(cPath && a_other) = delete;

	/** Performs part of the path calculation and returns the appropriate status.
	If PATH_FOUND is returned, the path was found, and you can call query the instance for waypoints via GetNextWayPoint, etc.
	If NEARBY_FOUND is returned, it means that the destination is not reachable, but a nearby destination
	is reachable. If the user likes the alternative destination, they can call AcceptNearbyPath to treat the path as found,
	and to make consequent calls to step return PATH_FOUND
	If PATH_NOT_FOUND is returned, then no path was found. */
	ePathFinderStatus CalculationStep(cChunk & a_Chunk);

	/** Called after the PathFinder's step returns NEARBY_FOUND.
	Changes the PathFinder status from NEARBY_FOUND to PATH_FOUND, returns the nearby destination that
	the PathFinder found a path to. */
	Vector3i AcceptNearbyPath();

	// Point retrieval functions, inlined for performance:

	/** Returns the next point in the path. */
	inline Vector3d GetNextPoint()
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		ASSERT(m_CurrentPoint < m_PathPoints.size());
		Vector3i Point = m_PathPoints[m_PathPoints.size() - 1 - m_CurrentPoint];
		++m_CurrentPoint;
		return Vector3d(Point.x + m_HalfWidth, Point.y, Point.z + m_HalfWidth);
	}


	/** Checks if we have no more waypoints to return. Never call getnextPoint when this is true. */
	inline bool NoMoreWayPoints() const
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		return (m_CurrentPoint == m_PathPoints.size());
	}

	/** Returns true if GetNextPoint() was never called for this Path. */
	inline bool IsFirstPoint() const
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		return (m_CurrentPoint == 0);
	}

	/** Returns true if this path is properly initialized.
	Returns false if this path was initialized with an empty constructor.
	If false, the path is unusable and you should not call any methods. */
	inline bool IsValid() const
	{
		return m_IsValid;
	}

	/** The amount of waypoints left to return. */
	inline size_t WayPointsLeft() const
	{
		ASSERT(m_Status == ePathFinderStatus::PATH_FOUND);
		return m_PathPoints.size() - m_CurrentPoint;
	}




private:

	/* General */
	bool StepOnce();  // The public version just calls this version * CALCULATIONS_PER_CALL times.
	void FinishCalculation();  // Clears the memory used for calculating the path.
	void FinishCalculation(ePathFinderStatus a_NewStatus);  // Clears the memory used for calculating the path and changes the status.
	void AttemptToFindAlternative();
	void BuildPath();

	/* Openlist and closedlist management */
	void OpenListAdd(cPathCell * a_Cell);
	cPathCell * OpenListPop();
	bool ProcessIfWalkable(const Vector3i & a_Location, cPathCell * a_Source, int a_Cost);

	/* Map management */
	void ProcessCell(cPathCell * a_Cell,  cPathCell * a_Caller,  int a_GDelta);
	cPathCell * GetCell(const Vector3i & a_location);

	/* Pathfinding fields */
	std::priority_queue<cPathCell *,  std::vector<cPathCell *>,  compareHeuristics> m_OpenList;
	std::unordered_map<Vector3i, cPathCell, VectorHasher<int>> m_Map;
	Vector3i m_Destination;
	Vector3i m_Source;
	int m_BoundingBoxWidth;
	int m_BoundingBoxHeight;
	double m_HalfWidth;
	int m_StepsLeft;
	cPathCell * m_NearestPointToTarget;

	/* Control fields */
	ePathFinderStatus m_Status;
	bool m_IsValid;

	/* Final path fields */
	size_t m_CurrentPoint;
	std::vector<Vector3i> m_PathPoints;

	/* Interfacing with the world */
	void FillCellAttributes(cPathCell & a_Cell);  // Query our hosting world and fill the cell with info
	cChunk * m_Chunk;  // Only valid inside Step()!
	bool m_BadChunkFound;

	/* High level world queries */
	bool IsWalkable(const Vector3i & a_Location, const Vector3i & a_Source);
	bool BodyFitsIn(const Vector3i & a_Location, const Vector3i & a_Source);
	bool BlockTypeIsSpecial(BLOCKTYPE a_Type);
	bool SpecialIsSolidFromThisDirection(BLOCKTYPE a_Type, NIBBLETYPE a_Meta,  const Vector3i & a_Direction);
	bool HasSolidBelow(const Vector3i & a_Location);
	#ifdef COMPILING_PATHFIND_DEBUGGER
	#include "../path_irrlicht.cpp"
	#endif
};
