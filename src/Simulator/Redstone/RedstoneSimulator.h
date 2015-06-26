
#pragma once

#include "../Simulator.h"
#include "cRedstoneSimulatorChunkData.h"
#include <unordered_map>
#include <vector>

// for now simulator and data are not in redstone namespace due to refactoring
using namespace Redstone;

class cRedstoneSimulator :
	public cSimulator
{
	
public:
	cRedstoneSimulator(cWorld & a_World) :
		cSimulator(a_World), m_ticks(0)
	{
		
	}
	
	virtual void Simulate(float a_dt);
	virtual void cRedstoneSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk);
	virtual void WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk);

	virtual cRedstoneSimulatorChunkData * CreateChunkData()
	{
		return new cRedstoneSimulatorChunkData;
	}

	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) 
	{
		return a_BlockType == E_BLOCK_REDSTONE_TORCH_ON ||
			a_BlockType == E_BLOCK_REDSTONE_TORCH_OFF ||
			a_BlockType == E_BLOCK_IRON_BLOCK;
	}
	
protected:
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
	{
		WakeUp(a_BlockX, a_BlockY, a_BlockZ, a_Chunk);
	}

private:
	long m_ticks;
	
	// oh yea its crazy time
	cRedstoneSimulatorChunkData data;
} ;
