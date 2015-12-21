#include "Globals.h"
#include "PathFinder.h"
#include "../Chunk.h"





cPathFinder::cPathFinder(double a_MobWidth, double a_MobHeight) :
	m_Path(),
	m_GiveUpCounter(0),
	m_NotFoundCooldown(0)
{
	m_Width = a_MobWidth;
	m_Height = a_MobHeight;
}





ePathFinderStatus cPathFinder::GetNextWayPoint(cChunk & a_Chunk, const Vector3d & a_Source, Vector3d * a_Destination, Vector3d * a_OutputWaypoint, bool a_DontCare)
{
	m_FinalDestination = *a_Destination;
	m_Source = a_Source;

	// If a recent PATH_NOT_FOUND was returned, we rest for a few ticks.
	if (m_NotFoundCooldown > 0)
	{
		m_NotFoundCooldown -= 1;
		return ePathFinderStatus::CALCULATING;
	}

	// Tweak the destination. If something is wrong with the destination or the chunk, rest for a while.
	if (!EnsureProperDestination(a_Chunk))
	{
		m_NotFoundCooldown = 20;
		return ePathFinderStatus::PATH_NOT_FOUND;
	}

	// Rest is over. Prepare m_Path by calling ResetPathFinding.
	if (m_NotFoundCooldown == 0)
	{
		m_NotFoundCooldown = -1;
		ResetPathFinding(a_Chunk);
	}

	// If m_Path has not been initialized yet, initialize it.
	if (!m_Path->IsValid())
	{
		ResetPathFinding(a_Chunk);
	}

	switch (m_Path->CalculationStep(a_Chunk))
	{
		case ePathFinderStatus::NEARBY_FOUND:
		{
			m_NoPathToTarget = true;
			m_PathDestination = m_Path->AcceptNearbyPath();
			if (a_DontCare)
			{
				m_FinalDestination = m_PathDestination;
				*a_Destination = m_FinalDestination;  // Modify the mob's final destination because it doesn't care about reaching an exact spot
			}
			else
			{
				m_DeviationOrigin = m_FinalDestination;  // This is the only case in which m_DeviationOrigin != m_PathDestination
			}
			return ePathFinderStatus::CALCULATING;
			// The next call will trigger the PATH_FOUND case
		}

		case ePathFinderStatus::PATH_NOT_FOUND:
		{
			m_NotFoundCooldown = 20;
			return ePathFinderStatus::PATH_NOT_FOUND;
		}
		case ePathFinderStatus::CALCULATING:
		{
			return ePathFinderStatus::CALCULATING;
		}
		case ePathFinderStatus::PATH_FOUND:
		{
			m_GiveUpCounter -= 1;

			if ((m_GiveUpCounter == 0) || PathIsTooOld())
			{
				ResetPathFinding(a_Chunk);
				return ePathFinderStatus::CALCULATING;
			}

			if (m_Path->NoMoreWayPoints())
			{
				// We're always heading towards m_PathDestination.
				// If m_PathDestination is exactly m_FinalDestination, then we're about to reach the destination.
				if (m_PathDestination == m_FinalDestination)
				{
					*a_OutputWaypoint = m_FinalDestination;
					return ePathFinderStatus::PATH_FOUND;

				}
				else
				{
					// Otherwise, we've finished our approximate path and time to recalc.
					ResetPathFinding(a_Chunk);
					return ePathFinderStatus::CALCULATING;
				}
			}

			Vector3d Waypoint(m_WayPoint);
			Vector3d Source(m_Source);
			Waypoint.y = 0;
			Source.y = 0;

			if (m_Path->IsFirstPoint() || (((Waypoint - Source).SqrLength() < WAYPOINT_RADIUS) && (m_Source.y >= m_WayPoint.y)))
			{
				// if the mob has just started or if the mob reached a waypoint, give them a new waypoint.
				m_WayPoint = m_Path->GetNextPoint();
				m_GiveUpCounter = 40;
				return ePathFinderStatus::PATH_FOUND;
			}
			else
			{
				// Otherwise, the mob is still walking towards its waypoint, we'll patiently wait. We won't update m_WayPoint.
				*a_OutputWaypoint = m_WayPoint;
				return ePathFinderStatus::PATH_FOUND;
			}
		}
		#ifndef __clang__
		default:
		{
			return ePathFinderStatus::PATH_FOUND;
			// Fixes GCC warning: "control reaches end of non-void function".
		}
		#endif
	}
}





void cPathFinder::ResetPathFinding(cChunk &a_Chunk)
{
	m_GiveUpCounter = 40;
	m_NoPathToTarget = false;
	m_PathDestination = m_FinalDestination;
	m_DeviationOrigin = m_PathDestination;
	m_Path.reset(new cPath(a_Chunk, m_Source, m_PathDestination, 20, m_Width, m_Height));
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
				if ((x == 0) && (z == 0))
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





bool cPathFinder::PathIsTooOld() const
{
	size_t acceptableDeviation = m_Path->WayPointsLeft() / 2;
	if (acceptableDeviation == 0)
	{
		acceptableDeviation = 1;
	}
	if ((m_FinalDestination - m_DeviationOrigin).SqrLength() > acceptableDeviation * acceptableDeviation)
	{
		return true;
	}
	return false;
}
