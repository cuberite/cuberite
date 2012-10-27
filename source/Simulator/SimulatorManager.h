
// cSimulatorManager.h




#pragma once




#include "Simulator.h"





class cSimulatorManager
{
public:
	cSimulatorManager(void);
	~cSimulatorManager();

	void Simulate(float a_Dt);
	void WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ);

	void RegisterSimulator(cSimulator * a_Simulator, int a_Rate);  // Takes ownership of the simulator object!

protected:

	typedef std::vector <std::pair<cSimulator *, int> > cSimulators;
	
	cSimulators m_Simulators;
	long long   m_Ticks;
};




