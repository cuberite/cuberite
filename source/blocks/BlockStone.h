#pragma once
#include "Block.h"
#include "../MersenneTwister.h"
#include "../cWorld.h"

class cBlockStoneHandler : public cBlockHandler
{
public:
	cBlockStoneHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual int GetDropID()
	{
		return E_ITEM_COBBLESTONE;
	}	
};