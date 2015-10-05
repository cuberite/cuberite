

#include "Globals.h"
#include "PathFinder.h"
#include "../Chunk.h"

cPathFinder::cPathFinder(double a_MobWidth, double a_MobHeight)
	: m_Path(nullptr)
	, m_GiveUpCounter(0)
	, m_TicksSinceLastPathReset(1000)
{
	m_Width = a_MobWidth;
	m_Height = a_MobHeight;

}





ePathFinderStatus cPathFinder::GetNextWayPoint(cChunk &a_Chunk, Vector3d a_Source, Vector3d a_Destination, Vector3d &a_OutputWaypoint)
{
	m_FinalDestination = a_Destination;
	m_Source = a_Source;

	if (m_TicksSinceLastPathReset < 1000)
	{
		// No need to count beyond 1000. 1000 is arbitary here.
		++m_TicksSinceLastPathReset;
	}

	/* if (ReachedFinalDestination())
	{
		StopMovingToPosition();
		return false;
	} */

	if ((m_FinalDestination - m_PathDestination).Length() > 0.25)  // if the distance between where we're going and where we should go is too big.
	{
		/* If we reached the last path waypoint,
		Or if we haven't re-calculated for too long.
		Interval is proportional to distance squared, and its minimum is 10.
		(Recalculate lots when close, calculate rarely when far) */
		if (
			((m_Source - m_PathDestination).Length() < 0.25) ||
			((m_TicksSinceLastPathReset > 10) && (m_TicksSinceLastPathReset > (0.4 * (m_FinalDestination - m_Source).SqrLength())))
		)
		{
			/* Re-calculating is expensive when there's no path to target, and it results in mobs freezing very often as a result of always recalculating.
			This is a workaround till we get better path recalculation. */
			if (!m_NoPathToTarget)
			{
				ResetPathFinding();
			}
		}
	}

	if (m_Path == nullptr)
	{
		if (!EnsureProperDestination(a_Chunk))
		{
			return ePathFinderStatus::PATH_NOT_FOUND;
		}

		m_GiveUpCounter = 40;
		m_NoPathToTarget = false;
		m_NoMoreWayPoints = false;
		m_PathDestination = m_FinalDestination;
		m_Path = new cPath(a_Chunk, m_Source, m_PathDestination, 20, m_Width, m_Height);
	}

	switch (m_Path->Step(a_Chunk))
	{
		case ePathFinderStatus::NEARBY_FOUND:
		{
			m_NoPathToTarget = true;
			m_PathDestination = m_Path->AcceptNearbyPath();
			return ePathFinderStatus::PATH_FOUND;
		}

		case ePathFinderStatus::PATH_NOT_FOUND:
		{
			return ePathFinderStatus::PATH_NOT_FOUND;
		}
		case ePathFinderStatus::CALCULATING:
		{
			m_TicksSinceLastPathReset = 0;  // Start ticking only after a path is found.
			return ePathFinderStatus::CALCULATING;
		}
		case ePathFinderStatus::PATH_FOUND:
		{
			if (m_NoMoreWayPoints || (--m_GiveUpCounter == 0))
			{
				ResetPathFinding();  // Try to calculate a path again.
				return ePathFinderStatus::CALCULATING;
			}
			else if (!m_Path->IsLastPoint())  // Have we arrived at the next cell, as denoted by m_NextWayPointPosition?
			{
				if ((m_Path->IsFirstPoint() || (m_NextWayPointPosition - m_Source).SqrLength() < 0.25))
				{
					m_NextWayPointPosition = m_Path->GetNextPoint();
					a_OutputWaypoint = m_NextWayPointPosition;
					m_GiveUpCounter = 40;  // Give up after 40 ticks (2 seconds) if failed to reach m_NextWayPointPosition.
				}
			}
			else
			{
				m_NoMoreWayPoints = true;
			}

			return ePathFinderStatus::PATH_FOUND;
		}
	}
}





void cPathFinder::ResetPathFinding(void)
{
	m_TicksSinceLastPathReset = 0;
	m_NoMoreWayPoints = false;
	if (m_Path != nullptr)
	{
		delete m_Path;
		m_Path = nullptr;
	}
}





bool cPathFinder::EnsureProperDestination(cChunk & a_Chunk)
{
	cChunk * Chunk = a_Chunk.GetNeighborChunk(FloorC(m_FinalDestination.x), FloorC(m_FinalDestination.z));
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;

	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}

	int RelX = FloorC(m_FinalDestination.x) - Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = FloorC(m_FinalDestination.z) - Chunk->GetPosZ() * cChunkDef::Width;

	// If destination in the air, first try to go 1 block north, or east, or west.
	// This fixes the player leaning issue.
	// If that failed, we instead go down to the lowest air block.
	Chunk->GetBlockTypeMeta(RelX, FloorC(m_FinalDestination.y) - 1, RelZ, BlockType, BlockMeta);
	if (!cBlockInfo::IsSolid(BlockType))
	{
		bool InTheAir = true;
		int x, z;
		for (z = -1; z <= 1; ++z)
		{
			for (x = -1; x <= 1; ++x)
			{
				if ((x==0) && (z==0))
				{
					continue;
				}
				Chunk = a_Chunk.GetNeighborChunk(FloorC(m_FinalDestination.x+x), FloorC(m_FinalDestination.z+z));
				if ((Chunk == nullptr) || !Chunk->IsValid())
				{
					return false;
				}
				RelX = FloorC(m_FinalDestination.x+x) - Chunk->GetPosX() * cChunkDef::Width;
				RelZ = FloorC(m_FinalDestination.z+z) - Chunk->GetPosZ() * cChunkDef::Width;
				Chunk->GetBlockTypeMeta(RelX, FloorC(m_FinalDestination.y) - 1, RelZ, BlockType, BlockMeta);
				if (cBlockInfo::IsSolid(BlockType))
				{
					m_FinalDestination.x += x;
					m_FinalDestination.z += z;
					InTheAir = false;
					goto breakBothLoops;
				}
			}
		}
		breakBothLoops:

		// Go down to the lowest air block.
		if (InTheAir)
		{
			while (m_FinalDestination.y > 0)
			{
				Chunk->GetBlockTypeMeta(RelX, FloorC(m_FinalDestination.y) - 1, RelZ, BlockType, BlockMeta);
				if (cBlockInfo::IsSolid(BlockType))
				{
					break;
				}
				m_FinalDestination.y -= 1;
			}
		}
	}

	// If destination in water, go up to the highest water block.
	// If destination in solid, go up to first air block.
	bool InWater = false;
	while (m_FinalDestination.y < cChunkDef::Height)
	{
		Chunk->GetBlockTypeMeta(RelX, FloorC(m_FinalDestination.y), RelZ, BlockType, BlockMeta);
		if (BlockType == E_BLOCK_STATIONARY_WATER)
		{
			InWater = true;
		}
		else if (cBlockInfo::IsSolid(BlockType))
		{
			InWater = false;
		}
		else
		{
			break;
		}
		m_FinalDestination.y += 1;
	}
	if (InWater)
	{
		m_FinalDestination.y -= 1;
	}


	return true;
}
