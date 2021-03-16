
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemSeedsHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemSeedsHandler(int a_ItemType):
		Super(a_ItemType)
	{

	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) override
	{
		// Only allow planting seeds from the top side of the block:
		if ((a_ClickedBlockFace != BLOCK_FACE_TOP) || (a_PlacedBlockPos.y <= 0))
		{
			return false;
		}

		// Only allow placement on farmland
		if (a_World->GetBlock(a_PlacedBlockPos.addedY(-1)).Type() != BlockType::Farmland)
		{
			return false;
		}

		using namespace Block;

		// Get the produce block based on the seed item:
		switch (m_ItemType)
		{
			case E_ITEM_BEETROOT_SEEDS: a_Block = Beetroots::Beetroots();     return true;
			case E_ITEM_CARROT:         a_Block = Carrots::Carrots();         return true;
			case E_ITEM_MELON_SEEDS:    a_Block = MelonStem::MelonStem();     return true;
			case E_ITEM_POTATO:         a_Block = Potatoes::Potatoes();       return true;
			case E_ITEM_PUMPKIN_SEEDS:  a_Block = PumpkinStem::PumpkinStem(); return true;
			case E_ITEM_SEEDS:          a_Block = Wheat::Wheat();             return true;
			default: return false;
		}
	}
} ;




