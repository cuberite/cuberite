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

#pragma once

#include "RedstoneSimulator.h"





class cRedstoneNoopSimulator :
	public cRedstoneSimulator
{
	typedef cRedstoneSimulator super;
public:

	cRedstoneNoopSimulator(cWorld & a_World) :
		super(a_World)
	{
	}

	//~cRedstoneNoopSimulator();

	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}  // not used
	virtual void SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override 
	{
		UNUSED(a_Dt);
		UNUSED(a_ChunkX);
		UNUSED(a_ChunkZ);
		UNUSED(a_Chunk);
	}
	virtual bool IsAllowedBlock( BLOCKTYPE a_BlockType ) override { return false; }
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override
	{
		UNUSED(a_BlockX);
		UNUSED(a_BlockY);
		UNUSED(a_BlockZ);
		UNUSED(a_Chunk);
	}

} ;
