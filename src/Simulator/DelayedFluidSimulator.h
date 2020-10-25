
// DelayedFluidSimulator.h

// Interfaces to the cDelayedFluidSimulator class representing a fluid simulator that has a configurable delay
// before simulating a block. Each tick it takes a consecutive delay "slot" and simulates only blocks in that slot.




#pragma once

#include "FluidSimulator.h"





class cDelayedFluidSimulatorChunkData :
	public cFluidSimulatorData
{
public:
	class cSlot
	{
	public:
		/** Returns true if the specified block is stored */
		bool HasBlock(int a_RelX, int a_RelY, int a_RelZ);

		/** Adds the specified block unless already present; returns true if added, false if the block was already present */
		bool Add(int a_RelX, int a_RelY, int a_RelZ);

		/** Array of block containers, each item stores blocks for one Z coord
		Int param is the block index (for faster duplicate comparison in Add())
		*/
		cCoordWithIntVector m_Blocks[16];
	} ;

	cDelayedFluidSimulatorChunkData(int a_TickDelay);
	virtual ~cDelayedFluidSimulatorChunkData();

	/** Slots, one for each delay tick, each containing the blocks to simulate */
	cSlot * m_Slots;
} ;





class cDelayedFluidSimulator:
	public cFluidSimulator
{
	using Super = cFluidSimulator;

public:

	cDelayedFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, int a_TickDelay);

protected:

	virtual void Simulate(float a_Dt) override;
	virtual void SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override;
	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;
	virtual cFluidSimulatorData * CreateChunkData(void) override { return new cDelayedFluidSimulatorChunkData(m_TickDelay); }

	int m_TickDelay;   // Count of the m_Slots array in each ChunkData
	int m_AddSlotNum;  // Index into m_Slots[] where to add new blocks in each ChunkData
	int m_SimSlotNum;  // Index into m_Slots[] where to simulate blocks in each ChunkData

	int m_TotalBlocks;  // Statistics only: the total number of blocks currently queued

	/* Slots:
	| 0 | 1 | ... | m_AddSlotNum | m_SimSlotNum | ... | m_TickDelay - 1 |
	|       adding blocks here ^ | ^ simulating here */

	/** Called from SimulateChunk() to simulate each block in one slot of blocks. Descendants override this method to provide custom simulation. */
	virtual void SimulateBlock(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ) = 0;
} ;




