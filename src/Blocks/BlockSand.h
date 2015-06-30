
#pragma once

#include "BlockHandler.h"





class cBlockSandHandler :
	public cBlockHandler
{
public:
	cBlockSandHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 2;
	}
};




