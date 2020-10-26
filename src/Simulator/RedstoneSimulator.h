
#pragma once

#include "Simulator.h"





class cRedstoneSimulatorChunkData
{
public:

	virtual ~cRedstoneSimulatorChunkData()
	{
	}
};





class cRedstoneSimulator:
	public cSimulator
{
	using Super = cSimulator;

public:

	using Super::cSimulator;

	virtual cRedstoneSimulatorChunkData * CreateChunkData() = 0;
};
