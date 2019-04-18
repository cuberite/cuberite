
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemAmorStandHandler : public cItemHandler
{
public:
	cItemAmorStandHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if (a_BlockFace < 0)
		{
			return false;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

		if (a_BlockFace == BLOCK_FACE_YM)
		{
			a_BlockY--;
		}

		if (
			(a_World->SpawnArmorStand(Vector3d(a_BlockX + 0.5, a_BlockY, a_BlockZ + 0.5)) != cEntity::INVALID_ID))  // Spawning succeeded
		{
			if (!a_Player->IsGameModeCreative())
			{
				// The mob was spawned, "use" the item:
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
			return true;
		}

		return false;
	}
} ;
