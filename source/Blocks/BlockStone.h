
#pragma once

#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"





class cBlockStoneHandler :
	public cBlockHandler
{
public:
	cBlockStoneHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_STONE, 1, 0));
	}
} ;




