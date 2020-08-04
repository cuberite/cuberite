
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

	/** Called in each tick, a_Dt is the time passed since the last tick, in msec. */
	void Simulate(float a_Dt);

	/** Called in each tick for each chunk, a_Dt is the time passed since the last tick, in msec; direct access to chunk data available. */
	void SimulateChunk(std::chrono::milliseconds a_DT, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk);

	/* Called when a single block changes, wakes all simulators up for the block and its face-neighbors.
	The simulator implementation may also decide to wake blocks farther away. */
	void WakeUp(cChunk & a_Chunk, Vector3i a_Position);

	/** Does the same processing as WakeUp, but for all blocks within the specified area.
	Has better performance than calling WakeUp for each block individually, due to neighbor-checking.
	All chunks intersected by the area should be valid (outputs a warning if not).
	Note that, unlike WakeUp(), this call adds blocks not only face-neighboring, but also edge-neighboring and corner-neighboring the specified area. */
	void WakeUp(const cCuboid & a_Area);

	void RegisterSimulator(cSimulator * a_Simulator, int a_Rate);  // Takes ownership of the simulator object!

protected:

	typedef std::vector <std::pair<cSimulator *, int> > cSimulators;

	cWorld & m_World;
	cSimulators m_Simulators;
	long long   m_Ticks;
};




