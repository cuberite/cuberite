
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockSaplingHandler :
	public cBlockHandler
{
public:
	cBlockSaplingHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Only the first 2 bits contain the display information, the others are for growing
		a_Pickups.push_back(cItem(E_ITEM_SAPLING, 1, a_BlockMeta & 3));
	}


	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		return IsBlockTypeOfDirt(a_World->GetBlock(a_X, a_Y - 1, a_Z));
	}


	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}
	

	void OnUpdate(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_X, a_Y, a_Z);
		
		if ((Meta & 0x08) != 0)
		{
			a_World->GrowTree(a_X, a_Y, a_Z);
		}
		else
		{
			a_World->SetBlockMeta(a_X, a_Y, a_Z, Meta | 0x08);
		}
	}
	
	
	virtual bool CanBePlacedOnSide() override
	{
		return false;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




