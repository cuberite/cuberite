
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

	virtual const char * GetStepSound(void) override
	{
		return "step.sand";
	}
	
};




