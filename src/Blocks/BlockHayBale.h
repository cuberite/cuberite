
#pragma once

#include "BlockSideways.h"





class cBlockHayBaleHandler :
	public cBlockSidewaysHandler
{
public:
	cBlockHayBaleHandler(BLOCKTYPE a_BlockType)
		: cBlockSidewaysHandler(a_BlockType)
	{
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




