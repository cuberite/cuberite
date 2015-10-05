
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
	/** Creates a cPathFinder instance. Each mob should have one cPathFinder throughout its lifetime.
	@param a_MobWidth The mob width.
	@param a_MobWidth The mob height.
	*/
	cPathFinder(double a_MobWidth, double a_MobHeight);

	/** Updates the PathFinder's internal state and returns a waypoint.
	A waypoint is a coordinate which the mob can safely move to from its current position in a straight line.
	The mob is expected to call this function tick as long as it is following a path.
	@param a_Chunk The chunk in which the mob is currently at.
	@param a_Source The mob's position. a_Source's coordinates are expected to be within the chunk given in a_Chunk.
	@param a_Destination The position the mob would like to reach,
	@param a_OutputWaypoint An output parameter: The next waypoint to go to.

	Returns an ePathFinderStatus.
	ePathFinderStatus:CALCULATING - The PathFinder is still processing a path. Nothing was written to a_OutputWaypoint. The mob should probably not move.
	ePathFinderStatus:PATH_FOUND - The PathFinder has found a path to the target. The next waypoint was written a_OutputWaypoint. The mob should probably move to a_OutputWaypoint.
	ePathFinderStatus:NEARBY_FOUND - The PathFinder did not find a destination to the target but did find a nearby spot. The next waypoint was written a_OutputWaypoint. The mob should probably move to a_OutputWaypoint.
	ePathFinderStatus:PATH_NOT_FOUND - The PathFinder did not find a destination to the target. Nothing was written to a_OutputWaypoint. The mob should probably not move.

	Note: Once NEARBY_FOUND is returned once, subsequent calls return PATH_FOUND. This may be changed in the future. */
	ePathFinderStatus GetNextWayPoint(cChunk & a_Chunk, Vector3d a_Source, Vector3d a_Destination, Vector3d & a_OutputWaypoint);

private:

	/** The width of the Mob which owns this PathFinder. */
	double m_Width;

	/** The height of the Mob which owns this PathFinder. */
	double m_Height;

	/** The current cPath instance we have. This is discarded and recreated when a path recalculation is needed. */
	cPath *  m_Path;  // TODO unique ptr

	/** If 0, will give up reaching the next m_NextWayPointPosition and will recalculate path. */
	int m_GiveUpCounter;

	/** The ticks since the last recalculation was completed. */
	int m_TicksSinceLastPathReset;

	/** Coordinates of the next position that should be reached. */
	Vector3d m_NextWayPointPosition;

	/** Coordinates for the ultimate, final destination. */
	Vector3d m_FinalDestination;

	/** Coordinates for the destination last given to cPath. */
	Vector3d m_PathDestination;

	/** Coordinates for where the mob is currently at. */
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
