
#pragma once





class cBlockFlowerPotHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

	static constexpr bool IsBlockFlowerPot(BlockState a_Block)
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
				return true;
			default: return false;
		}
	}

	static constexpr BlockType GetPlantFromPot(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::PottedAcaciaSapling:      return BlockType::AcaciaSapling;
			case BlockType::PottedAzureBluet:         return BlockType::AzureBluet;
			case BlockType::PottedBamboo:             return BlockType::Bamboo;
			case BlockType::PottedBirchSapling:       return BlockType::BirchSapling;
			case BlockType::PottedBlueOrchid:         return BlockType::BlueOrchid;
			case BlockType::PottedBrownMushroom:      return BlockType::BrownMushroom;
			case BlockType::PottedCactus:             return BlockType::Cactus;
			case BlockType::PottedCornflower:         return BlockType::Cornflower;
			case BlockType::PottedCrimsonRoots:       return BlockType::CrimsonRoots;
			case BlockType::PottedCrimsonFungus:      return BlockType::CrimsonFungus;
			case BlockType::PottedDandelion:          return BlockType::Dandelion;
			case BlockType::PottedDarkOakSapling:     return BlockType::DarkOakSapling;
			case BlockType::PottedDeadBush:           return BlockType::DeadBush;
			case BlockType::PottedFern:               return BlockType::Fern;
			case BlockType::PottedJungleSapling:      return BlockType::JungleSapling;
			case BlockType::PottedLilyOfTheValley:    return BlockType::LilyOfTheValley;
			case BlockType::PottedOakSapling:         return BlockType::OakSapling;
			case BlockType::PottedOrangeTulip:        return BlockType::OrangeTulip;
			case BlockType::PottedOxeyeDaisy:         return BlockType::OxeyeDaisy;
			case BlockType::PottedPinkTulip:          return BlockType::PinkTulip;
			case BlockType::PottedPoppy:              return BlockType::Poppy;
			case BlockType::PottedRedMushroom:        return BlockType::RedMushroom;
			case BlockType::PottedRedTulip:           return BlockType::RedTulip;
			case BlockType::PottedSpruceSapling:      return BlockType::SpruceSapling;
			case BlockType::PottedWarpedFungus:       return BlockType::WarpedFungus;
			case BlockType::PottedWarpedRoots:        return BlockType::WarpedRoots;
			case BlockType::PottedWhiteTulip:         return BlockType::WhiteTulip;
			case BlockType::PottedWitherRose:         return BlockType::WitherRose;
			case BlockType::PottedAllium:             return BlockType::Allium;
			default: return BlockType::Air;
		}
	}

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(E_ITEM_FLOWER_POT, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




