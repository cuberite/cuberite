
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemSeedsHandler :
	public cItemHandler
{
public:
	cItemSeedsHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}
	
	virtual BLOCKTYPE GetBlockType() override
	{
		switch(m_ItemType)
		{
			case E_ITEM_SEEDS:         return E_BLOCK_CROPS;
			case E_ITEM_MELON_SEEDS:   return E_BLOCK_MELON_STEM;
			case E_ITEM_PUMPKIN_SEEDS: return E_BLOCK_PUMPKIN_STEM;
			default:                   return E_BLOCK_AIR;
		}
	}

	virtual NIBBLETYPE GetBlockMeta(short a_ItemDamage) override
	{
		return 0;	//Not grown yet
	}
	
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		if (a_Dir != BLOCK_FACE_TOP)
		{
			// Only allow planting seeds from the top side of the block
			return;
		}
		
		int X = a_BlockX;
		int Y = a_BlockY;
		int Z = a_BlockZ;

		AddDirection(X, Y, Z, a_Dir, true);

		if (a_World->GetBlock(X, Y, Z) != E_BLOCK_FARMLAND)
		{
			return;
		}
		
		return cItemHandler::PlaceBlock(a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
	}
} ;




