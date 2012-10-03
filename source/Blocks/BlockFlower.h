
#pragma once

#include "BlockHandler.h"





class cBlockFlowerHandler :
	public cBlockHandler
{
public:
	cBlockFlowerHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		return IsBlockTypeOfDirt(a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ));
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




