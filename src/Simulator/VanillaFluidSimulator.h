
// VanillaFluidSimulator.h





#pragma once

#include "FloodyFluidSimulator.h"





class cVanillaFluidSimulator:
	public cFloodyFluidSimulator
{
	using Super = cFloodyFluidSimulator;

public:

	cVanillaFluidSimulator(
		cWorld & a_World,
		BlockType a_Fluid,
		unsigned char a_StationaryFlowValue,
		unsigned char a_Falloff,
		size_t a_TickDelay,
		int a_NumNeighborsForSource
	);

protected:
	// cFloodyFluidSimulator overrides:
	virtual void SpreadXZ(cChunk * a_Chunk, Vector3i a_RelPos, unsigned char a_Falloff) override;

	/** Recursively calculates the minimum number of blocks needed to descend a level. */
	int CalculateFlowCost(cChunk * a_Chunk, Vector3i a_RelPos, FluidDirection a_Dir, unsigned a_Iteration = 0);

} ;




