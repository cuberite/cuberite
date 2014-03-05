
// FloodyFluidSimulator.h

// Interfaces to the cFloodyFluidSimulator that represents a fluid simulator that tries to flood everything :)
// http://forum.mc-server.org/showthread.php?tid=565





#pragma once

#include "DelayedFluidSimulator.h"





// fwd:
class cBlockArea;





class cFloodyFluidSimulator :
	public cDelayedFluidSimulator
{
	typedef cDelayedFluidSimulator super;
	
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

	/** Spread water to neighbors.
	 *
	 * May be overridden to provide more sophisticated algorithms.
	 */
	virtual void Spread(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_NewMeta);
} ;




