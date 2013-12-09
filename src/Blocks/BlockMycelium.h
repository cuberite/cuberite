
#pragma once

#include "BlockHandler.h"





class cBlockMyceliumHandler :
	public cBlockHandler
{
public:
	cBlockMyceliumHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_DIRT, 1, 0));
	}
	
	virtual const char * GetStepSound(void) override
	{
		return "step.gravel";
	}
} ;




