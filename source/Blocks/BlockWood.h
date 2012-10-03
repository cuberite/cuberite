
#pragma once

#include "BlockHandler.h"





class cBlockWoodHandler : public cBlockHandler
{
public:
	cBlockWoodHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




