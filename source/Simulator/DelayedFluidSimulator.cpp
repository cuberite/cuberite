
// DelayedFluidSimulator.cpp

// Interfaces to the cDelayedFluidSimulator class representing a fluid simulator that has a configurable delay
// before simulating a block. Each tick it takes a consecutive delay "slot" and simulates only blocks in that slot.

#include "Globals.h"

#include "DelayedFluidSimulator.h"
#include "../World.h"
#include "../Chunk.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulatorChunkData:

cDelayedFluidSimulatorChunkData::cDelayedFluidSimulatorChunkData(int a_TickDelay) :
	m_Slots(new cCoordWithIntList[a_TickDelay])
{
}





cDelayedFluidSimulatorChunkData::~cDelayedFluidSimulatorChunkData()
{
	delete[] m_Slots;
	m_Slots = NULL;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cDelayedFluidSimulator:

cDelayedFluidSimulator::cDelayedFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, int a_TickDelay) :
	super(a_World, a_Fluid, a_StationaryFluid),
	m_TickDelay(a_TickDelay),
	m_AddSlotNum(a_TickDelay - 1),
	m_SimSlotNum(0),
	m_TotalBlocks(0)
{
}





void cDelayedFluidSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if ((a_BlockY < 0) || (a_BlockY >= cChunkDef::Height))
	{
		// Not inside the world (may happen when rclk with a full bucket - the client sends Y = -1)
		return;
	}
	
	int RelX = a_BlockX - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_BlockZ - a_Chunk->GetPosZ() * cChunkDef::Width;
	BLOCKTYPE BlockType = a_Chunk->GetBlock(RelX, a_BlockY, RelZ);
	if (BlockType != m_FluidBlock)
	{
		return;
	}

	void * ChunkDataRaw = (m_FluidBlock == E_BLOCK_WATER) ? a_Chunk->GetWaterSimulatorData() : a_Chunk->GetLavaSimulatorData();
	cDelayedFluidSimulatorChunkData * ChunkData = (cDelayedFluidSimulatorChunkData *)ChunkDataRaw;
	cCoordWithIntList & Blocks = ChunkData->m_Slots[m_AddSlotNum];
		
	// Check for duplicates:
	for (cCoordWithIntList::iterator itr = Blocks.begin(), end = Blocks.end(); itr != end; ++itr)
	{
		if ((itr->x == RelX) && (itr->y == a_BlockY) && (itr->z == RelZ))
		{
			return;
		}
	}

	++m_TotalBlocks;
	Blocks.push_back(cCoordWithInt(RelX, a_BlockY, RelZ));
}





void cDelayedFluidSimulator::Simulate(float a_Dt)
{
	if (m_TotalBlocks > 0)
	{
		LOGD("DFS: %d blocks in queue", m_TotalBlocks);
	}
	m_AddSlotNum = m_SimSlotNum;
	m_SimSlotNum += 1;
	if (m_SimSlotNum >= m_TickDelay)
	{
		m_SimSlotNum = 0;
	}
}





void cDelayedFluidSimulator::SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{	
	void * ChunkDataRaw = (m_FluidBlock == E_BLOCK_WATER) ? a_Chunk->GetWaterSimulatorData() : a_Chunk->GetLavaSimulatorData();
	cDelayedFluidSimulatorChunkData * ChunkData = (cDelayedFluidSimulatorChunkData *)ChunkDataRaw;
	cCoordWithIntList & Blocks = ChunkData->m_Slots[m_SimSlotNum];
	
	// Simulate the blocks in the scheduled slot:
	for (cCoordWithIntList::iterator itr = Blocks.begin(), end = Blocks.end(); itr != end; ++itr)
	{
		SimulateBlock(a_Chunk, itr->x, itr->y, itr->z);
	}
	m_TotalBlocks -= Blocks.size();
	Blocks.clear();
}




