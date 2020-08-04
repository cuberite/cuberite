
// NoopFluidSimulator.h

// Declares the cNoopFluidSimulator class representing a fluid simulator that performs nothing, it ignores all blocks





#pragma once

#include "FluidSimulator.h"





class cNoopFluidSimulator:
	public cFluidSimulator
{
	using Super = cFluidSimulator;

public:

	using Super::cFluidSimulator;

private:

	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}
	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override
	{
		UNUSED(a_Block);
		UNUSED(a_Chunk);
	}
	virtual cFluidSimulatorData * CreateChunkData(void) override { return nullptr; }
} ;




