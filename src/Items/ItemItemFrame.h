
#pragma once

#include "ItemHandler.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/Player.h"





class cItemItemFrameHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
	{
		// Can only place on a side face:
		if ((a_ClickedBlockFace == BLOCK_FACE_NONE) || (a_ClickedBlockFace == BLOCK_FACE_YP) || (a_ClickedBlockFace == BLOCK_FACE_YM))
		{
			return false;
		}

		// Make sure the support block is a valid block to place an item frame on:
		if (!cHangingEntity::IsValidSupportBlock(a_World->GetBlock(a_ClickedBlockPos)))
		{
			return false;
		}

		// Make sure block that will be occupied by the item frame is free now:
		const auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		auto Block = a_World->GetBlock(PlacePos);
		if (Block != Block::Air::Air())
		{
			return false;
		}

		// An item frame, centred so pickups spawn nicely.
		auto ItemFrame = std::make_unique<cItemFrame>(a_ClickedBlockFace, Vector3d(0.5, 0.5, 0.5) + PlacePos);
		auto ItemFramePtr = ItemFrame.get();
		if (!ItemFramePtr->Initialize(std::move(ItemFrame), *a_World))
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}
};




