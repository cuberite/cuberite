
#pragma once

#include "Simulator.h"



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

} ;
