
// FloodyFluidSimulator.h

// Interfaces to the cFloodyFluidSimulator that represents a fluid simulator that tries to flood everything :)
// https://forum.cuberite.org/thread-565.html





#pragma once

#include "DelayedFluidSimulator.h"





class cFloodyFluidSimulator:
	public cDelayedFluidSimulator
{
	using Super = cDelayedFluidSimulator;

public:

	cFloodyFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, NIBBLETYPE a_Falloff, int a_TickDelay, int a_NumNeighborsForSource);

protected:

	NIBBLETYPE m_Falloff;
	int        m_NumNeighborsForSource;

	// cDelayedFluidSimulator overrides:
	virtual void SimulateBlock(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override;

	/** Checks tributaries, if not fed, decreases the block's level and returns true. */
	bool CheckTributaries(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_MyMeta);

	/** Spreads into the specified block, if the blocktype there allows. a_Area is for checking. */
	void SpreadToNeighbor(cChunk * a_NearChunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_NewMeta);

	/** Checks if there are enough neighbors to create a source at the coords specified; turns into source and returns true if so. */
	bool CheckNeighborsForSource(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ);

	/** Checks if the specified block should harden (Water / Lava interaction) and if so, converts it to a suitable block.
	Returns whether the block was changed or not. */
	bool HardenBlock(cChunk * a_Chunk, Vector3i a_RelPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta);

	/** Spread fluid to XZ neighbors.
	The coords are of the block currently being processed; a_NewMeta is the new meta for the new fluid block.
	Descendants may overridde to provide more sophisticated algorithms. */
	virtual void SpreadXZ(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_NewMeta);
} ;




