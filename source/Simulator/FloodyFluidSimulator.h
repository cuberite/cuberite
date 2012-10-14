
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
	cFloodyFluidSimulator(cWorld * a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, NIBBLETYPE a_Falloff, int a_TickDelay);
	
protected:
	NIBBLETYPE m_Falloff;
	
	// cDelayedFluidSimulator overrides:
	virtual void SimulateBlock(int a_BlockX, int a_BlockY, int a_BlockZ) override;
	
	/// Checks tributaries, if not fed, decreases the block's level and returns true
	bool CheckTributaries(int a_BlockX, int a_BlockY, int a_BlockZ, const cBlockArea & a_Area, NIBBLETYPE a_MyMeta);

	/// Spreads into the specified block, if the block there allows. a_Area is for checking.
	void SpreadToNeighbor(int a_BlockX, int a_BlockY, int a_BlockZ, const cBlockArea & a_Area, NIBBLETYPE a_NewMeta);
} ;




