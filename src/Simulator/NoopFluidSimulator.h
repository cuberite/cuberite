
// NoopFluidSimulator.h

// Declares the cNoopFluidSimulator class representing a fluid simulator that performs nothing, it ignores all blocks





#pragma once

#include "FluidSimulator.h"





class cNoopFluidSimulator:
	public cFluidSimulator
{
	using Super = cFluidSimulator;

public:

	cNoopFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid):
		Super(a_World, a_Fluid, a_StationaryFluid)
	{
	}

	// cSimulator overrides:
	virtual void AddBlock(Vector3i a_Block, cChunk * a_Chunk) override
	{
		UNUSED(a_Block);
		UNUSED(a_Chunk);
	}
	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}
} ;




