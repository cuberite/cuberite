
// DelayedFluidSimulator.cpp

// Interfaces to the cDelayedFluidSimulator class representing a fluid simulator that has a configurable delay
// before simulating a block. Each tick it takes a consecutive delay "slot" and simulates only blocks in that slot.

#include "Globals.h"

#include "DelayedFluidSimulator.h"
#include "../World.h"
#include "../Chunk.h"
#include "Blocks/BlockFluid.h"





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

cDelayedFluidSimulatorChunkData::cDelayedFluidSimulatorChunkData(size_t a_TickDelay) :
	m_Slots(new cSlot[ToUnsigned(a_TickDelay)])
{
}





cDelayedFluidSimulatorChunkData::~cDelayedFluidSimulatorChunkData()
{
	delete[] m_Slots;
	m_Slots = nullptr;
}





////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulator:

cDelayedFluidSimulator::cDelayedFluidSimulator(cWorld & a_World, BlockType a_Fluid, unsigned char a_StationaryFlowValue, size_t a_TickDelay) :
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
	for (size_t i = 0; i < ARRAYCOUNT(Slot.m_Blocks); i++)
	{
		auto & Blocks = Slot.m_Blocks[i];
		if (Blocks.empty())
		{
			continue;
		}
		for (const auto & Block : Blocks)
		{
			SimulateBlock(a_Chunk, {Block.x, Block.y, Block.z});
		}
		m_TotalBlocks -= static_cast<int>(Blocks.size());
		Blocks.clear();
	}
}





void cDelayedFluidSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
{
	if (a_Block.Type() != m_FluidBlock)
	{
		return;
	}

	auto ChunkDataRaw = (m_FluidBlock == BlockType::Water) ? a_Chunk.GetWaterSimulatorData() : a_Chunk.GetLavaSimulatorData();
	cDelayedFluidSimulatorChunkData * ChunkData = static_cast<cDelayedFluidSimulatorChunkData *>(ChunkDataRaw);
	cDelayedFluidSimulatorChunkData::cSlot & Slot = ChunkData->m_Slots[m_AddSlotNum];

	// Add, if not already present:
	if (!Slot.Add(a_Position.x, a_Position.y, a_Position.z))
	{
		return;
	}

	++m_TotalBlocks;
}
