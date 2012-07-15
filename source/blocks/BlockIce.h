#pragma once
#include "Block.h"
#include "../MersenneTwister.h"
#include "../cWorld.h"

class cBlockIceHandler : public cBlockHandler
{
public:
	cBlockIceHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual int GetDropID()
	{
		return E_ITEM_EMPTY;
	}
	
	virtual void OnDestroyed(cWorld *a_World, int a_X, int a_Y, int a_Z)
	{
		a_World->FastSetBlock(a_X, a_Y, a_Z, E_BLOCK_STATIONARY_WATER, 8);
		//This is called later than the real destroying of this ice block
	}	
	
	
};