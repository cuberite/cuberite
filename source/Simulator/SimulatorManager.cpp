
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SimulatorManager.h"





cSimulatorManager::cSimulatorManager()
{

}





cSimulatorManager::~cSimulatorManager()
{
	for (cSimulators::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr )
	{
		delete *itr;
	}  // for itr - m_Simulators[]
}





void cSimulatorManager::Simulate( float a_Dt )
{
	m_Ticks++;
	for (cSimulators::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr )
	{
		if(m_Ticks % (*itr)->second == 0)
			(*itr)->first->Simulate(a_Dt);
	}
}





void cSimulatorManager::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	for (cSimulators::iterator itr = m_Simulators.begin(); itr != m_Simulators.end(); ++itr )
	{
		(*itr)->first->WakeUp(a_BlockX, a_BlockY, a_BlockZ);
	}
}





void cSimulatorManager::RegisterSimulator(cSimulator *a_Simulator, short a_Rate)
{
	//TODO needs some checking
	std::pair<cSimulator *, short> *Pair = new std::pair<cSimulator *, short>(a_Simulator, a_Rate);

	m_Simulators.push_back(Pair);
}




