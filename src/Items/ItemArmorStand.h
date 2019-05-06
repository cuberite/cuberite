
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemArmorStandHandler : public cItemHandler
{
public:
	cItemArmorStandHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}



	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		// TODO: Should verify that there are no armor stand or other mob before placing

		if (a_ClickedBlockFace < 0)
		{
			return false;
		}

		AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);

		double Rotation = FloorC((a_Player->GetYaw() + 22.5f) / 45.0f) * 45.0f - 180.0f;

		if (
			(a_World->SpawnArmorStand(Vector3d(a_ClickedBlockPos.x + 0.5, a_ClickedBlockPos.y, a_ClickedBlockPos.z + 0.5), Rotation) != cEntity::INVALID_ID))  // Spawning succeeded
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
