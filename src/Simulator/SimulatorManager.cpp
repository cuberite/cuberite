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

#include "SimulatorManager.h"
#include "../World.h"





cSimulatorManager::cSimulatorManager(cWorld & a_World) :
	m_World(a_World),
	m_Ticks(0)
{
}





cSimulatorManager::~cSimulatorManager()
{
}





void cSimulatorManager::Simulate(float a_Dt)
{
	m_Ticks++;
	for (cSimulators::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr )
	{
		if ((m_Ticks % itr->second) == 0)
		{
			itr->first->Simulate(a_Dt);
		}
	}
}





void cSimulatorManager::SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	// m_Ticks has already been increased in Simulate()
	for (cSimulators::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr )
	{
		if ((m_Ticks % itr->second) == 0)
		{
			itr->first->SimulateChunk(a_Dt, a_ChunkX, a_ChunkZ, a_Chunk);
		}
	}
}





void cSimulatorManager::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	for (cSimulators::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr )
	{
		itr->first->WakeUp(a_BlockX, a_BlockY, a_BlockZ, a_Chunk);
	}
}





void cSimulatorManager::RegisterSimulator(cSimulator * a_Simulator, int a_Rate)
{
	m_Simulators.push_back(std::make_pair(a_Simulator, a_Rate));
}




