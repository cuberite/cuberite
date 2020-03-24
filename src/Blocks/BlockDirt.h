
#pragma once

#include "BlockHandler.h"





class cBlockDirtHandler :
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockDirtHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		if (a_BlockMeta == E_META_DIRT_COARSE)
		{
			// Drop the coarse block (dirt, meta 1)
			return cItem(E_BLOCK_DIRT, 1, E_META_DIRT_COARSE);
		}
		else
		{
			return cItem(E_BLOCK_DIRT, 1, E_META_DIRT_NORMAL);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 10;
	}
} ;




