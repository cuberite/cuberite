
// VaporizeFluidSimulator.cpp

// Implements the cVaporizeFluidSimulator class representing a fluid simulator that replaces all fluid blocks with air

#include "Globals.h"
#include "VaporizeFluidSimulator.h"
#include "../OpaqueWorld.h"
#include "../Chunk.h"
#include "../Blocks/BroadcastInterface.h"





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





void cVaporizeFluidSimulator::Simulate(float a_Dt)
{
	// Nothing needed
}




