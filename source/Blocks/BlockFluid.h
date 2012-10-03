
#pragma once

#include "BlockHandler.h"





class cBlockFluidHandler :
	public cBlockHandler
{
public:
	cBlockFluidHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{

	}
	
	
	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	
	// TODO: Implement proper fluid physics here
} ;




