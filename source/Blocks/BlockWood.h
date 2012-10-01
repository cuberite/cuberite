
#pragma once

#include "BlockHandler.h"





class cBlockWoodHandler : public cBlockHandler
{
public:
	cBlockWoodHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




