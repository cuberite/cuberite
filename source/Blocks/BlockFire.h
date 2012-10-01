
#pragma once

#include "BlockHandler.h"





class cBlockFireHandler :
	public cBlockHandler
{
public:
	cBlockFireHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual void OnDigging(cWorld * a_World, cPlayer * a_Player, int a_X, int a_Y, int a_Z) override
	{
		a_World->DigBlock(a_X, a_Y, a_Z);
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups from this block
	}

	virtual bool IsClickedThrough(void) override
	{
		return true;
	}
	
	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




