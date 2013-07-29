
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemSlabHandler : public cItemHandler
{
public:
	cItemSlabHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		BLOCKTYPE Block;
		NIBBLETYPE Meta;
		a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, Block, Meta);

		if (
			((a_Dir == 0) || (a_Dir == 1))  // Only when clicking on top or on bottom of the block
			&& ((Block == E_BLOCK_WOODEN_SLAB) || (Block == E_BLOCK_STONE_SLAB))  // It is a slab
			&& (Block == a_Item.m_ItemType)  // Same slab
			&& ((Meta & 0x7) == (a_Item.m_ItemDamage & 0x7)))  // Same Texture
		{
			if (a_Player->GetGameMode() == eGameMode_Creative)
			{
				a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, Block - 1, Meta); // Block - 1 simple hack to save one if statement
				return true;
			}
			else
			{
				if (a_Player->GetInventory().RemoveOneEquippedItem())
				{
					a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, Block - 1, Meta); // Block - 1 simple hack to save one if statement
					return true;
				}
			}
		}
		return false;
	}
} ;




