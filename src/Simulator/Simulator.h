
#pragma once

#include "ChunkDef.h"

class cWorld;
class cChunk;
class cCuboid;





/** Base class for all block-based physics simulators (such as fluid, fire, falling blocks etc.).
Each descendant provides an implementation of what needs to be done on each world tick.
The descendant may choose to do all processing in a single call for the entire world (Simulate())
or do per-chunk calculations (SimulateChunk()).
Whenever a block is changed, the WakeUp() functions are called to notify all simulators by the simulator manager.
The functions are invoked to add all affected blocks and their direct neighbors using the AddBlock() function.
The simulator may update its internal state based on this call. */
class cSimulator
{
public:

	cSimulator(cWorld & a_World)
		: m_World(a_World)
	{
	}

	virtual ~cSimulator() {}

	/** Contains offsets for direct adjacents of any position. */
	static constexpr std::array<Vector3i, 6> AdjacentOffsets
	{
		{
			{  1,  0,  0 },
			{ -1,  0,  0 },
			{  0,  1,  0 },
			{  0, -1,  0 },
			{  0,  0,  1 },
			{  0,  0, -1 },
		}
	};

	/** For a given offset from a position, return the offsets that represent the adjacents of the newly offset position, excluding the old position. */
	static std::array<Vector3i, 5> GetLinkedOffsets(Vector3i Offset);

protected:

	friend class cChunk;  // Calls AddBlock() in its WakeUpSimulators() function, to speed things up
	friend class cSimulatorManager;  // Class reponsible for dispatching calls to the various slave Simulators

	virtual void Simulate(float a_Dt) = 0;
	virtual void SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
	{
		UNUSED(a_Dt);
		UNUSED(a_ChunkX);
		UNUSED(a_ChunkZ);
		UNUSED(a_Chunk);
	}

	/** Called to simulate a new block. Unlike WakeUp this function will perform minimal checking.
	It queues the block to be simulated as fast as possible, suitable for area wakeups. */
	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) = 0;

	/** Called to simulate a single new block, typically as a result of a single block break or change.
	The simulator implementation may decide to perform additional checks or maintain consistency of internal state
	before the block is added to the simulate queue. */
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block);

	/** Called to simulate a single block, synthesised by the simulator manager.
	The position represents the adjacents of the block that was actually changed, with the offset used given.
	Simulators may use this information to update additional blocks that were affected by the change, or queue
	farther, extra-adjacents blocks to be updated. The simulator manager calls this overload after the 3-argument WakeUp. */
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BLOCKTYPE a_Block);

	/** Called to simulate an area by the manager, delegated to cSimulator to avoid virtual calls in tight loops. */
	void WakeUp(const cCuboid & a_Area);

	cWorld & m_World;
} ;
