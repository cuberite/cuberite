
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SimulatorManager.h"
#include "../Chunk.h"
#include "../Cuboid.h"
#include "../World.h"





cSimulatorManager::cSimulatorManager(cWorld & a_World) :
	m_World(a_World),
	m_Ticks(0)
{
}





cSimulatorManager::~cSimulatorManager()
{
}





void cSimulatorManager::Simulate(float a_Dt)
{
	m_Ticks++;

	for (cSimulators::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr)
	{
		if ((m_Ticks % itr->second) == 0)
		{
			itr->first->Simulate(a_Dt);
		}
	}
}





void cSimulatorManager::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	// m_Ticks has already been increased in Simulate()

	for (cSimulators::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr)
	{
		if ((m_Ticks % itr->second) == 0)
		{
			itr->first->SimulateChunk(a_Dt, a_ChunkX, a_ChunkZ, a_Chunk);
		}
	}
}





void cSimulatorManager::WakeUp(cChunk & a_Chunk, Vector3i a_Position)
{
	ASSERT(a_Chunk.IsValid());

	for (const auto & Item : m_Simulators)
	{
		Item.first->WakeUp(a_Chunk, a_Position, a_Chunk.GetBlock(a_Position));
	}

	for (const auto & Offset : cSimulator::ThreeDimensionalNeighborCoords)
	{
		auto Relative = a_Position + Offset;

		if (!cChunkDef::IsValidHeight(Relative))
		{
			continue;
		}

		const auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(Relative);

		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			continue;
		}

		// Stored block to give to simulators for performance
		// Since they all need this we save them querying it themselves
		const auto Block = Chunk->GetBlock(Relative);

		for (const auto & Item : m_Simulators)
		{
			Item.first->WakeUp(*Chunk, Relative, Offset, Block);
		}
	}
}





void cSimulatorManager::WakeUp(const cCuboid & a_Area)
{
	cCuboid area(a_Area);
	area.Sort();
	area.Expand(1, 1, 1, 1, 1, 1);  // Expand the area to contain the neighbors, too.
	area.ClampY(0, cChunkDef::Height - 1);

	cChunkCoords ChunkStart = cChunkDef::BlockToChunk(area.p1);
	cChunkCoords ChunkEnd = cChunkDef::BlockToChunk(area.p2);

	// Add all blocks, in a per-chunk manner:
	for (int cz = ChunkStart.m_ChunkZ; cz <= ChunkEnd.m_ChunkZ; ++cz)
	{
		for (int cx = ChunkStart.m_ChunkX; cx <= ChunkEnd.m_ChunkX; ++cx)
		{
			m_World.DoWithChunk(cx, cz, [this, &area](cChunk & a_CBChunk)
			{
				int startX = std::max(area.p1.x, a_CBChunk.GetPosX() * cChunkDef::Width);
				int startZ = std::max(area.p1.z, a_CBChunk.GetPosZ() * cChunkDef::Width);
				int endX = std::min(area.p2.x, a_CBChunk.GetPosX() * cChunkDef::Width + cChunkDef::Width - 1);
				int endZ = std::min(area.p2.z, a_CBChunk.GetPosZ() * cChunkDef::Width + cChunkDef::Width - 1);

				for (const auto & Item : m_Simulators)
				{
					const auto Simulator = Item.first;

					for (int y = area.p1.y; y <= area.p2.y; ++y)
					{
						for (int z = startZ; z <= endZ; ++z)
						{
							for (int x = startX; x <= endX; ++x)
							{
								const auto Position = cChunkDef::AbsoluteToRelative({ x, y, z });
								Simulator->WakeUp(a_CBChunk, Position, a_CBChunk.GetBlock(Position));
							}  // for x
						}  // for z
					}  // for y
				}
				return true;
			});
		}  // for cx
	}  // for cz
}





void cSimulatorManager::RegisterSimulator(cSimulator * a_Simulator, int a_Rate)
{
	m_Simulators.push_back(std::make_pair(a_Simulator, a_Rate));
}
