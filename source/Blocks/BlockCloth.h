
#pragma once

#include "BlockHandler.h"





class cBlockClothHandler :
	public cBlockHandler
{
public:
	cBlockClothHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_WOOL, 1, a_BlockMeta));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




