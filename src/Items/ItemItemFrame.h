
#pragma once

#include "ItemHandler.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/Player.h"





class cItemItemFrameHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemItemFrameHandler(int a_ItemType):
		Super(a_ItemType)
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
		// Can only place on a side face:
		if ((a_ClickedBlockFace == BLOCK_FACE_NONE) || (a_ClickedBlockFace == BLOCK_FACE_YP) || (a_ClickedBlockFace == BLOCK_FACE_YM))
		{
			return false;
		}

		// Make sure block that will be occupied by the item frame is free now:
		const auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		BLOCKTYPE Block = a_World->GetBlock(PlacePos);
		if (Block != E_BLOCK_AIR)
		{
			return false;
		}

		// Place the item frame:
		auto ItemFrame = std::make_unique<cItemFrame>(a_ClickedBlockFace, PlacePos);
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




