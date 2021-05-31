
// DelayedFluidSimulator.cpp

// Interfaces to the cDelayedFluidSimulator class representing a fluid simulator that has a configurable delay
// before simulating a block. Each tick it takes a consecutive delay "slot" and simulates only blocks in that slot.

#include "Globals.h"

#include "DelayedFluidSimulator.h"
#include "../World.h"
#include "../Chunk.h"

#include "../Blocks/BlockFluid.h"





////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulatorChunkData::cSlot

bool cDelayedFluidSimulatorChunkData::cSlot::Add(Vector3i a_RelPos)
{
	if (!cChunkDef::IsValidRelPos(a_RelPos))
	{
		return false;
	}

	int Index = cChunkDef::MakeIndex(a_RelPos);
	if (m_Blocks.find(Index) == m_Blocks.end())
	{
		return false;
	}

	m_Blocks.insert(std::make_pair(Index, a_RelPos));
	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulatorChunkData:

cDelayedFluidSimulatorChunkData::cDelayedFluidSimulatorChunkData(int a_TickDelay)
{
	auto Default = cSlot();
	m_Slots = std::vector<cSlot>(a_TickDelay + 2, Default);
}





////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulator:

cDelayedFluidSimulator::cDelayedFluidSimulator(cWorld & a_World, BlockType a_Fluid, unsigned char a_StationaryFlowValue, int a_TickDelay) :
	Super(a_World, a_Fluid, a_StationaryFlowValue),
	m_TickDelay(a_TickDelay),
	m_AddSlotNum(a_TickDelay - 1),
	m_SimSlotNum(0),
	m_TotalBlocks(0)
{
}





void cDelayedFluidSimulator::Simulate(float a_Dt)
{
	m_AddSlotNum = m_SimSlotNum;
	m_SimSlotNum += 1;
	if (m_SimSlotNum >= m_TickDelay)
	{
		m_SimSlotNum = 0;
	}
}





void cDelayedFluidSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	auto ChunkDataRaw = (m_FluidBlock == BlockType::Water) ? a_Chunk->GetWaterSimulatorData() : a_Chunk->GetLavaSimulatorData();
	cDelayedFluidSimulatorChunkData * ChunkData = static_cast<cDelayedFluidSimulatorChunkData *>(ChunkDataRaw);
	cDelayedFluidSimulatorChunkData::cSlot & Slot = ChunkData->m_Slots[m_SimSlotNum];

	// Simulate all the blocks in the scheduled slot:
	for (auto & [Index, Position] : Slot.m_Blocks)
	{
		SimulateBlock(a_Chunk, Position);
	}

	m_TotalBlocks -= static_cast<int>(Slot.m_Blocks.size());
	Slot.m_Blocks.clear();
}





void cDelayedFluidSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
{
	if ((a_Block != m_FluidBlock) && cBlockFluidHandler::GetFalloff(a_Block) != m_StationaryFalloffValue)
	{
		return;
	}

	auto ChunkDataRaw = (m_FluidBlock == BlockType::Water) ? a_Chunk.GetWaterSimulatorData() : a_Chunk.GetLavaSimulatorData();
	cDelayedFluidSimulatorChunkData * ChunkData = static_cast<cDelayedFluidSimulatorChunkData *>(ChunkDataRaw);

	if (ChunkData == nullptr)
	{
		return;
	}

	while (ChunkData->m_Slots.size() <= m_AddSlotNum)
	{
		ChunkData->m_Slots.push_back(cDelayedFluidSimulatorChunkData::cSlot());
	}

	auto & Slot = ChunkData->m_Slots[m_AddSlotNum];

	// Add, if not already present:
	if (!Slot.Add(a_Position))
	{
		return;
	}

	++m_TotalBlocks;
}





void cDelayedFluidSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
{
	if (!cChunkDef::IsValidHeight(a_Position.y))
	{
		// Not inside the world (may happen when rclk with a full bucket - the client sends Y = -1)
		return;
	}

	AddBlock(a_Chunk, a_Position, a_Block);
}
