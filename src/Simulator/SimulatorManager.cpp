
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SimulatorManager.h"
#include "../Chunk.h"
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

	for (const auto & Offset : cSimulator::AdjacentOffsets)
	{
		auto Relative = a_Position + Offset;
		if (!cChunkDef::IsValidHeight(Relative.y))
		{
			continue;
		}

		auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(Relative);
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
	for (const auto & Item : m_Simulators)
	{
		Item.first->WakeUp(a_Area);
	}
}





void cSimulatorManager::RegisterSimulator(cSimulator * a_Simulator, int a_Rate)
{
	m_Simulators.push_back(std::make_pair(a_Simulator, a_Rate));
}
