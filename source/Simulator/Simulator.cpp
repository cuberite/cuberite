
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Simulator.h"
#include "../World.h"
#include "../Vector3i.h"
#include "../BlockID.h"
#include "../Defines.h"





cSimulator::cSimulator( cWorld* a_World )
	: m_World(a_World)
{
}





cSimulator::~cSimulator()
{
}





void cSimulator::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	AddBlock(a_BlockX,     a_BlockY,     a_BlockZ);
	AddBlock(a_BlockX - 1, a_BlockY,     a_BlockZ);
	AddBlock(a_BlockX + 1, a_BlockY,     a_BlockZ);
	AddBlock(a_BlockX,     a_BlockY - 1, a_BlockZ);
	AddBlock(a_BlockX,     a_BlockY + 1, a_BlockZ);
	AddBlock(a_BlockX,     a_BlockY,     a_BlockZ - 1);
	AddBlock(a_BlockX,     a_BlockY,     a_BlockZ + 1);
}




