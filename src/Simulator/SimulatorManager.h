
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

	void SimulateChunk(std::chrono::milliseconds a_DT, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk);

	/* Called when a single block changes, wakes all simulators up for the block and its face-neighbors. */
	void WakeUp(Vector3i a_Block, cChunk * a_Chunk);

	/** Does the same processing as WakeUp, but for all blocks within the specified area.
	Has better performance than calling WakeUp for each block individually, due to neighbor-checking.
	All chunks intersected by the area should be valid (outputs a warning if not).
	Note that, unlike WakeUp(), this call adds blocks not only face-neighboring, but also edge-neighboring and
	corner-neighboring the specified area. So far none of the simulators care about that. */
	void WakeUpArea(const cCuboid & a_Area);

	void RegisterSimulator(cSimulator * a_Simulator, int a_Rate);  // Takes ownership of the simulator object!

protected:
	typedef std::vector <std::pair<cSimulator *, int> > cSimulators;

	cWorld & m_World;
	cSimulators m_Simulators;
	long long   m_Ticks;
};




