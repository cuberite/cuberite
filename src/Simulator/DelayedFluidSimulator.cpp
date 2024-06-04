
// DelayedFluidSimulator.cpp

// Interfaces to the cDelayedFluidSimulator class representing a fluid simulator that has a configurable delay
// before simulating a block. Each tick it takes a consecutive delay "slot" and simulates only blocks in that slot.

#include "Globals.h"

#include "DelayedFluidSimulator.h"
#include "../World.h"
#include "../Chunk.h"





////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulatorChunkData::cSlot

bool cDelayedFluidSimulatorChunkData::cSlot::Add(int a_RelX, int a_RelY, int a_RelZ)
{
	ASSERT(a_RelZ >= 0);
	ASSERT(a_RelZ < static_cast<int>(ARRAYCOUNT(m_Blocks)));

	auto & Blocks = m_Blocks[a_RelZ];
	const auto Index = cChunkDef::MakeIndex(a_RelX, a_RelY, a_RelZ);
	for (const auto & Block : Blocks)
	{
		if (Block.Data == Index)
		{
			// Already present
			return false;
		}
	}  // for itr - Blocks[]
	Blocks.emplace_back(a_RelX, a_RelY, a_RelZ, Index);
	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulatorChunkData:

cDelayedFluidSimulatorChunkData::cDelayedFluidSimulatorChunkData(int a_TickDelay) :
	m_Slots(new cSlot[ToUnsigned(a_TickDelay)])
{
}





////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulator:

cDelayedFluidSimulator::cDelayedFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, size_t a_TickDelay) :
	Super(a_World, a_Fluid, a_StationaryFluid),
	m_TickDelay(a_TickDelay),
	m_AddSlotNum(a_TickDelay - 1)
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
	auto & ChunkDataRaw = (m_FluidBlock == E_BLOCK_WATER) ? a_Chunk->GetWaterSimulatorData() : a_Chunk->GetLavaSimulatorData();
	auto ChunkData = static_cast<cDelayedFluidSimulatorChunkData *>(ChunkDataRaw.get());
	auto & Slot = ChunkData->m_Slots[m_SimSlotNum];

	// Simulate all the blocks in the scheduled slot:
	for (auto & Blocks : Slot.m_Blocks)
	{
		if (Blocks.empty())
		{
			continue;
		}
		for (const auto & Block : Blocks)
		{
			SimulateBlock(a_Chunk, Block.x, Block.y, Block.z);
		}
		m_TotalBlocks -= Blocks.size();
		Blocks.clear();
	}
}





void cDelayedFluidSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	if ((a_Block != m_FluidBlock) && (a_Block != m_StationaryFluidBlock))
	{
		return;
	}

	const auto & ChunkDataRaw = (m_FluidBlock == E_BLOCK_WATER) ? a_Chunk.GetWaterSimulatorData() : a_Chunk.GetLavaSimulatorData();
	auto ChunkData = static_cast<cDelayedFluidSimulatorChunkData *>(ChunkDataRaw.get());
	auto & Slot = ChunkData->m_Slots[m_AddSlotNum];

	// Add, if not already present:
	if (!Slot.Add(a_Position.x, a_Position.y, a_Position.z))
	{
		return;
	}

	++m_TotalBlocks;
}
