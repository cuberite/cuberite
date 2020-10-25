
// VaporizeFluidSimulator.h

// Declares the cVaporizeFluidSimulator class representing a fluid simulator that replaces all fluid blocks with air
// Useful for water simulation in the Nether





#pragma once

#include "FluidSimulator.h"





class cVaporizeFluidSimulator:
	public cFluidSimulator
{
	using Super = cFluidSimulator;

public:

	using Super::cFluidSimulator;

private:

	virtual void Simulate(float a_Dt) override;
	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;
	virtual cFluidSimulatorData * CreateChunkData(void) override { return nullptr; }
} ;




