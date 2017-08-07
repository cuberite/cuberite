
// VaporizeFluidSimulator.h

// Declares the cVaporizeFluidSimulator class representing a fluid simulator that replaces all fluid blocks with air
// Useful for water simulation in the Nether





#pragma once

#include "FluidSimulator.h"





class cVaporizeFluidSimulator :
	public cFluidSimulator
{
	typedef cFluidSimulator super;

public:
	cVaporizeFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid);

	// cSimulator overrides:
	virtual void AddBlock(Vector3i a_Block, cChunk * a_Chunk) override;
	virtual void Simulate(float a_Dt) override;
} ;




