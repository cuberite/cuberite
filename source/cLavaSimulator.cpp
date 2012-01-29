
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cLavaSimulator.h"
#include "Defines.h"


cLavaSimulator::cLavaSimulator(cWorld *a_World)
	: cFluidSimulator(a_World)
{
	m_FluidBlock = E_BLOCK_LAVA;
	m_MaxHeight = 6;
	m_FlowReduction = 2;
}


bool cLavaSimulator::IsAllowedBlock(char a_BlockID)
{
	return IsBlockLava(a_BlockID);
}