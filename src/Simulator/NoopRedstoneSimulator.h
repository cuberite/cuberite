
#pragma once

#include "RedstoneSimulator.h"





class cRedstoneNoopSimulator :
	public cRedstoneSimulator
{
	typedef cRedstoneSimulator super;
public:

	cRedstoneNoopSimulator(cWorld & a_World) :
		super(a_World)
	{
	}

	// ~cRedstoneNoopSimulator();

	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}  // not used
	virtual void SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override
	{
		UNUSED(a_Dt);
		UNUSED(a_ChunkX);
		UNUSED(a_ChunkZ);
		UNUSED(a_Chunk);
	}
	virtual bool IsAllowedBlock( BLOCKTYPE a_BlockType) override { return false; }
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override
	{
		UNUSED(a_BlockX);
		UNUSED(a_BlockY);
		UNUSED(a_BlockZ);
		UNUSED(a_Chunk);
	}

} ;
