
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"





class cItemShearsHandler :
	public cItemHandler
{
public:
	cItemShearsHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}
	
	
	virtual bool IsTool(void) override
	{
		return true;
	}
	
	
	virtual bool OnDiggingBlock(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		if (Block == E_BLOCK_LEAVES)
		{
			cItems Drops;
			Drops.push_back(cItem(E_BLOCK_LEAVES, 1, a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x03));
			a_World->SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);

			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			a_Player->UseEquippedItem();
			return true;
		}
		// TODO: cobweb, vines
		return false;
	}


	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_COBWEB:
			case E_BLOCK_VINES:
			case E_BLOCK_LEAVES:
			{
				return true;
			}
		}  // switch (a_BlockType)
		return false;
	}
} ;




