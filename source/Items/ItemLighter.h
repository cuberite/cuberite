
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"
#include "../TNTEntity.h"





class cItemLighterHandler :
	public cItemHandler
{
public:
	cItemLighterHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace) override
	{
		if (a_BlockFace < 0)
		{
			return false;
		}
		
		a_Player->UseEquippedItem();

		switch (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ))
		{
			case E_BLOCK_TNT:
			{
				// Activate the TNT:
				a_World->BroadcastSoundEffect("random.fuse", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, 0.6f);
				a_World->SpawnPrimedTNT(a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5, 4);  // 4 seconds to boom
				a_World->SetBlock(a_BlockX,a_BlockY,a_BlockZ, E_BLOCK_AIR, 0);
				break;
			}
			default:
			{
				// Light a fire next to the block:
				AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
				a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FIRE, 0);
				break;
			}
		}

		return false;
	}
} ;




