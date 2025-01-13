
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
			case Item::BeetrootSeeds: Block = Beetroots::Beetroots();     break;
			case Item::Carrot:        Block = Carrots::Carrots();         break;
			case Item::MelonSeeds:    Block = MelonStem::MelonStem();     break;
			case Item::Potato:        Block = Potatoes::Potatoes();       break;
			case Item::PumpkinSeeds:  Block = PumpkinStem::PumpkinStem(); break;
			case Item::WheatSeeds:    Block = Wheat::Wheat();             break;
			case Item::TorchflowerSeeds:Block = TorchflowerCrop::TorchflowerCrop(); break;
			case Item::PitcherPod:    Block = PitcherPlant::PitcherPlant(); break;
			default:                  UNREACHABLE("Unsupported seed type");
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



