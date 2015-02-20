
#include "Globals.h"

#include "../World.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../Chunk.h"

#ifdef __clang__
	#pragma clang diagnostic ignored "-Wweak-template-vtables"
#endif  // __clang__



#include "Simulator.h"





void cSimulator::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	AddBlock(a_BlockX, a_BlockY, a_BlockZ, a_Chunk);
	AddBlock(a_BlockX - 1, a_BlockY, a_BlockZ, a_Chunk->GetNeighborChunk(a_BlockX - 1, a_BlockZ));
	AddBlock(a_BlockX + 1, a_BlockY, a_BlockZ, a_Chunk->GetNeighborChunk(a_BlockX + 1, a_BlockZ));
	AddBlock(a_BlockX, a_BlockY, a_BlockZ - 1, a_Chunk->GetNeighborChunk(a_BlockX, a_BlockZ - 1));
	AddBlock(a_BlockX, a_BlockY, a_BlockZ + 1, a_Chunk->GetNeighborChunk(a_BlockX, a_BlockZ + 1));
	if (a_BlockY > 0)
	{
		AddBlock(a_BlockX, a_BlockY - 1, a_BlockZ, a_Chunk);
	}
	if (cChunkDef::IsRelCoordNeighborLessThanChunkHeight(a_BlockY))
	{
		AddBlock(a_BlockX, a_BlockY + 1, a_BlockZ, a_Chunk);
	}
}




