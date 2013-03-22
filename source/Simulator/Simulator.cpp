
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Simulator.h"
#include "../World.h"
#include "../Vector3i.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../Chunk.h"





cSimulator::cSimulator(cWorld & a_World)
	: m_World(a_World)
{
}





cSimulator::~cSimulator()
{
}





void cSimulator::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	AddBlock(a_BlockX,     a_BlockY,     a_BlockZ,     a_Chunk);
	AddBlock(a_BlockX - 1, a_BlockY,     a_BlockZ,     a_Chunk->GetNeighborChunk(a_BlockX - 1, a_BlockZ));
	AddBlock(a_BlockX + 1, a_BlockY,     a_BlockZ,     a_Chunk->GetNeighborChunk(a_BlockX + 1, a_BlockZ));
	AddBlock(a_BlockX,     a_BlockY,     a_BlockZ - 1, a_Chunk->GetNeighborChunk(a_BlockX,     a_BlockZ - 1));
	AddBlock(a_BlockX,     a_BlockY,     a_BlockZ + 1, a_Chunk->GetNeighborChunk(a_BlockX,     a_BlockZ + 1));
	if (a_BlockY > 0)
	{
		AddBlock(a_BlockX,   a_BlockY - 1, a_BlockZ,     a_Chunk);
	}
	if (a_BlockY < cChunkDef::Height - 1)
	{
		AddBlock(a_BlockX,   a_BlockY + 1, a_BlockZ,     a_Chunk);
	}
}




