
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
	inline static bool IsAlwaysTicked(BlockType a_Block);

	/** Returns if a block is any sort of redstone device */
	inline static bool IsRedstone(BlockType a_Block);

	void ProcessWorkItem(cChunk & Chunk, cChunk & TickingSource, const Vector3i Position);

	virtual void SimulateChunk(std::chrono::milliseconds Dt, int ChunkX, int ChunkZ, cChunk * Chunk) override;
	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block) override;
	virtual cRedstoneSimulatorChunkData * CreateChunkData() override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block) override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BlockState a_Block) override;
} ;
