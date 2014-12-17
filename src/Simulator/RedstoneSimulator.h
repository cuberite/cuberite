
#pragma once

#include "Simulator.h"


class cRedstoneSimulatorChunkData
{
public:
	virtual ~cRedstoneSimulatorChunkData() = 0;
} ;


inline cRedstoneSimulatorChunkData::~cRedstoneSimulatorChunkData() {}


class cRedstoneSimulator :
	public cSimulator
{
	typedef cSimulator super;
	
public:
	cRedstoneSimulator(cWorld & a_World) :
	super(a_World)
	{
	}
	
	virtual cRedstoneSimulatorChunkData * CreateChunkData() = 0;

} ;
