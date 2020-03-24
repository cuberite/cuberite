
#pragma once

#include "BlockEntity.h"





class cBlockFlowerPotHandler :
	public cBlockEntityHandler
{
	using super = cBlockEntityHandler;

public:

	cBlockFlowerPotHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_ITEM_FLOWER_POT, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




