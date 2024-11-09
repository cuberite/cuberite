
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

	using Super::Super;

	virtual cRedstoneSimulatorChunkData * CreateChunkData() = 0;
};
