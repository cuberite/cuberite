
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"





class cItemSpawnEggHandler : public cItemHandler
{
public:
	cItemSpawnEggHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}


	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, cItem * a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace) override
	{
		if (a_BlockFace < 0)
		{
			return false;
		}
		
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

		if (a_BlockFace == BLOCK_FACE_BOTTOM)
		{
			a_BlockY--;
		}

		if (a_World->SpawnMob(a_BlockX + 0.5, a_BlockY, a_BlockZ + 0.5, a_Item->m_ItemDamage) >= 0)
		{
			if (a_Player->GetGameMode() != 1)
			{
				// The mob was spawned, "use" the item:
				a_Player->GetInventory().RemoveItem(a_Player->GetInventory().GetEquippedItem());
			}
			return true;
		}
		
		return false;
	}
} ;




