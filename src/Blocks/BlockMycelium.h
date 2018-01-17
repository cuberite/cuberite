
#pragma once

#include "BlockHandler.h"





class cBlockMyceliumHandler :
	public cBlockHandler
{
public:
	cBlockMyceliumHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	// TODO: Add Mycel Spread

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_DIRT, 1, 0));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 24;
	}
} ;




