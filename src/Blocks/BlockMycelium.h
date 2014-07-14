
#pragma once

#include "BlockHandler.h"





class cBlockMyceliumHandler :
	public cClearMetaOnDrop<cBlockHandler>
{
public:
	cBlockMyceliumHandler(BLOCKTYPE a_BlockType)
		: cClearMetaOnDrop<cBlockHandler>(a_BlockType)
	{
	}

	// TODO: Add Mycel Spread

	
	virtual const char * GetStepSound(void) override
	{
		return "step.gravel";
	}
} ;




