#pragma once

#include "Item.h"
#include "../cWorld.h"

class cItemSlabHandler : public cItemHandler
{
public:
	cItemSlabHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir)
	{
		BLOCKTYPE Block;
		NIBBLETYPE Meta;
		a_World->GetBlockTypeMeta(a_X, a_Y, a_Z, Block, Meta);

		if(	(a_Dir == 0 || a_Dir == 1)											//Only when clicking on top or on bottom of the block
			&& (Block == E_BLOCK_WOODEN_SLAB || Block == E_BLOCK_STONE_SLAB)	//It is a slab
			&& (Block == a_Item->m_ItemID)										//Same slab
			&& ((Meta & 0x7) == (a_Item->m_ItemHealth & 0x7)))						//Same Texture
		{
			if(a_Player->GetInventory().RemoveItem(cItem(a_Item->m_ItemID, 1)))
			{
				a_World->SetBlock(a_X, a_Y, a_Z, Block - 1, Meta);				//Block - 1 simple hack to save one if statement
				return true;
			}
		}
		return false;
	}

	virtual char GetBlockMeta(char a_ItemMeta)
	{
		return a_ItemMeta;
	}
};