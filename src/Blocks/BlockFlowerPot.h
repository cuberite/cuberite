
#pragma once

#include "BlockEntity.h"





class cBlockFlowerPotHandler :
	public cClearMetaOnDrop<cBlockEntityHandler>
{
	using super = cClearMetaOnDrop<cBlockEntityHandler>;

public:

	cBlockFlowerPotHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




