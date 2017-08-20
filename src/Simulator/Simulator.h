
#pragma once

class cWorld;
class cChunk;
class cCuboid;





/** Base class for all block-based physics simulators (such as fluid, fire, falling blocks etc.).
Each descendant provides an implementation of what needs to be done on each world tick.
The descendant may choose to do all processing in a single call for the entire world (Simulate())
or do per-chunk calculations (SimulateChunk()).
Whenever a block is changed, the WakeUp() or WakeUpArea() functions are called to notify all simulators.
The functions add all affected blocks and all their direct neighbors using the AddBlock() function. The simulator
may update its internal state based on this call. */
class cSimulator
{
public:
	cSimulator(cWorld & a_World)
		: m_World(a_World)
	{
	}

	virtual ~cSimulator() {}

	/** Called in each tick, a_Dt is the time passed since the last tick, in msec */
	virtual void Simulate(float a_Dt) = 0;

	/** Called in each tick for each chunk, a_Dt is the time passed since the last tick, in msec; direct access to chunk data available */
	virtual void SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
	{
		UNUSED(a_Dt);
		UNUSED(a_ChunkX);
		UNUSED(a_ChunkZ);
		UNUSED(a_Chunk);
	}

	/** Called when a block changes */
	void WakeUp(Vector3i a_Block, cChunk * a_Chunk);

	/** Does the same processing as WakeUp, but for all blocks within the specified area.
	Has better performance than calling WakeUp for each block individually, due to neighbor-checking.
	All chunks intersected by the area should be valid (outputs a warning if not).
	Note that, unlike WakeUp(), this call adds blocks not only face-neighboring, but also edge-neighboring and
	corner-neighboring the specified area. So far none of the simulators care about that. */
	void WakeUpArea(const cCuboid & a_Area);

	/** Returns true if the specified block type is "interesting" for this simulator. */
	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) = 0;

protected:
	friend class cChunk;  // Calls AddBlock() in its WakeUpSimulators() function, to speed things up

	/** Called to simulate a new block */
	virtual void AddBlock(Vector3i a_Block, cChunk * a_Chunk) = 0;

	cWorld & m_World;
} ;




