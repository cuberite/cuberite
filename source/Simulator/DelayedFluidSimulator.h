
// DelayedFluidSimulator.h

// Interfaces to the cDelayedFluidSimulator class representing a fluid simulator that has a configurable delay
// before simulating a block. Each tick it takes a consecutive delay "slot" and simulates only blocks in that slot.




#pragma once

#include "FluidSimulator.h"





class cDelayedFluidSimulator :
	public cFluidSimulator
{
	typedef cFluidSimulator super;

public:	
	cDelayedFluidSimulator(cWorld * a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, int a_TickDelay);
	virtual ~cDelayedFluidSimulator();
	
	// cSimulator overrides:
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ) override;
	virtual void Simulate(float a_Dt) override;
	
protected:
	typedef std::vector<Vector3i> CoordsArray;
	
	int           m_TickDelay;       // Count of the m_Slots array
	CoordsArray * m_Slots;           // Slots, one for each delay tick
	int           m_CurrentSlotNum;  // Index into m_Slots[] where to insert new blocks
	
	/// Called from Simulate() to simulate each block in one slot of blocks. Descendants override this method to provide custom simulation.
	virtual void SimulateBlock(int a_BlockX, int a_BlockY, int a_BlockZ) = 0;
} ;




