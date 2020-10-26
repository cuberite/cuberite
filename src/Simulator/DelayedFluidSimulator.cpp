
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

	cCoordWithIntVector & Blocks = m_Blocks[a_RelZ];
	int Index = cChunkDef::MakeIndexNoCheck(a_RelX, a_RelY, a_RelZ);
	for (cCoordWithIntVector::const_iterator itr = Blocks.begin(), end = Blocks.end(); itr != end; ++itr)
	{
		if (itr->Data == Index)
		{
			// Already present
			return false;
		}
	}  // for itr - Blocks[]
	Blocks.push_back(cCoordWithInt(a_RelX, a_RelY, a_RelZ, Index));
	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulatorChunkData:

cDelayedFluidSimulatorChunkData::cDelayedFluidSimulatorChunkData(int a_TickDelay) :
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

cDelayedFluidSimulator::cDelayedFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, int a_TickDelay) :
	Super(a_World, a_Fluid, a_StationaryFluid),
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
	auto ChunkDataRaw = (m_FluidBlock == E_BLOCK_WATER) ? a_Chunk->GetWaterSimulatorData() : a_Chunk->GetLavaSimulatorData();
	cDelayedFluidSimulatorChunkData * ChunkData = static_cast<cDelayedFluidSimulatorChunkData *>(ChunkDataRaw);
	cDelayedFluidSimulatorChunkData::cSlot & Slot = ChunkData->m_Slots[m_SimSlotNum];

	// Simulate all the blocks in the scheduled slot:
	for (size_t i = 0; i < ARRAYCOUNT(Slot.m_Blocks); i++)
	{
		cCoordWithIntVector & Blocks = Slot.m_Blocks[i];
		if (Blocks.empty())
		{
			continue;
		}
		for (cCoordWithIntVector::iterator itr = Blocks.begin(), end = Blocks.end(); itr != end; ++itr)
		{
			SimulateBlock(a_Chunk, itr->x, itr->y, itr->z);
		}
		m_TotalBlocks -= static_cast<int>(Blocks.size());
		Blocks.clear();
	}
}





void cDelayedFluidSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	if ((a_Block != m_FluidBlock) && (a_Block != m_StationaryFluidBlock))
	{
		return;
	}

	auto ChunkDataRaw = (m_FluidBlock == E_BLOCK_WATER) ? a_Chunk.GetWaterSimulatorData() : a_Chunk.GetLavaSimulatorData();
	cDelayedFluidSimulatorChunkData * ChunkData = static_cast<cDelayedFluidSimulatorChunkData *>(ChunkDataRaw);
	cDelayedFluidSimulatorChunkData::cSlot & Slot = ChunkData->m_Slots[m_AddSlotNum];

	// Add, if not already present:
	if (!Slot.Add(a_Position.x, a_Position.y, a_Position.z))
	{
		return;
	}

	++m_TotalBlocks;
}





void cDelayedFluidSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	if (!cChunkDef::IsValidHeight(a_Position.y))
	{
		// Not inside the world (may happen when rclk with a full bucket - the client sends Y = -1)
		return;
	}

	AddBlock(a_Chunk, a_Position, a_Block);
}
