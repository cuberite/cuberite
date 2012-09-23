#pragma once
#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"

class cBlockStoneHandler : public cBlockHandler
{
public:
	cBlockStoneHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual int GetDropID() override
	{
		return E_ITEM_COBBLESTONE;
	}	
};