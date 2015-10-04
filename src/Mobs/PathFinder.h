
#pragma once

#include "Path.h"

/*
TODO DOXY style

This class wraps cPath.
cPath is a "dumb device" - You give it point A and point B, and it returns a full path path.
cPathFinder - You give it a constant stream of point A (where you are) and point B (where you want to go),
and it tells you where to go next. It manages path recalculation internally, and is much more efficient that calling cPath every step.

*/

class cPathFinder
{

public:
	cPathFinder(double a_MobWidth, double a_MobHeight);
	ePathFinderStatus GetNextWayPoint(cChunk & a_Chunk, Vector3d a_Source, Vector3d a_Destination, Vector3d & a_OutputWaypoint);

private:

	double m_Width;
	double m_Height;

	cPath *  m_Path;  // TODO unique ptr

	/** Stores if mobile is currently moving towards the ultimate, final destination */
	bool m_IsFollowingPath;

	/* If 0, will give up reaching the next m_NextWayPointPosition and will re-compute path. */
	int m_GiveUpCounter;
	int m_TicksSinceLastPathReset;
	int m_JumpCoolDown;

	/** Coordinates of the next position that should be reached */
	Vector3d m_NextWayPointPosition;

	/** Coordinates for the ultimate, final destination. */
	Vector3d m_FinalDestination;

	/** Coordinates for the ultimate, final destination last given to the pathfinder. */
	Vector3d m_PathFinderDestination;

	/** */
	Vector3d m_Source;

	/** True if there's no path to target and we're walking to an approximated location. */
	bool m_NoPathToTarget;

	/** Whether The mob has finished their path, note that this does not imply reaching the destination,
	the destination may sometimes differ from the current path. */
	bool m_NoMoreWayPoints;

	/** Ensures the destination is not buried underground or under water. Also ensures the destination is not in the air.
	Only the Y coordinate of m_FinalDestination might be changed.
	1. If m_FinalDestination is the position of a water block, m_FinalDestination's Y will be modified to point to the heighest water block in the pool in the current column.
	2. If m_FinalDestination is the position of a solid, m_FinalDestination's Y will be modified to point to the first airblock above the solid in the current column.
	3. If m_FinalDestination is the position of an air block, Y will keep decreasing until hitting either a solid or water.
	Now either 1 or 2 is performed. */
	bool EnsureProperDestination(cChunk & a_Chunk);

	/** Resets a pathfinding task, be it due to failure or something else
	Resets the pathfinder. If m_IsFollowingPath is true, TickPathFinding starts a brand new path.
	Should only be called by the pathfinder, cMonster::Tick or StopMovingToPosition. */
	void ResetPathFinding(void);

};
