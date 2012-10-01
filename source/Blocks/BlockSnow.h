
#pragma once

#include "BlockHandler.h"





class cBlockSnowHandler :
	public cBlockHandler
{
public:
	cBlockSnowHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	
	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SNOWBALL, 4, 0));
	}


	virtual bool CanBeAt(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		return (a_World->GetBlock(a_X, a_Y - 1, a_Z) != E_BLOCK_AIR);
	}
	
	
	virtual bool DoesDropOnUnsuitable(void) override
	{
		return false;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




