
#pragma once

#include "BlockHandler.h"





class cBlockMyceliumHandler:
	public cBlockHandler
{
public:
	cBlockMyceliumHandler(BLOCKTYPE a_BlockType):
		cBlockHandler(a_BlockType)
	{
	}

	// TODO: Add Mycel Spread





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_BLOCK_DIRT, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 24;
	}
} ;




