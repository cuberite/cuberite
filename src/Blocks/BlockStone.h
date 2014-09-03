
#pragma once

#include "BlockHandler.h"





class cBlockStoneHandler :
	public cBlockHandler
{
public:
	cBlockStoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_COBBLESTONE, 1, 0));
	}
} ;




