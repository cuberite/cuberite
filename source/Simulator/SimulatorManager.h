
// cSimulatorManager.h




#pragma once




#include "Simulator.h"





// fwd: Chunk.h
class cChunk;





class cSimulatorManager
{
public:
	cSimulatorManager(void);
	~cSimulatorManager();

	void Simulate(float a_Dt);
	void WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk);

	void RegisterSimulator(cSimulator * a_Simulator, int a_Rate);  // Takes ownership of the simulator object!

protected:

	typedef std::vector <std::pair<cSimulator *, int> > cSimulators;
	
	cSimulators m_Simulators;
	long long   m_Ticks;
};




