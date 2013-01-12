
#pragma once

#include "BlockHandler.h"
#include "../World.h"
#include "../Sign.h"
#include "../Player.h"





class cBlockSignHandler :
	public cBlockHandler
{
public:
	cBlockSignHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SIGN, 1, 0));
	}

	
	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




