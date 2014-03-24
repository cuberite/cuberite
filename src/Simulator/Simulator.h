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

#include "../Vector3.h"
#include "inifile/iniFile.h"





class cWorld;
class cChunk;





class cSimulator
{
public:
	cSimulator(cWorld & a_World);
	virtual ~cSimulator();

	/// Called in each tick, a_Dt is the time passed since the last tick, in msec
	virtual void Simulate(float a_Dt) = 0;
	
	/// Called in each tick for each chunk, a_Dt is the time passed since the last tick, in msec; direct access to chunk data available
	virtual void SimulateChunk(float a_Dt, int a_ChunkX,
		int a_ChunkZ, cChunk * a_Chunk) 
	{
		UNUSED(a_Dt);
		UNUSED(a_ChunkX);
		UNUSED(a_ChunkZ);
		UNUSED(a_Chunk);
	};
	
	/// Called when a block changes
	virtual void WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk);

	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) = 0;

protected:
	friend class cChunk;  // Calls AddBlock() in its WakeUpSimulators() function, to speed things up
	
	/// Called to simulate a new block
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) = 0;

	cWorld & m_World;
} ;




