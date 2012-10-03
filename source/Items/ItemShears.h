
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"

class cItemShearsHandler : public cItemHandler
{
public:
	cItemShearsHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}
	virtual bool IsTool() override
	{
		return true;
	}
	virtual bool OnDiggingBlock(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		BLOCKTYPE Block = a_World->GetBlock(a_X, a_Y, a_Z);
		if(Block == E_BLOCK_LEAVES)
		{
			cItems Drops;
			Drops.push_back(cItem(E_ITEM_LEAVES, 1, a_World->GetBlockMeta(a_X, a_Y, a_Z)));
			a_World->SpawnItemPickups(Drops, a_X, a_Y, a_Z);

			a_World->SetBlock(a_X, a_Y, a_Z, E_BLOCK_AIR, 0);
			a_Player->UseEquippedItem();
			return true;
		}
		return false;
	}


	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		return a_BlockType == E_BLOCK_COBWEB
			|| a_BlockType == E_BLOCK_VINES;
	}
};