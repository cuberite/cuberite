
#pragma once

#include "BlockHandler.h"





class cBlockMelonHandler :
	public cBlockHandler
{
public:
	cBlockMelonHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		MTRand r1;
		a_Pickups.push_back(cItem(E_ITEM_MELON_SLICE, (char)(3 + r1.randInt(4)), 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




