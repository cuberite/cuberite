
#pragma once

#include "BlockHandler.h"
#include "BlockEntity.h"


class cBlockFlowerPotHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

	static inline bool IsBlockFlowerPot(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::PottedAcaciaSapling:
			case BlockType::PottedAzureBluet:
			case BlockType::PottedBamboo:
			case BlockType::PottedBirchSapling:
			case BlockType::PottedBlueOrchid:
			case BlockType::PottedBrownMushroom:
			case BlockType::PottedCactus:
			case BlockType::PottedCornflower:
			case BlockType::PottedCrimsonRoots:
			case BlockType::PottedCrimsonFungus:
			case BlockType::PottedDandelion:
			case BlockType::PottedDarkOakSapling:
			case BlockType::PottedDeadBush:
			case BlockType::PottedFern:
			case BlockType::PottedJungleSapling:
			case BlockType::PottedLilyOfTheValley:
			case BlockType::PottedOakSapling:
			case BlockType::PottedOrangeTulip:
			case BlockType::PottedOxeyeDaisy:
			case BlockType::PottedPinkTulip:
			case BlockType::PottedPoppy:
			case BlockType::PottedRedMushroom:
			case BlockType::PottedRedTulip:
			case BlockType::PottedSpruceSapling:
			case BlockType::PottedWarpedFungus:
			case BlockType::PottedWarpedRoots:
			case BlockType::PottedWhiteTulip:
			case BlockType::PottedWitherRose:
			case BlockType::PottedAllium:
			case BlockType::FlowerPot:
			case BlockType::PottedAzaleaBush:
			case BlockType::PottedCherrySapling:
			case BlockType::PottedFloweringAzaleaBush:
			case BlockType::PottedMangrovePropagule:
			case BlockType::PottedPaleOakSapling:
			case BlockType::PottedTorchflower:
				return true;
			default: return false;
		}
	}

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::FlowerPot);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;




