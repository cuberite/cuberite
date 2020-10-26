
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

	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}  // not used
	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override
	{
		UNUSED(a_Block);
		UNUSED(a_Chunk);
	}

	virtual cRedstoneSimulatorChunkData * CreateChunkData() override
	{
		return nullptr;
	}

} ;
