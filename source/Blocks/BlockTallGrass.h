
#pragma once

#include "BlockHandler.h"





class cBlockTallGrassHandler :
	public cBlockHandler
{
public:
	cBlockTallGrassHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
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


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		return a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ) != E_BLOCK_AIR;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




