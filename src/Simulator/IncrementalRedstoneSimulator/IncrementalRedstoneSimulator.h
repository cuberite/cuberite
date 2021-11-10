
#pragma once

#include "../RedstoneSimulator.h"





class cIncrementalRedstoneSimulator final :
	public cRedstoneSimulator
{
	using Super = cRedstoneSimulator;

public:

	using Super::Super;

private:

	/** Returns if a redstone device is always ticked due to influence by its environment */
	static bool IsAlwaysTicked(BLOCKTYPE a_Block);

	/** Returns if a block is any sort of redstone device */
	static bool IsRedstone(BLOCKTYPE a_Block);

	void ProcessWorkItem(cChunk & Chunk, cChunk & TickingSource, const Vector3i Position);

	virtual void Simulate(float Dt) override {}
	virtual void SimulateChunk(std::chrono::milliseconds Dt, int ChunkX, int ChunkZ, cChunk * Chunk) override;
	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;
	virtual cRedstoneSimulatorChunkData * CreateChunkData() override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BLOCKTYPE a_Block) override;
} ;
