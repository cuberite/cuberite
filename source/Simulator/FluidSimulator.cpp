
#include "Globals.h"

#include "FluidSimulator.h"





cFluidSimulator::cFluidSimulator(cWorld * a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid) :
	super(a_World),
	m_FluidBlock(a_Fluid),
	m_StationaryFluidBlock(a_StationaryFluid)
{
}




