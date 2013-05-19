
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"

class cItemHoeHandler : public cItemHandler
{
public:
	cItemHoeHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);

		if ((Block == E_BLOCK_DIRT) || (Block == E_BLOCK_GRASS))
		{
			a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FARMLAND, 0);

			a_Player->UseEquippedItem();
			return true;
		
		}
		return false;
	}
};