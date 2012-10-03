
#pragma once

#include "BlockHandler.h"





class cBlockFireHandler :
	public cBlockHandler
{
public:
	cBlockFireHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void OnDigging(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		a_World->DigBlock(a_BlockX, a_BlockY, a_BlockZ);
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




