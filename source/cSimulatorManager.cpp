#include "cSimulatorManager.h"
#include <vector>

cSimulatorManager::cSimulatorManager()
{

}

cSimulatorManager::~cSimulatorManager()
{
}

void cSimulatorManager::Simulate( float a_Dt )
{
	m_Ticks++;
	for( std::vector <std::pair<cSimulator *, short> *>::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr )
	{
		
		if(m_Ticks % (*itr)->second == 0)
			(*itr)->first->Simulate(a_Dt);
	}
}

void cSimulatorManager::WakeUp(int a_X, int a_Y, int a_Z)
{
	for( std::vector <std::pair<cSimulator *, short> *>::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr )
	{
		(*itr)->first->WakeUp(a_X, a_Y, a_Z);
	}
}

void cSimulatorManager::RegisterSimulator(cSimulator *a_Simulator, short a_Rate)
{
	//TODO needs some checking
	std::pair<cSimulator *, short> *Pair = new std::pair<cSimulator *, short>(a_Simulator, a_Rate);

	m_Simulators.push_back(Pair);
}