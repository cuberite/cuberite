
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




