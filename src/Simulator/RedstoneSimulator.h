
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

	virtual cRedstoneSimulatorChunkData * CreateChunkData() = 0;

};
