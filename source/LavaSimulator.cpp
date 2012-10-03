#include "Globals.h"
#include "LavaSimulator.h"
#include "Defines.h"
#include "World.h"


cLavaSimulator::cLavaSimulator(cWorld *a_World)
	: cFluidSimulator(a_World)
{
	m_FluidBlock = E_BLOCK_LAVA;
	m_StationaryFluidBlock = E_BLOCK_STATIONARY_LAVA;
	m_MaxHeight = 6;
	m_FlowReduction = 2;
}


bool cLavaSimulator::IsAllowedBlock(BLOCKTYPE a_BlockType)
{
	return IsBlockLava(a_BlockType);
}