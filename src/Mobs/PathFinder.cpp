#include "Globals.h"
#include "PathFinder.h"
#include "../Chunk.h"
#include "Path.h"

cPathFinder::cPathFinder(
	double a_BoundingBoxWidth, double a_BoundingBoxHeight)
{
	m_BoundingBoxWidth = CeilC(a_BoundingBoxWidth);
	m_BoundingBoxHeight = CeilC(a_BoundingBoxHeight);
}





Vector3d cPathFinder::GetWayPoint(cChunk & a_Chunk, const Vector3d & a_CurrentLocation, Vector3d & a_Destination)
{
	// This is a brute-force way and is not fit for production. It calculates a new path every tick.
	// It's for development only, a quick dirty way for having a usable interface that can be used to refactor Monster.cpp.
	m_Path = cpp14::make_unique<cPath>(a_Chunk, a_CurrentLocation, a_Destination, 20, m_BoundingBoxWidth, m_BoundingBoxHeight);
	if (!EnsureProperDestination(a_Chunk, a_Destination))
	{
		return a_CurrentLocation;
	}
	ePathFinderStatus Status;
	for (;;)
	{
		Status = m_Path->Step(a_Chunk);
		switch (Status)
		{
			case ePathFinderStatus::PATH_FOUND:
			{
				m_Path->GetNextPoint();  // Discard first point.
				return m_Path->GetNextPoint();
			}
			case ePathFinderStatus::NEARBY_FOUND:
			{
				a_Destination = m_Path->AcceptNearbyPath();
				m_Path->GetNextPoint();  // Discard first point.
				return m_Path->GetNextPoint();
			}
			case ePathFinderStatus::CALCULATING:
			{
				break;
			}
			default:
			{
				return a_CurrentLocation.Floor();
			}
		}
	}
}





bool cPathFinder::EnsureProperDestination(cChunk & a_Chunk, Vector3d & a_Destination)
{
	cChunk * Chunk = a_Chunk.GetNeighborChunk(FloorC(a_Destination.x), FloorC(a_Destination.z));
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;

	if ((Chunk == nullptr) || !Chunk->IsValid())
	{
		return false;
	}

	int RelX = FloorC(a_Destination.x) - Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = FloorC(a_Destination.z) - Chunk->GetPosZ() * cChunkDef::Width;

	// If destination in the air, first try to go 1 block north, or east, or west.
	// This fixes the player leaning issue.
	// If that failed, we instead go down to the lowest air block.
	Chunk->GetBlockTypeMeta(RelX, FloorC(a_Destination.y) - 1, RelZ, BlockType, BlockMeta);
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
				Chunk = a_Chunk.GetNeighborChunk(FloorC(a_Destination.x+x), FloorC(a_Destination.z+z));
				if ((Chunk == nullptr) || !Chunk->IsValid())
				{
					return false;
				}
				RelX = FloorC(a_Destination.x+x) - Chunk->GetPosX() * cChunkDef::Width;
				RelZ = FloorC(a_Destination.z+z) - Chunk->GetPosZ() * cChunkDef::Width;
				Chunk->GetBlockTypeMeta(RelX, FloorC(a_Destination.y) - 1, RelZ, BlockType, BlockMeta);
				if (cBlockInfo::IsSolid(BlockType))
				{
					a_Destination.x += x;
					a_Destination.z += z;
					InTheAir = false;
					goto breakBothLoops;
				}
			}
		}
		breakBothLoops:

		// Go down to the lowest air block.
		if (InTheAir)
		{
			while (a_Destination.y > 0)
			{
				Chunk->GetBlockTypeMeta(RelX, FloorC(a_Destination.y) - 1, RelZ, BlockType, BlockMeta);
				if (cBlockInfo::IsSolid(BlockType))
				{
					break;
				}
				a_Destination.y -= 1;
			}
		}
	}

	// If destination in water, go up to the highest water block.
	// If destination in solid, go up to first air block.
	bool InWater = false;
	while (a_Destination.y < cChunkDef::Height)
	{
		Chunk->GetBlockTypeMeta(RelX, FloorC(a_Destination.y), RelZ, BlockType, BlockMeta);
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
		a_Destination.y += 1;
	}
	if (InWater)
	{
		a_Destination.y -= 1;
	}


	return true;
}
