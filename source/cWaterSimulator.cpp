#include "cWaterSimulator.h"
#include "Defines.h"


cWaterSimulator::cWaterSimulator(cWorld *a_World)
	: cFluidSimulator(a_World)
{
	m_FluidBlock = E_BLOCK_WATER;
	m_MaxHeight = 7;
	m_FlowReduction = 1;
}


bool cWaterSimulator::IsAllowedBlock(char a_BlockID)
{
	return IsBlockWater(a_BlockID);
}

