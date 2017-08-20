
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
	m_Slots(new cSlot[a_TickDelay])
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
	super(a_World, a_Fluid, a_StationaryFluid),
	m_TickDelay(a_TickDelay),
	m_AddSlotNum(a_TickDelay - 1),
	m_SimSlotNum(0),
	m_TotalBlocks(0)
{
}





void cDelayedFluidSimulator::AddBlock(Vector3i a_Block, cChunk * a_Chunk)
{
	if ((a_Block.y < 0) || (a_Block.y >= cChunkDef::Height))
	{
		// Not inside the world (may happen when rclk with a full bucket - the client sends Y = -1)
		return;
	}

	if ((a_Chunk == nullptr) || !a_Chunk->IsValid())
	{
		return;
	}

	int RelX = a_Block.x - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_Block.z - a_Chunk->GetPosZ() * cChunkDef::Width;
	BLOCKTYPE BlockType = a_Chunk->GetBlock(RelX, a_Block.y, RelZ);
	if (BlockType != m_FluidBlock)
	{
		return;
	}

	auto ChunkDataRaw = (m_FluidBlock == E_BLOCK_WATER) ? a_Chunk->GetWaterSimulatorData() : a_Chunk->GetLavaSimulatorData();
	cDelayedFluidSimulatorChunkData * ChunkData = static_cast<cDelayedFluidSimulatorChunkData *>(ChunkDataRaw);
	cDelayedFluidSimulatorChunkData::cSlot & Slot = ChunkData->m_Slots[m_AddSlotNum];

	// Add, if not already present:
	if (!Slot.Add(RelX, a_Block.y, RelZ))
	{
		return;
	}

	++m_TotalBlocks;
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




