
#pragma once

#include "BlockHandler.h"





class cBlockFenceHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
public:

	cBlockFenceHandler(BLOCKTYPE a_BlockType) :
		super(a_BlockType)
	{
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
};




