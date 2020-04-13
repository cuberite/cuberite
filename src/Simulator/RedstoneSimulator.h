
#pragma once

#include "Simulator.h"





class cRedstoneSimulatorChunkData
{
public:
	virtual ~cRedstoneSimulatorChunkData() = 0;
};

inline cRedstoneSimulatorChunkData::~cRedstoneSimulatorChunkData() {}





class cRedstoneSimulator:
	public cSimulator
{
	using Super = cSimulator;

public:

	cRedstoneSimulator(cWorld & a_World):
		Super(a_World)
	{
	}

	virtual void Simulate(float a_Dt) = 0;
	virtual void SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) = 0;
	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) = 0;
	virtual void AddBlock(Vector3i a_Block, cChunk * a_Chunk) = 0;

	virtual cRedstoneSimulatorChunkData * CreateChunkData() = 0;

};
