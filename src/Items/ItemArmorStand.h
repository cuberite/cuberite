
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
		if (a_ClickedBlockFace < 0)
		{
			return false;
		}

		// Check for Entities or Blocks
		const auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		const auto AbovePos = AddFaceDirection(PlacePos, BLOCK_FACE_YP);
		BLOCKTYPE PlaceBlock = a_World->GetBlock(PlacePos);
		BLOCKTYPE AboveBlock = a_World->GetBlock(AbovePos);

		if ((PlaceBlock != E_BLOCK_AIR) || (AboveBlock != E_BLOCK_AIR))
		{
			return false;
		}

		const cBoundingBox BoundingBox(PlacePos, 0.8, 2);

		bool FoundEntity = false;

		a_World->ForEachEntityInBox(BoundingBox, [&FoundEntity](cEntity & ent)
		{
			if ((ent.IsArmorStand()) || (ent.IsPawn()) || (ent.IsEnderCrystal()))
			{
				FoundEntity = true;
			}

			return false;
		});

		if (FoundEntity)
		{
			return false;
		}

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
