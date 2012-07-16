#pragma once
#include "Item.h"
#include "../cWorld.h"
#include "../cPlayer.h"

class cItemHoeHandler : public cItemHandler
{
public:
	cItemHoeHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		BLOCKTYPE Block = a_World->GetBlock(a_X, a_Y, a_Z);

		if(Block == E_BLOCK_DIRT || Block == E_BLOCK_GRASS)
		{
			a_World->FastSetBlock(a_X, a_Y, a_Z, E_BLOCK_FARMLAND, 0);

			a_Player->UseEquippedItem();
			return true;
		
		}
		return false;
	}
};