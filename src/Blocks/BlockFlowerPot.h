
#pragma once

#include "BlockHandler.h"
#include "BlockEntity.h"





class cBlockFlowerPotHandler :
	public cBlockEntityHandler
{
public:
	cBlockFlowerPotHandler(BLOCKTYPE a_BlockType) :
		cBlockEntityHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_FLOWER_POT, 1, 0));
	}
} ;




