
#pragma once

#include "BlockHandler.h"





class cBlockSnowHandler :
	public cBlockHandler
{
public:
	cBlockSnowHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
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


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		BLOCKTYPE UnderlyingBlock = a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ);
		return g_BlockIsSnowable[UnderlyingBlock];
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




