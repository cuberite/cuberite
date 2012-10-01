
#pragma once

#include "BlockHandler.h"





class cBlockSugarcaneHandler :
	public cBlockHandler
{
public:
	cBlockSugarcaneHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SUGARCANE, 1, 0));
	}


	virtual bool CanBeAt(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		switch (a_World->GetBlock(a_X, a_Y - 1, a_Z))
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_FARMLAND:
			case E_BLOCK_SAND:
			{
				return a_World->IsBlockDirectlyWatered(a_X, a_Y - 1, a_Z);
			}
			case E_BLOCK_SUGARCANE:
			{
				return true;
			}
		}
		return false;
	}
	
	
	void OnUpdate(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		// TODO: Handle Growing here
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




