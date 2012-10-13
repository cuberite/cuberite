
#pragma once

#include "Simulator.h"





enum Direction
{
	X_PLUS,
	X_MINUS,
	Y_PLUS,
	Y_MINUS,
	Z_PLUS,
	Z_MINUS,
	NONE
};





class cFluidSimulator :
	public cSimulator
{
	typedef cSimulator super;
	
public:
	cFluidSimulator(cWorld * a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid);

	/// Gets the flowing direction. If a_Over is true also the block over the current block affects the direction (standard)
	virtual Direction GetFlowingDirection(int a_X, int a_Y, int a_Z, bool a_Over = true);
	
	bool IsFluidBlock          (BLOCKTYPE a_BlockType) const { return (a_BlockType == m_FluidBlock); }
	bool IsStationaryFluidBlock(BLOCKTYPE a_BlockType) const { return (a_BlockType == m_StationaryFluidBlock); }
	
	static bool CanWashAway(BLOCKTYPE a_BlockType);
	
	bool IsSolidBlock      (BLOCKTYPE a_BlockType);
	bool IsPassableForFluid(BLOCKTYPE a_BlockType);
	
protected:
	BLOCKTYPE m_FluidBlock;            // The fluid block type that needs simulating
	BLOCKTYPE m_StationaryFluidBlock;  // The fluid block type that indicates no simulation is needed
} ;





