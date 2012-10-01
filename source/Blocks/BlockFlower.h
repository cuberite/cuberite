
#pragma once

#include "BlockHandler.h"





class cBlockFlowerHandler :
	public cBlockHandler
{
public:
	cBlockFlowerHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(m_BlockID, 1, 0));
	}


	virtual bool CanBeAt(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		return IsBlockTypeOfDirt(a_World->GetBlock(a_X, a_Y - 1, a_Z));
	}


	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}
	
	
	virtual bool CanBePlacedOnSide(void) override
	{
		return false;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




