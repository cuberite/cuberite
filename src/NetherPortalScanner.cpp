
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NetherPortalScanner.h"
#include "BlockInfo.h"
#include "Entities/Entity.h"
#include "World.h"





const double cNetherPortalScanner::OutOffset = 0.5;
const double cNetherPortalScanner::AcrossOffset = 0.5;





cNetherPortalScanner::cNetherPortalScanner(cEntity & a_MovingEntity, cWorld & a_DestinationWorld, Vector3d a_DestPosition, int a_MaxY) :
	m_EntityID(a_MovingEntity.GetUniqueID()),
	m_SourceWorld(*a_MovingEntity.GetWorld()),
	m_World(a_DestinationWorld),
	m_FoundPortal(false),
	m_BuildPlatform(true),
	m_Dir(Direction::X),
	m_PortalLoc(a_DestPosition.Floor()),
	m_Position(a_DestPosition),
	m_MaxY(a_MaxY)
{
	int MinX = FloorC((m_Position.x - SearchRadius) / cChunkDef::Width);
	int MinZ = FloorC((m_Position.z - SearchRadius) / cChunkDef::Width);
	int MaxX = CeilC((m_Position.x + SearchRadius) / cChunkDef::Width);
	int MaxZ = CeilC((m_Position.z + SearchRadius) / cChunkDef::Width);
	for (int x = MinX; x < MaxX; x++)
	{
		for (int z = MinZ; z < MaxZ; z++)
		{
			Add(x, z);
		}
	}
	Enable(*a_DestinationWorld.GetChunkMap());
}





void cNetherPortalScanner::OnChunkAvailable(int a_ChunkX, int a_ChunkZ)
{
	cChunkDef::BlockTypes blocks;
	m_World.GetChunkBlockTypes(a_ChunkX, a_ChunkZ, blocks);

	// Iterate through all of the blocks in the chunk
	for (unsigned int i = 0; i < cChunkDef::NumBlocks; i++)
	{
		if (blocks[i] == E_BLOCK_NETHER_PORTAL)
		{
			Vector3i Coordinate = cChunkDef::IndexToCoordinate(i);
			if (Coordinate.y >= m_MaxY)
			{
				// This is above the map, don't consider it.
				continue;
			}

			Vector3d PortalLoc = Vector3d(Coordinate.x + a_ChunkX * cChunkDef::Width, Coordinate.y, Coordinate.z + a_ChunkZ * cChunkDef::Width);
			if (!m_FoundPortal)
			{
				m_FoundPortal = true;
				m_PortalLoc = PortalLoc;
			}
			else
			{
				if ((PortalLoc - m_Position).SqrLength() < (m_PortalLoc - m_Position).SqrLength())
				{
					m_FoundPortal = true;
					m_PortalLoc = PortalLoc;
				}
			}
		}
	}
}





bool cNetherPortalScanner::IsValidBuildLocation(Vector3i a_BlockPos)
{
	// Check the base
	for (int i = 0; i < SearchSolidBaseWidth; i++)
	{
		for (int j = 0; j < PortalLength; j++)
		{
			BLOCKTYPE blocktype = m_World.GetBlock(a_BlockPos.x + i, a_BlockPos.y, a_BlockPos.z + j);
			if (!cBlockInfo::IsSolid(blocktype))
			{
				return false;
			}

			// Check the airspace
			for (int k = 1; k < PortalHeight; k++)
			{
				blocktype = m_World.GetBlock(a_BlockPos.x + i, a_BlockPos.y + k, a_BlockPos.z + j);
				if (blocktype != E_BLOCK_AIR)
				{
					return false;
				}
			}
		}
	}
	return true;
}





bool cNetherPortalScanner::OnAllChunksAvailable(void)
{
	if (m_FoundPortal)
	{
		// Find the bottom of this portal
		while (m_World.GetBlock(m_PortalLoc.x, m_PortalLoc.y, m_PortalLoc.z) == E_BLOCK_NETHER_PORTAL)
		{
			m_PortalLoc.y -= 1;
		}
		m_PortalLoc.y += 1;

		// Figure out which way the portal is facing
		int BXP = m_World.GetBlock(m_PortalLoc.x + 1, m_PortalLoc.y, m_PortalLoc.z);
		int BXM = m_World.GetBlock(m_PortalLoc.x - 1, m_PortalLoc.y, m_PortalLoc.z);
		if ((BXP == E_BLOCK_NETHER_PORTAL) || (BXM == E_BLOCK_NETHER_PORTAL))
		{
			// The long axis is along X
			m_Dir = Direction::X;
		}
		else
		{
			// The long axis is along Z
			m_Dir = Direction::Y;
		}
	}
	else
	{
		// Scan the area for a suitable location
		int minx = FloorC(m_Position.x) - BuildSearchRadius;
		int minz = FloorC(m_Position.z) - BuildSearchRadius;
		int maxx = FloorC(m_Position.x) + BuildSearchRadius;
		int maxz = FloorC(m_Position.z) + BuildSearchRadius;
		int maxy = m_MaxY;
		std::vector<Vector3i> Possibilities;
		int x, y, z;
		for (y = 0; y < maxy - PortalHeight; y++)
		{
			for (x = minx; x < maxx - PortalLength; x++)
			{
				for (z = minz; z < maxz - SearchSolidBaseWidth; z++)
				{
					Vector3i Location = Vector3i(x, y, z);
					if (IsValidBuildLocation(Location))
					{
						Possibilities.push_back(Vector3i(x, y, z));
					}
				}
			}
		}

		if (Possibilities.size() > 0)
		{
			m_BuildPlatform = false;

			// Find the nearest
			double DistanceToClosest = (Possibilities[0] - m_Position).SqrLength();
			Vector3i Closest = Possibilities[0];
			for (const auto & itr : Possibilities)
			{
				double Distance = (itr - m_Position).SqrLength();
				if (Distance < DistanceToClosest)
				{
					DistanceToClosest = Distance;
					Closest = itr;
				}
			}

			m_PortalLoc = Closest;
		}
	}
	return true;
}





void cNetherPortalScanner::BuildNetherPortal(Vector3i a_Location, Direction a_Direction, bool a_IncludePlatform)
{
	int x = a_Location.x;
	int y = a_Location.y;
	int z = a_Location.z;

	// Clear a 3x4x4 area starting right above the base
	for (int i = 0; i < SearchSolidBaseWidth; i++)
	{
		for (int j = 0; j < PortalLength; j++)
		{
			for (int k = 1; k < PortalHeight; k++)
			{
				if (a_Direction == Direction::Y)
				{
					m_World.SetBlock(x + i, y + k, z + j, E_BLOCK_AIR, 0);
				}
				else if (a_Direction == Direction::X)
				{
					m_World.SetBlock(x + j, y + k, z + i, E_BLOCK_AIR, 0);
				}
			}
		}
	}

	// Put in an obsidian base
	if (a_IncludePlatform)
	{
		for (int j = 0; j < PortalLength; j++)
		{
			// +2 on the short axis because that's where we deposit the entity
			if (a_Direction == Direction::Y)
			{
				m_World.SetBlock(x + 2, y, z + j, E_BLOCK_OBSIDIAN, 0);
			}
			else if (a_Direction == Direction::X)
			{
				m_World.SetBlock(x + j, y, z + 2, E_BLOCK_OBSIDIAN, 0);
			}
		}
	}

	// Build an obsidian frame
	for (int i = 0; i < PortalHeight; i++)
	{
		if (a_Direction == Direction::Y)
		{
			m_World.SetBlock(x + 1, y + i, z, E_BLOCK_OBSIDIAN, 0);
			m_World.SetBlock(x + 1, y + i, z + 3, E_BLOCK_OBSIDIAN, 0);
		}
		else if (a_Direction == Direction::X)
		{
			m_World.SetBlock(x, y + i, z + 1, E_BLOCK_OBSIDIAN, 0);
			m_World.SetBlock(x + 3, y + i, z + 1, E_BLOCK_OBSIDIAN, 0);
		}
	}
	for (int i = 0; i < PortalLength; i++)
	{
		if (a_Direction == Direction::Y)
		{
			m_World.SetBlock(x + 1, y + 4, z + i, E_BLOCK_OBSIDIAN, 0);
			m_World.SetBlock(x + 1, y, z + i, E_BLOCK_OBSIDIAN, 0);
		}
		else if (a_Direction == Direction::X)
		{
			m_World.SetBlock(x + i, y + 4, z + 1, E_BLOCK_OBSIDIAN, 0);
			m_World.SetBlock(x + i, y, z + 1, E_BLOCK_OBSIDIAN, 0);
		}
	}

	// Fill the frame (place a fire in the bottom)
	m_World.SetBlock(x + 1, y + 1, z + 1, E_BLOCK_FIRE, 0);
}





void cNetherPortalScanner::OnDisabled(void)
{
	// Now we actually move the player
	if (!m_FoundPortal)
	{
		// Build a new nether portal.
		FLOGD("Building nether portal at {0}", m_PortalLoc);
		BuildNetherPortal(m_PortalLoc, m_Dir, m_BuildPlatform);
		m_PortalLoc.x += 1;
		m_PortalLoc.y += 1;
		m_PortalLoc.z += 1;
	}

	// Put the entity near the opening
	Vector3d Position = m_PortalLoc;
	if (m_Dir == Direction::Y)
	{
		Position.x += OutOffset;
		Position.z += AcrossOffset;
	}
	else if (m_Dir == Direction::X)
	{
		Position.x += AcrossOffset;
		Position.z += OutOffset;
	}

	auto EntityID = m_EntityID;
	auto & DestinationWorld = m_World;
	auto DestinationPosition = Position;

	// Lookup our warping entity by ID
	// Necessary as they may have been destroyed in the meantime (#4582)
	// And since this is called from the destination world's thread queue a task on the source world
	m_SourceWorld.QueueTask(
		[EntityID, &DestinationWorld, DestinationPosition](cWorld & a_World)
		{
			a_World.DoWithEntityByID(
				EntityID,
				[&DestinationWorld, &DestinationPosition](cEntity & a_Entity)
				{
					FLOGD("Placing player at {0}", DestinationPosition);
					a_Entity.MoveToWorld(DestinationWorld, DestinationPosition, true, false);
					return true;
				}
			);
		}
	);

	delete this;
}

