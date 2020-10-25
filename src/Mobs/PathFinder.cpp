#include "Globals.h"
#include "PathFinder.h"
#include "../BlockInfo.h"
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
	if (!(EnsureProperPoint(m_FinalDestination, a_Chunk) && EnsureProperPoint(m_Source, a_Chunk)))
	{
		m_NotFoundCooldown = 20;
		return ePathFinderStatus::PATH_NOT_FOUND;
	}

	/* printf("%d %d %d -> %d %d %d\n",
	static_cast<int>(m_Source.x),
	static_cast<int>(m_Source.y),
	static_cast<int>(m_Source.z),
	static_cast<int>(m_FinalDestination.x),
	static_cast<int>(m_FinalDestination.y),
	static_cast<int>(m_FinalDestination.z)); */

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

			if (m_GiveUpCounter == 0)
			{
				if (a_DontCare)
				{
					// We're having trouble reaching the next waypoint but the mob
					// Doesn't care where to go, just tell him we got there ;)
					m_FinalDestination = m_Source;
					*a_Destination = m_FinalDestination;
					ResetPathFinding(a_Chunk);
					return ePathFinderStatus::CALCULATING;
				}
				else
				{
					ResetPathFinding(a_Chunk);
					return ePathFinderStatus::CALCULATING;
				}
			}

			if (PathIsTooOld())
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
	}
	UNREACHABLE("Unsupported path finder status");
}





void cPathFinder::ResetPathFinding(cChunk &a_Chunk)
{
	m_GiveUpCounter = 40;
	m_NoPathToTarget = false;
	m_PathDestination = m_FinalDestination;
	m_DeviationOrigin = m_PathDestination;
	m_Path.reset(new cPath(a_Chunk, m_Source, m_PathDestination, 20, m_Width, m_Height));
}





bool cPathFinder::EnsureProperPoint(Vector3d & a_Vector, cChunk & a_Chunk)
{
	cChunk * Chunk = a_Chunk.GetNeighborChunk(FloorC(a_Vector.x), FloorC(a_Vector.z));
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;

	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}

	int RelX = FloorC(a_Vector.x) - Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = FloorC(a_Vector.z) - Chunk->GetPosZ() * cChunkDef::Width;

	// If destination in the air, first try to go 1 block north, or east, or west.
	// This fixes the player leaning issue.
	// If that failed, we instead go down to the lowest air block.
	int YBelowUs = FloorC(a_Vector.y) - 1;
	if (YBelowUs < 0)
	{
		return false;

	}
	Chunk->GetBlockTypeMeta(RelX, YBelowUs, RelZ, BlockType, BlockMeta);
	if (!(IsWaterOrSolid(BlockType)))
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
				Chunk = a_Chunk.GetNeighborChunk(FloorC(a_Vector.x+x), FloorC(a_Vector.z+z));
				if ((Chunk == nullptr) || !Chunk->IsValid())
				{
					return false;
				}
				RelX = FloorC(a_Vector.x+x) - Chunk->GetPosX() * cChunkDef::Width;
				RelZ = FloorC(a_Vector.z+z) - Chunk->GetPosZ() * cChunkDef::Width;
				Chunk->GetBlockTypeMeta(RelX, YBelowUs, RelZ, BlockType, BlockMeta);
				if (IsWaterOrSolid((BlockType)))
				{
					a_Vector.x += x;
					a_Vector.z += z;
					InTheAir = false;
					goto breakBothLoops;
				}
			}
		}
		breakBothLoops:

		// Go down to the lowest air block.
		if (InTheAir)
		{
			while (a_Vector.y > 0)
			{
				Chunk->GetBlockTypeMeta(RelX, FloorC(a_Vector.y) - 1, RelZ, BlockType, BlockMeta);
				if (IsWaterOrSolid(BlockType))
				{
					break;
				}
				a_Vector.y -= 1;
			}
		}
	}

	// If destination in water or solid, go up to the first air block.
	while (a_Vector.y < cChunkDef::Height)
	{
		Chunk->GetBlockTypeMeta(RelX, FloorC(a_Vector.y), RelZ, BlockType, BlockMeta);
		if (!IsWaterOrSolid(BlockType))
		{
			break;
		}
		a_Vector.y += 1;
	}

	return true;
}





bool cPathFinder::IsWaterOrSolid(BLOCKTYPE a_BlockType)
{
	return ((a_BlockType == E_BLOCK_STATIONARY_WATER) || cBlockInfo::IsSolid(a_BlockType));
}





bool cPathFinder::PathIsTooOld() const
{
	size_t acceptableDeviation = m_Path->WayPointsLeft() / 2;
	if (acceptableDeviation == 0)
	{
		acceptableDeviation = 1;
	}
	const auto DeviationSqr = (m_FinalDestination - m_DeviationOrigin).SqrLength();
	return (DeviationSqr > (acceptableDeviation * acceptableDeviation));
}
