
#pragma once

#include "BlockHandler.h"





class cBlockClothHandler :
	public cBlockHandler
{
public:
	cBlockClothHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




