#pragma once

#include "Item.h"
#include "../cWorld.h"

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
				AddDirection(a_X, a_Y, a_Z, a_Dir);
				BLOCKTYPE ClickedBlock = a_World->GetBlock(a_X, a_Y, a_Z);
				LOG("Bucket Clicked BlockID: %d", ClickedBlock);
				ENUM_ITEM_ID NewItem = E_ITEM_EMPTY;
				switch (ClickedBlock)
				{
				case E_BLOCK_WATER:
				case E_BLOCK_STATIONARY_WATER:
					NewItem = E_ITEM_WATER_BUCKET;
					break;
				case E_BLOCK_LAVA:
				case E_BLOCK_STATIONARY_LAVA:
					NewItem = E_ITEM_LAVA_BUCKET;
					break;
				}
				if (NewItem != E_ITEM_EMPTY
					&& (a_Player->GetGameMode() == 1 || (a_Player->GetInventory().RemoveItem(cItem(a_Item->m_ItemID, 1)))))
				{
					//Give New Bucket
					a_Player->GetInventory().AddItem(cItem(NewItem, 1));
					//remove water block
					a_Player->GetWorld()->SetBlock(a_X, a_Y, a_Z, E_BLOCK_AIR, 0);
					return true;
				}
			}
			break;
		case E_ITEM_WATER_BUCKET:
		case E_ITEM_LAVA_BUCKET:
			{
				BLOCKTYPE NewBlock = (m_ItemID == E_ITEM_LAVA_BUCKET) ? E_BLOCK_LAVA : E_BLOCK_WATER;
				
				AddDirection(a_X, a_Y, a_Z, a_Dir);
				if(a_World->GetBlock(a_X, a_Y, a_Z) == E_BLOCK_AIR)
				{
					if ((a_Player->GetGameMode() == 1) || (a_Player->GetInventory().RemoveItem(cItem(a_Item->m_ItemID, 1))))
					{
						a_World->SetBlock(a_X, a_Y, a_Z, NewBlock, 0);

						if (a_Player->GetGameMode() == 1)
						{
							break;		//No new Bucket for creative players
						}

						a_Player->GetInventory().AddItem(cItem(E_ITEM_BUCKET, 1));
						return true;
					}
				}
			}
			break;
		}

		return false;
	}

};