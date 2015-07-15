
#pragma once

#include "BlockHandler.h"





class cBlockBrewingStandHandler :
	public cBlockHandler
{
public:
	cBlockBrewingStandHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_BREWING_STAND, 1, 0));
	}

	virtual bool IsUseable() override
	{
		return true;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 6;
	}
} ;




