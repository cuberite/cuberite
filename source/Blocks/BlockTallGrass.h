
#pragma once

#include "BlockHandler.h"





class cBlockTallGrassHandler :
	public cBlockHandler
{
public:
	cBlockTallGrassHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	
	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Drop seeds, sometimes
		MTRand r1;
		if (r1.randInt(10) == 5)
		{
			a_Pickups.push_back(cItem(E_ITEM_SEEDS, 1, 0));
		}
	}


	virtual bool CanBeAt(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		return a_World->GetBlock(a_X, a_Y - 1, a_Z) != E_BLOCK_AIR;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




