
// cSimulatorManager.h




#pragma once




#include "cSimulator.h"





class cSimulatorManager
{
public:
	cSimulatorManager();
	~cSimulatorManager();

	void Simulate( float a_Dt );
	void WakeUp(int a_X, int a_Y, int a_Z);

	void RegisterSimulator(cSimulator * a_Simulator, short a_Rate);  // Takes ownership of the simulator object!

protected:

	typedef std::vector <std::pair<cSimulator *, short> *> cSimulators;
	
	cSimulators m_Simulators;
	long long   m_Ticks;
};




