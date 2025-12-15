
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NetherPortalScanner.h"
#include "BlockInfo.h"
#include "Entities/Entity.h"
#include "World.h"
#include "Blocks/BlockAir.h"





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
	class PortalSearchCallback : public cChunkDataCallback
	{
	public:

		PortalSearchCallback(const int a_ChunkX, const int a_ChunkZ, bool & a_FoundPortal, Vector3i & a_PortalLoc, const Vector3d a_Position, const int a_MaxY) :
			m_ChunkX(a_ChunkX),
			m_ChunkZ(a_ChunkZ),
			m_FoundPortal(a_FoundPortal),
			m_PortalLoc(a_PortalLoc),
			m_Position(a_Position),
			m_MaxY(a_MaxY)
		{
		}

	private:

		virtual void ChunkData(const ChunkBlockData & a_BlockData, const ChunkLightData &) override
		{
			for (size_t Y = 0; Y < cChunkDef::NumSections; ++Y)
			{
				const auto & Blocks = a_BlockData.GetSection(Y);
				if (Blocks == nullptr)
				{
					continue;
				}

				// Iterate through all of the blocks in the chunk:
				for (size_t i = 0; i < ChunkBlockData::SectionBlockCount; i++)
				{
					if ((*Blocks)[i].Type() != BlockType::NetherPortal)
					{
						continue;
					}

					Vector3i Coordinate = cChunkDef::IndexToCoordinate(i + Y * ChunkBlockData::SectionBlockCount);
					if (Coordinate.y >= m_MaxY)
					{
						// This is above the map, don't consider it:
						continue;
					}

					Vector3d PortalLoc = Vector3d(Coordinate.x + m_ChunkX * cChunkDef::Width, Coordinate.y, Coordinate.z + m_ChunkZ * cChunkDef::Width);
					if (!m_FoundPortal)
					{
						m_FoundPortal = true;
						m_PortalLoc = PortalLoc;
					}
					else if ((PortalLoc - m_Position).SqrLength() < (m_PortalLoc - m_Position).SqrLength())
					{
						// Found a closer portal, use that instead:
						m_PortalLoc = PortalLoc;
					}
				}
			}
		}

		int m_ChunkX, m_ChunkZ;

		bool & m_FoundPortal;
		Vector3i & m_PortalLoc;
		const Vector3d m_Position;

		const int m_MaxY;
	} Callback(a_ChunkX, a_ChunkZ, m_FoundPortal, m_PortalLoc, m_Position, m_MaxY);

	[[maybe_unused]] const bool Result = m_World.GetChunkData({ a_ChunkX, a_ChunkZ }, Callback);
	ASSERT(Result);
}





bool cNetherPortalScanner::IsValidBuildLocation(Vector3i a_BlockPos)
{
	// Check the base
	for (int i = 0; i < SearchSolidBaseWidth; i++)
	{
		for (int j = 0; j < PortalLength; j++)
		{
			auto BlockToCheck = m_World.GetBlock(a_BlockPos.addedXZ(i, j));
			if (!cBlockInfo::IsSolid(BlockToCheck))
			{
				return false;
			}

			// Check the airspace
			for (int k = 1; k < PortalHeight; k++)
			{
				BlockToCheck = m_World.GetBlock(a_BlockPos + Vector3i(i, k, j));
				if (cBlockAirHandler::IsBlockAir(BlockToCheck))
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
		while (m_World.GetBlock(m_PortalLoc).Type() == BlockType::NetherPortal)
		{
			m_PortalLoc.y -= 1;
		}
		m_PortalLoc.y += 1;

		// Figure out which way the portal is facing
		auto BlockXP = m_World.GetBlock(m_PortalLoc.addedX(1));
		auto BlockXM = m_World.GetBlock(m_PortalLoc.addedX(-1));
		if ((BlockXP.Type() == BlockType::NetherPortal) || (BlockXM.Type() == BlockType::NetherPortal))
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
						Possibilities.emplace_back(x, y, z);
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
					m_World.SetBlock({ x + i, y + k, z + j }, Block::Air::Air());
				}
				else if (a_Direction == Direction::X)
				{
					m_World.SetBlock({ x + j, y + k, z + i }, Block::Air::Air());
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
				m_World.SetBlock({ x + 2, y, z + j }, Block::Obsidian::Obsidian());
			}
			else if (a_Direction == Direction::X)
			{
				m_World.SetBlock({ x + j, y, z + 2 }, Block::Obsidian::Obsidian());
			}
		}
	}

	// Build an obsidian frame
	for (int i = 0; i < PortalHeight; i++)
	{
		if (a_Direction == Direction::Y)
		{
			m_World.SetBlock({ x + 1, y + i, z }, Block::Obsidian::Obsidian());
			m_World.SetBlock({ x + 1, y + i, z + 3 }, Block::Obsidian::Obsidian());
		}
		else if (a_Direction == Direction::X)
		{
			m_World.SetBlock({ x, y + i, z + 1 }, Block::Obsidian::Obsidian());
			m_World.SetBlock({ x + 3, y + i, z + 1 }, Block::Obsidian::Obsidian());
		}
	}
	for (int i = 0; i < PortalLength; i++)
	{
		if (a_Direction == Direction::Y)
		{
			m_World.SetBlock({ x + 1, y + 4, z + i }, Block::Obsidian::Obsidian());
			m_World.SetBlock({ x + 1, y, z + i }, Block::Obsidian::Obsidian());
		}
		else if (a_Direction == Direction::X)
		{
			m_World.SetBlock({ x + i, y + 4, z + 1 }, Block::Obsidian::Obsidian());
			m_World.SetBlock({ x + i, y, z + 1 }, Block::Obsidian::Obsidian());
		}
	}

	// Fill the frame (place a fire in the bottom)
	m_World.PlaceBlock(Vector3<int>(x + 1, y + 1, z + 1), Block::Fire::Fire());
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

