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

// cSimulatorManager.h




#pragma once




#include "Simulator.h"





// fwd: Chunk.h
class cChunk;

// fwd: World.h
class cWorld;





class cSimulatorManager
{
public:
	cSimulatorManager(cWorld & a_World);
	~cSimulatorManager();

	void Simulate(float a_Dt);
	
	void SimulateChunk(float a_DT, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk);
	
	void WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk);

	void RegisterSimulator(cSimulator * a_Simulator, int a_Rate);  // Takes ownership of the simulator object!

protected:
	typedef std::vector <std::pair<cSimulator *, int> > cSimulators;
	
	cWorld & m_World;
	cSimulators m_Simulators;
	long long   m_Ticks;
};




