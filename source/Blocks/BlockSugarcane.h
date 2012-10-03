
#pragma once

#include "BlockHandler.h"





class cBlockSugarcaneHandler :
	public cBlockHandler
{
public:
	cBlockSugarcaneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SUGARCANE, 1, 0));
	}


	virtual bool CanBeAt(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		switch (a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ))
		{
			case E_BLOCK_DIRT:
			case E_BLOCK_GRASS:
			case E_BLOCK_FARMLAND:
			case E_BLOCK_SAND:
			{
				return a_World->IsBlockDirectlyWatered(a_BlockX, a_BlockY - 1, a_BlockZ);
			}
			case E_BLOCK_SUGARCANE:
			{
				return true;
			}
		}
		return false;
	}
	
	
	void OnUpdate(cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		a_World->GrowSugarcane(a_BlockX, a_BlockY, a_BlockZ, 1);
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




