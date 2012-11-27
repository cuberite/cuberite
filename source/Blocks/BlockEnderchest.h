
#pragma once

#include "BlockHandler.h"





class cBlockEnderchestHandler :
	public cBlockHandler
{
public:
	cBlockEnderchestHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		//todo: Drop Ender Chest if using silk touch pickaxe
		a_Pickups.push_back(cItem(E_BLOCK_OBSIDIAN, 8, 0));
	}
} ;




