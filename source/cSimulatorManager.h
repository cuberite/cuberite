#pragma once
#include "cSimulator.h"
#include <vector>


class cSimulatorManager
{
public:
	cSimulatorManager();
	~cSimulatorManager();

	void Simulate( float a_Dt );
	void WakeUp(int a_X, int a_Y, int a_Z);

	void RegisterSimulator(cSimulator *a_Simulator, short a_Rate);

protected:
	std::vector <std::pair<cSimulator *, short> *> m_Simulators;
	long long m_Ticks;
};