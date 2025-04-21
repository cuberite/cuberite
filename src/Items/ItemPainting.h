
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Entities/Painting.h"





class cItemPaintingHandler final:
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
		// Paintings can't be flatly placed:
		if (
			(a_ClickedBlockFace == BLOCK_FACE_NONE) ||
			(a_ClickedBlockFace == BLOCK_FACE_YM) ||
			(a_ClickedBlockFace == BLOCK_FACE_YP)
		)
		{
			return false;
		}

		// Make sure the support block is a valid block to place a painting on:
		if (!cHangingEntity::IsValidSupportBlock(a_World->GetBlock(a_ClickedBlockPos)))
		{
			return false;
		}

		// Make sure block that will be occupied is free:
		auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		auto PlaceBlock = a_World->GetBlock(PlacePos);
		if (PlaceBlock.Type() != BlockType::Air)
		{
			return false;
		}

		// Define all the possible painting titles
		static const std::array<AString, 26> gPaintingTitlesList =
		{
			"Kebab",
			"Aztec",
			"Alban",
			"Aztec2",
			"Bomb",
			"Plant",
			"Wasteland",
			"Wanderer",
			"Graham",
			"Pool",
			"Courbet",
			"Sunset",
			"Sea",
			"Creebet",
			"Match",
			"Bust",
			"Stage",
			"Void",
			"SkullAndRes",
			"Wither",
			"Fighters",
			"Skeleton",
			"DonkeyKong",
			"Pointer",
			"Pigscene",
			"BurningSkull"
		};

		auto paint_index = a_World->GetTickRandomNumber(static_cast<int>(gPaintingTitlesList.size()) - 1);
		auto PaintingTitle = gPaintingTitlesList[static_cast<size_t>(paint_index)];

		// A painting, centred so pickups spawn nicely.
		auto Painting = std::make_unique<cPainting>(PaintingTitle, a_ClickedBlockFace, Vector3d(0.5, 0.5, 0.5) + PlacePos, static_cast<UInt32>(paint_index));
		auto PaintingPtr = Painting.get();
		if (!PaintingPtr->Initialize(std::move(Painting), *a_World))
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




