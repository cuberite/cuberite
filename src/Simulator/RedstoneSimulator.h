
#pragma once

#include "Simulator.h"


class cRedstoneSimulatorChunkData
{
public:
	virtual ~cRedstoneSimulatorChunkData() = 0;
} ;


inline cRedstoneSimulatorChunkData::~cRedstoneSimulatorChunkData() {}

template <class ChunkType, class WorldType>
class cRedstoneSimulator :
	public cSimulator<ChunkType, WorldType>
{
	typedef cSimulator<ChunkType, WorldType> super;
	
public:
	cRedstoneSimulator(WorldType & a_World) :
	super(a_World)
	{
	}
	
	virtual cRedstoneSimulatorChunkData * CreateChunkData() = 0;

} ;
