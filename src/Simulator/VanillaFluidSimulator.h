
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
		BLOCKTYPE a_Fluid,
		BLOCKTYPE a_StationaryFluid,
		NIBBLETYPE a_Falloff,
		int a_TickDelay,
		int a_NumNeighborsForSource
	);

protected:
	// cFloodyFluidSimulator overrides:
	virtual void SpreadXZ(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_NewMeta) override;

	/** Recursively calculates the minimum number of blocks needed to descend a level. */
	int CalculateFlowCost(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, Direction a_Dir, unsigned a_Iteration = 0);

} ;




