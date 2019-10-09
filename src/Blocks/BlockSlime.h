#pragma once

#include "BlockHandler.h"





class cBlockSlimeHandler:
	public cClearMetaOnDrop<cBlockHandler>
{
	using super = cClearMetaOnDrop<cBlockHandler>;

public:

	cBlockSlimeHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 1;
	}
};




