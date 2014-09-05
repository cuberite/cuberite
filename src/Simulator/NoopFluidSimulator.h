
// NoopFluidSimulator.h

// Declares the cNoopFluidSimulator class representing a fluid simulator that performs nothing, it ignores all blocks





#pragma once

#include "FluidSimulator.h"





class cNoopFluidSimulator :
	public cFluidSimulator
{
	typedef cFluidSimulator super;
	
public:
	cNoopFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid) :
		super(a_World, a_Fluid, a_StationaryFluid)
	{
	}

	// cSimulator overrides:
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override
	{
		UNUSED(a_BlockX);
		UNUSED(a_BlockY);
		UNUSED(a_BlockZ);
		UNUSED(a_Chunk);
	}
	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}
} ;




