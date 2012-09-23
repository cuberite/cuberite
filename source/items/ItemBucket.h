
#pragma once

#include "ItemHandler.h"
#include "../World.h"

class cItemBucketHandler : public cItemHandler
{
public:
	cItemBucketHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		switch(m_ItemID)
		{
			case E_ITEM_BUCKET:
			{
				if (a_Dir >= 0)
				{
					AddDirection(a_X, a_Y, a_Z, a_Dir);
				}
				BLOCKTYPE ClickedBlock = a_World->GetBlock(a_X, a_Y, a_Z);
				LOG("Bucket Clicked BlockID: %d", ClickedBlock);
				ENUM_ITEM_ID NewItem = E_ITEM_EMPTY;
				switch (ClickedBlock)
				{
					case E_BLOCK_WATER:
					case E_BLOCK_STATIONARY_WATER:
					{
						NewItem = E_ITEM_WATER_BUCKET;
						break;
					}
					case E_BLOCK_LAVA:
					case E_BLOCK_STATIONARY_LAVA:
					{
						NewItem = E_ITEM_LAVA_BUCKET;
						break;
					}
				}
				cItem Item(a_Item->m_ItemID, 1);
				if (
					(NewItem != E_ITEM_EMPTY) &&
					(
						((a_Player->GetGameMode() == 1) || 
						a_Player->GetInventory().RemoveItem(Item))
					)
				)
				{
					// Give New Bucket
					cItem Item(NewItem, 1);
					a_Player->GetInventory().AddItem(Item);
					// Remove water / lava block
					a_Player->GetWorld()->SetBlock(a_X, a_Y, a_Z, E_BLOCK_AIR, 0);
					return true;
				}
				break;
			}
			
			case E_ITEM_WATER_BUCKET:
			case E_ITEM_LAVA_BUCKET:
			{
				BLOCKTYPE NewBlock = (m_ItemID == E_ITEM_LAVA_BUCKET) ? E_BLOCK_LAVA : E_BLOCK_WATER;
				if (a_Dir >= 0)
				{
					AddDirection(a_X, a_Y, a_Z, a_Dir);
				}
				if(a_World->GetBlock(a_X, a_Y, a_Z) == E_BLOCK_AIR)
				{
					cItem Item(a_Item->m_ItemID, 1);
					if ((a_Player->GetGameMode() == 1) || (a_Player->GetInventory().RemoveItem(Item)))
					{
						a_World->SetBlock(a_X, a_Y, a_Z, NewBlock, 0);

						if (a_Player->GetGameMode() == 1)
						{
							break;		//No new Bucket for creative players
						}
						cItem Item(E_ITEM_BUCKET, 1);
						a_Player->GetInventory().AddItem(Item);
						return true;
					}
				}
			}
			break;
		}

		return false;
	}

};