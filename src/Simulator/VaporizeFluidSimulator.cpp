/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// VaporizeFluidSimulator.cpp

// Implements the cVaporizeFluidSimulator class representing a fluid simulator that replaces all fluid blocks with air

#include "Globals.h"
#include "VaporizeFluidSimulator.h"
#include "../Chunk.h"





cVaporizeFluidSimulator::cVaporizeFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid) :
	super(a_World, a_Fluid, a_StationaryFluid)
{
}





void cVaporizeFluidSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if (a_Chunk == NULL)
	{
		return;
	}
	int RelX = a_BlockX - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_BlockZ - a_Chunk->GetPosZ() * cChunkDef::Width;	
	BLOCKTYPE BlockType = a_Chunk->GetBlock(RelX, a_BlockY, RelZ);
	if (
		(BlockType == m_FluidBlock) ||
		(BlockType == m_StationaryFluidBlock)
	)
	{
		a_Chunk->SetBlock(RelX, a_BlockY, RelZ, E_BLOCK_AIR, 0);
		a_Chunk->BroadcastSoundEffect("random.fizz", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 1.0f, 0.6f);
	}
}





void cVaporizeFluidSimulator::Simulate(float a_Dt)
{
	// Nothing needed
}




