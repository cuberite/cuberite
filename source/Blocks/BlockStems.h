
#pragma once

#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"





class cBlockStemsHandler :
	public cBlockHandler
{
public:
	cBlockStemsHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		int ItemType = (m_BlockID == E_BLOCK_MELON_STEM) ? E_ITEM_MELON_SEEDS : E_ITEM_PUMPKIN_SEEDS;
		a_Pickups.push_back(cItem(ItemType, 1, 0));
	}

	
	void OnUpdate(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		// TODO: Handle Growing here
	}


	virtual bool CanBeAt(cWorld * a_World, int a_X, int a_Y, int a_Z) override
	{
		return a_World->GetBlock(a_X, a_Y - 1, a_Z) == E_BLOCK_FARMLAND;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




