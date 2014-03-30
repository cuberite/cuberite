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

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Simulator.h"
#include "../World.h"
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




