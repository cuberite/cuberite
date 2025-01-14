
#pragma once

#include "RedstoneSimulator.h"





class cRedstoneNoopSimulator final :
	public cRedstoneSimulator
{
	using Super = cRedstoneSimulator;

public:

	cRedstoneNoopSimulator(cWorld & a_World):
		Super(a_World)
	{
	}

	virtual void SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override
	{
		UNUSED(a_Dt);
		UNUSED(a_ChunkX);
		UNUSED(a_ChunkZ);
		UNUSED(a_Chunk);
	}

	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block) override
	{
		UNUSED(a_Block);
		UNUSED(a_Chunk);
	}

	virtual cRedstoneSimulatorChunkData * CreateChunkData() override
	{
		return nullptr;
	}
} ;
