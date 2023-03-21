
// VaporizeFluidSimulator.cpp

// Implements the cVaporizeFluidSimulator class representing a fluid simulator that replaces all fluid blocks with air

#include "Globals.h"
#include "VaporizeFluidSimulator.h"
#include "BlockType.h"
#include "../OpaqueWorld.h"
#include "../Chunk.h"
#include "../Blocks/BroadcastInterface.h"





void cVaporizeFluidSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	// Nothing needed

	UNUSED(a_Dt);
	UNUSED(a_ChunkX);
	UNUSED(a_ChunkZ);
	UNUSED(a_Chunk);
}





void cVaporizeFluidSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	if ((a_Block == m_FluidBlock) || (a_Block == m_StationaryFluidBlock))
	{
		a_Chunk.FastSetBlock(a_Position, E_BLOCK_AIR, 0);
		World::GetBroadcastInterface(m_World).BroadcastSoundEffect(
			"block.fire.extinguish",
			Vector3d(a_Position),
			1.0f,
			0.6f
		);
	}
}
