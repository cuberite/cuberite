
// DelayedFluidSimulator.cpp

// Interfaces to the cDelayedFluidSimulator class representing a fluid simulator that has a configurable delay
// before simulating a block. Each tick it takes a consecutive delay "slot" and simulates only blocks in that slot.

#include "Globals.h"

#include "DelayedFluidSimulator.h"
#include "../World.h"





cDelayedFluidSimulator::cDelayedFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, int a_TickDelay) :
	super(a_World, a_Fluid, a_StationaryFluid),
	m_TickDelay(a_TickDelay),
	m_Slots(NULL),
	m_CurrentSlotNum(0)
{
	m_Slots = new CoordsArray[a_TickDelay];
}





cDelayedFluidSimulator::~cDelayedFluidSimulator()
{
	delete[] m_Slots;
	m_Slots = NULL;
}





void cDelayedFluidSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if ((a_BlockY < 0) || (a_BlockY >= cChunkDef::Height))
	{
		// Not inside the world (may happen when rclk with a full bucket - the client sends Y = -1)
		return;
	}
	
	// TODO: This can be optimized:
	BLOCKTYPE BlockType = m_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	if (BlockType != m_FluidBlock)
	{
		return;
	}

	CoordsArray & Blocks = m_Slots[m_CurrentSlotNum];
		
	// Check for duplicates:
	for (CoordsArray::iterator itr = Blocks.begin(), end = Blocks.end(); itr != end; ++itr)
	{
		if ((itr->x == a_BlockX) && (itr->y == a_BlockY) && (itr->z == a_BlockZ))
		{
			return;
		}
	}

	Blocks.push_back(Vector3i(a_BlockX, a_BlockY, a_BlockZ));
}





void cDelayedFluidSimulator::Simulate(float a_Dt)
{
	int SlotNum = m_CurrentSlotNum + 1;
	if (SlotNum >= m_TickDelay)
	{
		SlotNum = 0;
	}
	
	CoordsArray & Blocks = m_Slots[SlotNum];
	
	// Simulate the blocks in the scheduled slot:
	for (CoordsArray::iterator itr = Blocks.begin(), end = Blocks.end(); itr != end; ++itr)
	{
		SimulateBlock(itr->x, itr->y, itr->z);
	}
	Blocks.clear();
	
	m_CurrentSlotNum = SlotNum;
}




