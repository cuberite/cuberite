#pragma once
#include "Item.h"
#include "../cWorld.h"
#include "../cPlayer.h"

#include "../blocks/Block.h"

class cItemShovelHandler : public cItemHandler
{
public:
	cItemShovelHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool OnDiggingBlock(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		BLOCKTYPE Block = a_World->GetBlock(a_X, a_Y, a_Z);
		if(Block == E_BLOCK_SNOW)
		{
			BlockHandler(Block)->DropBlock(a_World, a_X, a_Y, a_Z);

			a_World->SetBlock(a_X, a_Y, a_Z, E_BLOCK_AIR, 0);
			a_Player->UseEquippedItem();
			return true;
		}
		return false;
	}
	
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockID) override
	{
		return a_BlockID == E_BLOCK_SNOW;
	}
};