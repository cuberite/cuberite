
#pragma once
#include "Path.h"

#define WAYPOINT_RADIUS 0.5

/** This class wraps cPath.
cPath is a "dumb device" - You give it point A and point B, and it returns a full path path.
cPathFinder - You give it a constant stream of point A (where you are) and point B (where you want to go),
and it tells you where to go next. It manages path recalculation internally, and is much more efficient that calling cPath every step. */
class cPathFinder
{

public:
	/** Creates a cPathFinder instance. Each mob should have one cPathFinder throughout its lifetime.
	@param a_MobWidth The mob width.
	@param a_MobHeight The mob height.
	*/
	cPathFinder(double a_MobWidth, double a_MobHeight);

	/** Updates the PathFinder's internal state and returns a waypoint.
	A waypoint is a coordinate which the mob can safely move to from its current position in a straight line.
	The mob is expected to call this function tick as long as it is following a path.
	@param a_Chunk The chunk in which the mob is currently at.
	@param a_Source The mob's position. a_Source's coordinates are expected to be within the chunk given in a_Chunk.
	@param a_Destination The position the mob would like to reach. If a_ExactPath is true, the PathFinder may modify this.
	@param a_OutputWaypoint An output parameter: The next waypoint to go to.
	@param a_DontCare If true, the mob doesn't care where to go, and the Pathfinder may modify a_Destination.
	This should usually be false. An exception is a wandering idle mob which doesn't care about its final destination.
	In the future, idle mobs shouldn't use A* at all.

	Returns an ePathFinderStatus.
	ePathFinderStatus:CALCULATING - The PathFinder is still processing a path. Nothing was written to a_OutputWaypoint. The mob should probably not move.
	ePathFinderStatus:PATH_FOUND - The PathFinder has found a path to the target. The next waypoint was written a_OutputWaypoint. The mob should probably move to a_OutputWaypoint.
	ePathFinderStatus:NEARBY_FOUND - The PathFinder did not find a destination to the target but did find a nearby spot. The next waypoint was written a_OutputWaypoint. The mob should probably move to a_OutputWaypoint.
	ePathFinderStatus:PATH_NOT_FOUND - The PathFinder did not find a destination to the target. Nothing was written to a_OutputWaypoint. The mob should probably not move.

	Note: Once NEARBY_FOUND is returned once, subsequent calls return PATH_FOUND. */
	ePathFinderStatus GetNextWayPoint(cChunk & a_Chunk, const Vector3d & a_Source, Vector3d * a_Destination, Vector3d * a_OutputWaypoint, bool a_DontCare = false);

private:

	/** The width of the Mob which owns this PathFinder. */
	double m_Width;

	/** The height of the Mob which owns this PathFinder. */
	double m_Height;

	/** The current cPath instance we have. This is discarded and recreated when a path recalculation is needed. */
	std::unique_ptr<cPath> m_Path;

	/** If 0, will give up reaching the next m_WayPoint and will recalculate path. */
	int m_GiveUpCounter;

	/** Coordinates of the next position that should be reached. */
	Vector3d m_WayPoint;

	/** Coordinates for where we should go. This is out ultimate, final destination. */
	Vector3d m_FinalDestination;

	/** Coordinates for where we are practically going. */
	Vector3d m_PathDestination;

	/** When FinalDestination is too far from this, we recalculate.
	This usually equals PathDestination. Except when m_NoPathToTarget is true. */
	Vector3d m_DeviationOrigin;


	/** Coordinates for where the mob is currently at. */
	Vector3d m_Source;

	/** True if there's no path to target and we're walking to a nearby location instead. */
	bool m_NoPathToTarget;

	/** When a path is not found, this cooldown prevents any recalculations for several ticks. */
	int m_NotFoundCooldown;

	/** Ensures the location is not in the air or under water.
	May change the Y coordinate of the given vector.
	1. If a_Vector is the position of water, a_Vector's Y will be modified to point to the first air block above it.
	2. If a_Vector is the position of air, a_Vector's Y will be modified to point to the first airblock below it which has solid or water beneath. */
	bool EnsureProperPoint(Vector3d & a_Vector, cChunk & a_Chunk);

	/** Resets a pathfinding task, typically because m_FinalDestination has deviated too much from m_DeviationOrigin. */
	void ResetPathFinding(cChunk &a_Chunk);

	/** Return true the the blocktype is either water or solid */
	bool IsWaterOrSolid(BLOCKTYPE a_BlockType);

	/** Is the path too old and should be recalculated? When this is true ResetPathFinding() is called. */
	bool PathIsTooOld() const;
};
