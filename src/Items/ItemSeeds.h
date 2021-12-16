
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemSeedsHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		// Only allow planting seeds from the top side of the block:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		BlockState Block;

		using namespace Block;

		// Get the produce block based on the seed item:
		switch (m_ItemType)
		{
			case E_ITEM_BEETROOT_SEEDS: Block = Beetroots::Beetroots();     break;
			case E_ITEM_CARROT:         Block = Carrots::Carrots();         break;
			case E_ITEM_MELON_SEEDS:    Block = MelonStem::MelonStem();     break;
			case E_ITEM_POTATO:         Block = Potatoes::Potatoes();       break;
			case E_ITEM_PUMPKIN_SEEDS:  Block = PumpkinStem::PumpkinStem(); break;
			case E_ITEM_SEEDS:          Block = Wheat::Wheat();             break;
			default:                    UNREACHABLE("Unsupported seed type");
		}

		return a_Player.PlaceBlock(a_PlacePosition, Block);
	}





	virtual bool IsPlaceable(void) const override
	{
		return true;
	}

protected:
	~cItemSeedsHandler() = default;
} ;

class cItemSimpleSeedsHandler final:
	public cItemSeedsHandler
{
	using cItemSeedsHandler::cItemSeedsHandler;
};



