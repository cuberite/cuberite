
#pragma once

#include "Simulator.h"
#include "../IniFile.h"





// fwd:
class cChunk;





/** Per-chunk data for the simulator, specified individual chunks to simulate; Data is not used */
typedef cCoordWithIntList cSandSimulatorChunkData;





/** Despite the class name, this simulator takes care of all blocks that fall when suspended in the air. */
class cSandSimulator :
	public cSimulator
{
public:

	cSandSimulator(cWorld & a_World, cIniFile & a_IniFile);

	/** Returns true if a falling-able block can start falling through the specified block type */
	static bool CanStartFallingThrough(BLOCKTYPE a_BlockType);

	/** Returns true if an already-falling block can pass through the specified block type (e. g. torch) */
	static bool CanContinueFallThrough(BLOCKTYPE a_BlockType);

	/** Returns true if the falling block rematerializing will replace the specified block type (e. g. tall grass) */
	static bool IsReplacedOnRematerialization(BLOCKTYPE a_BlockType);

	/** Returns true if the specified block breaks falling blocks while they fall through it (e. g. halfslabs) */
	static bool DoesBreakFallingThrough(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	/** Called when a block finishes falling at the specified coords, either by insta-fall,
	or through cFallingBlock entity.
	It either rematerializes the block (a_FallingBlockType) at the specified coords, or creates a pickup,
	based on the block currently present in the world at the dest specified coords. */
	static void FinishFalling(
		cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,
		BLOCKTYPE a_FallingBlockType, NIBBLETYPE a_FallingBlockMeta
	);

	static bool IsAllowedBlock(BLOCKTYPE a_BlockType);

private:

	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}  // not used
	virtual void SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override;

	bool m_IsInstantFall;  // If set to true, blocks don't fall using cFallingBlock entity, but instantly instead

	int  m_TotalBlocks;    // Total number of blocks currently in the queue for simulating

	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;

	/** Performs the instant fall of the block - removes it from top, Finishes it at the bottom */
	void DoInstantFall(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ);
};









