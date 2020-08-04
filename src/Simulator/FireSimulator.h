
#pragma once

#include "Simulator.h"
#include "../IniFile.h"





/** The fire simulator takes care of the fire blocks.
It periodically increases their meta ("steps") until they "burn out"; it also supports the forever burning netherrack.
Each individual fire block gets stored in per-chunk data; that list is then used for fast retrieval.
The data value associated with each coord is used as the number of msec that the fire takes until
it progresses to the next step (blockmeta++). This value is updated if a neighbor is changed.
The simulator reads its parameters from the ini file given to the constructor.
*/
class cFireSimulator :
	public cSimulator
{
public:

	cFireSimulator(cWorld & a_World, cIniFile & a_IniFile);

	static bool IsFuel   (BLOCKTYPE a_BlockType);
	static bool DoesBurnForever(BLOCKTYPE a_BlockType);

private:

	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}  // not used
	virtual void SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override;

	static bool IsAllowedBlock(BLOCKTYPE a_BlockType);

	/** Time (in msec) that a fire block takes to burn with a fuel block into the next step */
	unsigned m_BurnStepTimeFuel;

	/** Time (in msec) that a fire block takes to burn without a fuel block into the next step */
	unsigned m_BurnStepTimeNonfuel;

	/** Chance [0..100000] of an adjacent fuel to catch fire on each tick */
	int m_Flammability;

	/** Chance [0..100000] of a fuel burning out being replaced by a new fire block instead of an air block */
	int m_ReplaceFuelChance;

	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;

	/** Returns the time [msec] after which the specified fire block is stepped again; based on surrounding fuels */
	int GetBurnStepTime(cChunk * a_Chunk, Vector3i a_RelPos);

	/** Tries to spread fire to a neighborhood of the specified block */
	void TrySpreadFire(cChunk * a_Chunk, Vector3i a_RelPos);

	/** Removes all burnable blocks neighboring the specified block */
	void RemoveFuelNeighbors(cChunk * a_Chunk, Vector3i a_RelPos);

	/** Returns true if a fire can be started in the specified block,
	that is, it is an air block and has fuel next to it.
	Note that a_NearChunk may be a chunk neighbor to the block specified!
	The coords are relative to a_NearChunk but not necessarily in it. */
	bool CanStartFireInBlock(cChunk * a_NearChunk, Vector3i a_RelPos);
} ;





/** Stores individual fire blocks in the chunk; the int data is used as the time [msec] the fire takes to step to another stage (blockmeta++) */
typedef cCoordWithIntList cFireSimulatorChunkData;




