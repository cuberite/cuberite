#include "Globals.h"

#include "BlockInfo.h"
#include "BlockType.h"

#include "Blocks/BlockHandler.h"
#include "Blocks/BlockSlab.h"




bool IsBlockAir(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Air:
		case BlockType::CaveAir:
		case BlockType::VoidAir:
			return true;
		default:
			return false;
	}
}





bool IsBlockAnvil(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Anvil:
		case BlockType::ChippedAnvil:
		case BlockType::DamagedAnvil:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockIce(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Ice:
		case BlockType::PackedIce:
		case BlockType::FrostedIce:
			return true;
		default:
			return false;
	}
}





bool IsBlockLiquid(BlockState a_Block)
{
	return ((a_Block.Type() == BlockType::Water) || (a_Block.Type() == BlockType::Lava));
}





bool IsBlockMaterialDirt(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::CoarseDirt:
		case BlockType::Dirt:
		case BlockType::Farmland:
		case BlockType::GrassBlock:
		case BlockType::DirtPath:
			return true;
		default: return false;
	}
}





bool IsBlockMaterialGourd(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Pumpkin:
		case BlockType::JackOLantern:
		case BlockType::Melon:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialIron(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::LapisBlock:
		case BlockType::GoldBlock:
		case BlockType::IronBlock:
		case BlockType::DiamondBlock:
		case BlockType::IronDoor:
		case BlockType::IronBars:
		case BlockType::BrewingStand:
		case BlockType::Cauldron:
		case BlockType::EmeraldBlock:
		case BlockType::CommandBlock:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::RedstoneBlock:
		case BlockType::Hopper:
		case BlockType::IronTrapdoor:
		case BlockType::RepeatingCommandBlock:
		case BlockType::ChainCommandBlock:
		case BlockType::StructureBlock:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialPlants(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Dandelion:
		case BlockType::BrownMushroom:
		case BlockType::RedMushroom:
		case BlockType::Wheat:
		case BlockType::PumpkinStem:
		case BlockType::AttachedPumpkinStem:
		case BlockType::MelonStem:
		case BlockType::AttachedMelonStem:
		case BlockType::LilyPad:
		case BlockType::NetherWart:
		case BlockType::Cocoa:
		case BlockType::Carrots:
		case BlockType::Potatoes:
		case BlockType::ChorusPlant:
		case BlockType::ChorusFlower:
		case BlockType::Beetroots:
			// Flower
		case BlockType::Allium:
		case BlockType::AzureBluet:
		case BlockType::BlueOrchid:
		case BlockType::Cornflower:
		case BlockType::LilyOfTheValley:
		case BlockType::OrangeTulip:
		case BlockType::OxeyeDaisy:
		case BlockType::PinkTulip:
		case BlockType::Poppy:
		case BlockType::RedTulip:
		case BlockType::WhiteTulip:
			// Big Flower
		case BlockType::TallGrass:
		case BlockType::LargeFern:
		case BlockType::Lilac:
		case BlockType::Peony:
		case BlockType::RoseBush:
		case BlockType::Sunflower:
			// Sapling
		case BlockType::AcaciaSapling:
		case BlockType::BirchSapling:
		case BlockType::JungleSapling:
		case BlockType::DarkOakSapling:
		case BlockType::OakSapling:
		case BlockType::SpruceSapling:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialRock(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Andesite:
		case BlockType::Bedrock:
		case BlockType::BlackTerracotta:
		case BlockType::BlueTerracotta:
		case BlockType::BoneBlock:
		case BlockType::BrickStairs:
		case BlockType::Bricks:
		case BlockType::BrownTerracotta:
		case BlockType::CoalBlock:
		case BlockType::CoalOre:
		case BlockType::Cobblestone:
		case BlockType::CobblestoneStairs:
		case BlockType::CobblestoneWall:
		case BlockType::CyanTerracotta:
		case BlockType::DiamondOre:
		case BlockType::Diorite:
		case BlockType::Dispenser:
		case BlockType::Dropper:
		case BlockType::EmeraldOre:
		case BlockType::EnchantingTable:
		case BlockType::EndPortalFrame:
		case BlockType::EndStone:
		case BlockType::EndStoneBricks:
		case BlockType::EnderChest:
		case BlockType::Furnace:
		case BlockType::GoldOre:
		case BlockType::Granite:
		case BlockType::GrayTerracotta:
		case BlockType::GreenTerracotta:
		case BlockType::IronOre:
		case BlockType::LapisOre:
		case BlockType::LightBlueTerracotta:
		case BlockType::LimeTerracotta:
		case BlockType::MagentaTerracotta:
		case BlockType::MagmaBlock:
		case BlockType::MossyCobblestone:
		case BlockType::NetherBrickFence:
		case BlockType::NetherBrickStairs:
		case BlockType::NetherBricks:
		case BlockType::NetherQuartzOre:
		case BlockType::Netherrack:
		case BlockType::Observer:
		case BlockType::Obsidian:
		case BlockType::OrangeTerracotta:
		case BlockType::PinkTerracotta:
		case BlockType::Prismarine:
		case BlockType::PurpleTerracotta:
		case BlockType::PurpurBlock:
		case BlockType::PurpurPillar:
		case BlockType::PurpurSlab:
		case BlockType::PurpurStairs:
		case BlockType::QuartzBlock:
		case BlockType::QuartzStairs:
		case BlockType::RedNetherBricks:
		case BlockType::RedSandstone:
		case BlockType::RedSandstoneSlab:
		case BlockType::RedSandstoneStairs:
		case BlockType::RedTerracotta:
		case BlockType::RedstoneOre:
		case BlockType::Sandstone:
		case BlockType::SandstoneStairs:
		case BlockType::Spawner:
		case BlockType::Stone:
		case BlockType::StoneBrickStairs:
		case BlockType::StoneBricks:
		case BlockType::StonePressurePlate:
		case BlockType::StoneSlab:
		case BlockType::Terracotta:
		case BlockType::WhiteTerracotta:
		case BlockType::YellowTerracotta:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialVine(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::DeadBush:
		case BlockType::Vine:
			// Big Flower
		case BlockType::TallGrass:
		case BlockType::LargeFern:
		case BlockType::Lilac:
		case BlockType::Peony:
		case BlockType::RoseBush:
		case BlockType::Sunflower:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockMaterialWood(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::NoteBlock:
		case BlockType::Bookshelf:
		case BlockType::Chest:
		case BlockType::CraftingTable:
		case BlockType::Jukebox:
		case BlockType::BrownMushroomBlock:
		case BlockType::RedMushroomBlock:
		case BlockType::TrappedChest:
		case BlockType::DaylightDetector:
			// BannerEntity
		case BlockType::BlackBanner:
		case BlockType::BlueBanner:
		case BlockType::BrownBanner:
		case BlockType::CyanBanner:
		case BlockType::GrayBanner:
		case BlockType::GreenBanner:
		case BlockType::LightBlueBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LimeBanner:
		case BlockType::MagentaBanner:
		case BlockType::OrangeBanner:
		case BlockType::PinkBanner:
		case BlockType::PurpleBanner:
		case BlockType::RedBanner:
		case BlockType::WhiteBanner:
		case BlockType::YellowBanner:

		case BlockType::BlackWallBanner:
		case BlockType::BlueWallBanner:
		case BlockType::BrownWallBanner:
		case BlockType::CyanWallBanner:
		case BlockType::GrayWallBanner:
		case BlockType::GreenWallBanner:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayWallBanner:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaWallBanner:
		case BlockType::OrangeWallBanner:
		case BlockType::PinkWallBanner:
		case BlockType::PurpleWallBanner:
		case BlockType::RedWallBanner:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowWallBanner:
			// Doors
		case BlockType::AcaciaDoor:
		case BlockType::BirchDoor:
		case BlockType::CrimsonDoor:
		case BlockType::DarkOakDoor:
		case BlockType::JungleDoor:
		case BlockType::OakDoor:
		case BlockType::SpruceDoor:
		case BlockType::WarpedDoor:
			// Fence
		case BlockType::AcaciaFence:
		case BlockType::BirchFence:
		case BlockType::CrimsonFence:
		case BlockType::DarkOakFence:
		case BlockType::JungleFence:
		case BlockType::OakFence:
		case BlockType::SpruceFence:
		case BlockType::WarpedFence:
			// Fence Gate
		case BlockType::AcaciaFenceGate:
		case BlockType::BirchFenceGate:
		case BlockType::CrimsonFenceGate:
		case BlockType::DarkOakFenceGate:
		case BlockType::JungleFenceGate:
		case BlockType::OakFenceGate:
		case BlockType::SpruceFenceGate:
		case BlockType::WarpedFenceGate:
			// Logs
		case BlockType::AcaciaLog:
		case BlockType::BirchLog:
		case BlockType::DarkOakLog:
		case BlockType::JungleLog:
		case BlockType::OakLog:
		case BlockType::SpruceLog:
			// PressurePlate
		case BlockType::AcaciaPressurePlate:
		case BlockType::BirchPressurePlate:
		case BlockType::CrimsonPressurePlate:
		case BlockType::DarkOakPressurePlate:
		case BlockType::JunglePressurePlate:
		case BlockType::OakPressurePlate:
		case BlockType::SprucePressurePlate:
		case BlockType::WarpedPressurePlate:
			// Planks
		case BlockType::AcaciaPlanks:
		case BlockType::BirchPlanks:
		case BlockType::CrimsonPlanks:
		case BlockType::DarkOakPlanks:
		case BlockType::JunglePlanks:
		case BlockType::OakPlanks:
		case BlockType::SprucePlanks:
		case BlockType::WarpedPlanks:
			// Sign
		case BlockType::AcaciaSign:
		case BlockType::BirchSign:
		case BlockType::CrimsonSign:
		case BlockType::DarkOakSign:
		case BlockType::JungleSign:
		case BlockType::OakSign:
		case BlockType::SpruceSign:
		case BlockType::WarpedSign:

		case BlockType::AcaciaWallSign:
		case BlockType::BirchWallSign:
		case BlockType::CrimsonWallSign:
		case BlockType::DarkOakWallSign:
		case BlockType::JungleWallSign:
		case BlockType::OakWallSign:
		case BlockType::SpruceWallSign:
		case BlockType::WarpedWallSign:
			// Slabs
		case BlockType::AcaciaSlab:
		case BlockType::BirchSlab:
		case BlockType::CrimsonSlab:
		case BlockType::DarkOakSlab:
		case BlockType::JungleSlab:
		case BlockType::OakSlab:
		case BlockType::SpruceSlab:
		case BlockType::WarpedSlab:
			// Stairs
		case BlockType::AcaciaStairs:
		case BlockType::BirchStairs:
		case BlockType::CrimsonStairs:
		case BlockType::DarkOakStairs:
		case BlockType::JungleStairs:
		case BlockType::OakStairs:
		case BlockType::SpruceStairs:
		case BlockType::WarpedStairs:
			// Trapdoor
		case BlockType::AcaciaTrapdoor:
		case BlockType::BirchTrapdoor:
		case BlockType::CrimsonTrapdoor:
		case BlockType::DarkOakTrapdoor:
		case BlockType::JungleTrapdoor:
		case BlockType::OakTrapdoor:
		case BlockType::SpruceTrapdoor:
		case BlockType::WarpedTrapdoor:
			return true;
		default:
			return false;
	}
}





bool IsBlockShulkerBox(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::ShulkerBox:
		case BlockType::BlackShulkerBox:
		case BlockType::BlueShulkerBox:
		case BlockType::BrownShulkerBox:
		case BlockType::CyanShulkerBox:
		case BlockType::GrayShulkerBox:
		case BlockType::GreenShulkerBox:
		case BlockType::LightBlueShulkerBox:
		case BlockType::LightGrayShulkerBox:
		case BlockType::LimeShulkerBox:
		case BlockType::MagentaShulkerBox:
		case BlockType::OrangeShulkerBox:
		case BlockType::PinkShulkerBox:
		case BlockType::PurpleShulkerBox:
		case BlockType::RedShulkerBox:
		case BlockType::WhiteShulkerBox:
		case BlockType::YellowShulkerBox:
			return true;
		default: return false;
	}
}





bool IsBlockMobHead(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





bool IsBlockWaterOrIce(BlockState a_Block)
{
	return ((a_Block.Type() == BlockType::Water) || IsBlockIce(a_Block));
}





////////////////////////////////////////////////////////////////////////////////
// cBlockInfo:

LIGHTTYPE cBlockInfo::GetLightValue(BlockState a_Block)
{
	// Emissive blocks:
	switch (a_Block.Type())
	{
		case BlockType::Comparator:
		{
			return Block::Comparator::Powered(a_Block) ? 9 : 0;
		}
		case BlockType::Beacon:               return 15;
		case BlockType::BrewingStand:         return 1;
		case BlockType::BrownMushroom:        return 1;
		case BlockType::Furnace:
		{
			return Block::Furnace::Lit(a_Block) ? 13 : 0;
		}
		case BlockType::DragonEgg:            return 1;
		case BlockType::EndPortal:            return 15;
		case BlockType::EndPortalFrame:       return 1;
		case BlockType::EndRod:               return 14;
		case BlockType::EnderChest:           return 7;
		case BlockType::Fire:                 return 15;
		case BlockType::Glowstone:            return 15;
		case BlockType::JackOLantern:         return 15;
		case BlockType::Lava:                 return 15;
		case BlockType::MagmaBlock:           return 3;
		case BlockType::NetherPortal:         return 11;
		case BlockType::RedstoneLamp:
		{
			return Block::RedstoneLamp::Lit(a_Block) ? 15 : 0;
		}
		case BlockType::RedstoneOre:
		{
			return Block::RedstoneOre::Lit(a_Block) ? 9 : 0;
		}
		case BlockType::Repeater:
		{
			return Block::Repeater::Powered(a_Block) ? 9 : 0;
		}
		case BlockType::RedstoneTorch:
		{
			return Block::RedstoneTorch::Lit(a_Block) ? 7 : 0;
		}
		case BlockType::RedstoneWallTorch:
		{
			return Block::RedstoneWallTorch::Lit(a_Block) ? 7 : 0;
		}
		case BlockType::SeaLantern:        return 15;
		case BlockType::Torch:             return 14;
		default:                           return 0;
	}
}





LIGHTTYPE cBlockInfo::GetSpreadLightFalloff(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		// Spread blocks:
		case BlockType::AcaciaDoor:
		case BlockType::AcaciaFence:
		case BlockType::AcaciaFenceGate:
		case BlockType::AcaciaLeaves:
		case BlockType::AcaciaPressurePlate:
		case BlockType::AcaciaSapling:
		case BlockType::AcaciaSign:
		case BlockType::AcaciaWallSign:
		case BlockType::ActivatorRail:
		case BlockType::Air:
		case BlockType::Allium:
		case BlockType::Anvil:
		case BlockType::AzureBluet:
		case BlockType::Barrier:
		case BlockType::Beacon:
		case BlockType::Beetroots:
		case BlockType::BirchDoor:
		case BlockType::BirchFence:
		case BlockType::BirchFenceGate:
		case BlockType::BirchLeaves:
		case BlockType::BirchPressurePlate:
		case BlockType::BirchSapling:
		case BlockType::BirchSign:
		case BlockType::BirchWallSign:
		case BlockType::BlackBanner:
		case BlockType::BlackBed:
		case BlockType::BlackCarpet:
		case BlockType::BlackStainedGlass:
		case BlockType::BlackStainedGlassPane:
		case BlockType::BlackWallBanner:
		case BlockType::BlueBanner:
		case BlockType::BlueBed:
		case BlockType::BlueCarpet:
		case BlockType::BlueOrchid:
		case BlockType::BlueStainedGlass:
		case BlockType::BlueStainedGlassPane:
		case BlockType::BlueWallBanner:
		case BlockType::BrewingStand:
		case BlockType::BrownBanner:
		case BlockType::BrownBed:
		case BlockType::BrownCarpet:
		case BlockType::BrownMushroom:
		case BlockType::BrownStainedGlass:
		case BlockType::BrownStainedGlassPane:
		case BlockType::BrownWallBanner:
		case BlockType::Cactus:
		case BlockType::Cake:
		case BlockType::Carrots:
		case BlockType::Cauldron:
		case BlockType::CaveAir:
		case BlockType::Chest:
		case BlockType::ChippedAnvil:
		case BlockType::ChorusFlower:
		case BlockType::ChorusPlant:
		case BlockType::CobblestoneWall:
		case BlockType::Cobweb:
		case BlockType::Cocoa:
		case BlockType::Comparator:
		case BlockType::Cornflower:
		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::CrimsonDoor:
		case BlockType::CrimsonFenceGate:
		case BlockType::CrimsonPressurePlate:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonWallSign:
		case BlockType::CyanBanner:
		case BlockType::CyanBed:
		case BlockType::CyanCarpet:
		case BlockType::CyanStainedGlass:
		case BlockType::CyanStainedGlassPane:
		case BlockType::CyanWallBanner:
		case BlockType::DamagedAnvil:
		case BlockType::Dandelion:
		case BlockType::DarkOakDoor:
		case BlockType::DarkOakFence:
		case BlockType::DarkOakFenceGate:
		case BlockType::DarkOakLeaves:
		case BlockType::DarkOakPressurePlate:
		case BlockType::DarkOakSapling:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakWallSign:
		case BlockType::DaylightDetector:
		case BlockType::DeadBush:
		case BlockType::DetectorRail:
		case BlockType::DragonEgg:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::EnchantingTable:
		case BlockType::EndPortal:
		case BlockType::EndPortalFrame:
		case BlockType::EndRod:
		case BlockType::EnderChest:
		case BlockType::Fire:
		case BlockType::Glass:
		case BlockType::GlassPane:
		case BlockType::GrayBanner:
		case BlockType::GrayBed:
		case BlockType::GrayCarpet:
		case BlockType::GrayStainedGlass:
		case BlockType::GrayStainedGlassPane:
		case BlockType::GrayWallBanner:
		case BlockType::ShortGrass:
		case BlockType::GreenBanner:
		case BlockType::GreenBed:
		case BlockType::GreenCarpet:
		case BlockType::GreenStainedGlass:
		case BlockType::GreenStainedGlassPane:
		case BlockType::GreenWallBanner:
		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::Hopper:
		case BlockType::IronBars:
		case BlockType::IronDoor:
		case BlockType::IronTrapdoor:
		case BlockType::JungleDoor:
		case BlockType::JungleFence:
		case BlockType::JungleFenceGate:
		case BlockType::JungleLeaves:
		case BlockType::JunglePressurePlate:
		case BlockType::JungleSapling:
		case BlockType::JungleSign:
		case BlockType::JungleWallSign:
		case BlockType::Ladder:
		case BlockType::LargeFern:
		case BlockType::Lever:
		case BlockType::LightBlueBanner:
		case BlockType::LightBlueBed:
		case BlockType::LightBlueCarpet:
		case BlockType::LightBlueStainedGlass:
		case BlockType::LightBlueStainedGlassPane:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LightGrayBed:
		case BlockType::LightGrayCarpet:
		case BlockType::LightGrayStainedGlass:
		case BlockType::LightGrayStainedGlassPane:
		case BlockType::LightGrayWallBanner:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::Lilac:
		case BlockType::LilyOfTheValley:
		case BlockType::LilyPad:
		case BlockType::LimeBanner:
		case BlockType::LimeBed:
		case BlockType::LimeCarpet:
		case BlockType::LimeStainedGlass:
		case BlockType::LimeStainedGlassPane:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaBanner:
		case BlockType::MagentaBed:
		case BlockType::MagentaCarpet:
		case BlockType::MagentaStainedGlass:
		case BlockType::MagentaStainedGlassPane:
		case BlockType::MagentaWallBanner:
		case BlockType::MelonStem:
		case BlockType::NetherBrickFence:
		case BlockType::NetherPortal:
		case BlockType::NetherWart:
		case BlockType::OakDoor:
		case BlockType::OakFence:
		case BlockType::OakFenceGate:
		case BlockType::OakLeaves:
		case BlockType::OakPressurePlate:
		case BlockType::OakSapling:
		case BlockType::OakSign:
		case BlockType::OakWallSign:
		case BlockType::OrangeBanner:
		case BlockType::OrangeBed:
		case BlockType::OrangeCarpet:
		case BlockType::OrangeStainedGlass:
		case BlockType::OrangeStainedGlassPane:
		case BlockType::OrangeTulip:
		case BlockType::OrangeWallBanner:
		case BlockType::OxeyeDaisy:
		case BlockType::Peony:
		case BlockType::PinkBanner:
		case BlockType::PinkBed:
		case BlockType::PinkCarpet:
		case BlockType::PinkStainedGlass:
		case BlockType::PinkStainedGlassPane:
		case BlockType::PinkTulip:
		case BlockType::PinkWallBanner:
		case BlockType::Piston:
		case BlockType::PistonHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::PolishedBlackstonePressurePlate:
		case BlockType::Poppy:
		case BlockType::Potatoes:
		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAllium:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedCrimsonRoots:
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
		case BlockType::PoweredRail:
		case BlockType::PumpkinStem:
		case BlockType::PurpleBanner:
		case BlockType::PurpleBed:
		case BlockType::PurpleCarpet:
		case BlockType::PurpleStainedGlass:
		case BlockType::PurpleStainedGlassPane:
		case BlockType::PurpleWallBanner:
		case BlockType::Rail:
		case BlockType::RedBanner:
		case BlockType::RedBed:
		case BlockType::RedCarpet:
		case BlockType::RedMushroom:
		case BlockType::RedStainedGlass:
		case BlockType::RedStainedGlassPane:
		case BlockType::RedTulip:
		case BlockType::RedWallBanner:
		case BlockType::RedstoneTorch:
		case BlockType::RedstoneWallTorch:
		case BlockType::RedstoneWire:
		case BlockType::Repeater:
		case BlockType::RoseBush:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::Snow:
		case BlockType::Spawner:
		case BlockType::SpruceDoor:
		case BlockType::SpruceFence:
		case BlockType::SpruceFenceGate:
		case BlockType::SpruceLeaves:
		case BlockType::SprucePressurePlate:
		case BlockType::SpruceSapling:
		case BlockType::SpruceSign:
		case BlockType::SpruceWallSign:
		case BlockType::StickyPiston:
		case BlockType::StoneButton:
		case BlockType::StonePressurePlate:
		case BlockType::SugarCane:
		case BlockType::Sunflower:
		case BlockType::TallGrass:
		case BlockType::Torch:
		case BlockType::TrappedChest:
		case BlockType::Tripwire:
		case BlockType::TripwireHook:
		case BlockType::Vine:
		case BlockType::VoidAir:
		case BlockType::WallTorch:
		case BlockType::WarpedDoor:
		case BlockType::WarpedFence:
		case BlockType::WarpedFenceGate:
		case BlockType::WarpedPressurePlate:
		case BlockType::WarpedSign:
		case BlockType::WarpedWallSign:
		case BlockType::Wheat:
		case BlockType::WhiteBanner:
		case BlockType::WhiteBed:
		case BlockType::WhiteCarpet:
		case BlockType::WhiteStainedGlass:
		case BlockType::WhiteStainedGlassPane:
		case BlockType::WhiteTulip:
		case BlockType::WhiteWallBanner:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
		case BlockType::YellowBanner:
		case BlockType::YellowBed:
		case BlockType::YellowCarpet:
		case BlockType::YellowStainedGlass:
		case BlockType::YellowStainedGlassPane:
		case BlockType::YellowWallBanner:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:
			return 1;
		// Light in ice and water disappears faster:
		case BlockType::Ice:
		case BlockType::Water:
			return 3;
		// Light does not pass through anything else (note: including tilled farmland, stairs, and slabs):
		default: return 15;
	}
}





bool cBlockInfo::CanBeTerraformed(BlockState a_Block)
{
	// Blocks that can be terraformed:
	switch (a_Block.Type())
	{
		case BlockType::CoalOre:
		case BlockType::CoarseDirt:
		case BlockType::Cobblestone:
		case BlockType::DiamondOre:
		case BlockType::Dirt:
		case BlockType::GoldOre:
		case BlockType::GrassBlock:
		case BlockType::Gravel:
		case BlockType::IronOre:
		case BlockType::Mycelium:
		case BlockType::Netherrack:
		case BlockType::RedstoneOre:
		case BlockType::Sand:
		case BlockType::Sandstone:
		case BlockType::SoulSand:
		case BlockType::Stone:
			// Teracotta
		case BlockType::Terracotta:
		case BlockType::BlackTerracotta:
		case BlockType::BlueTerracotta:
		case BlockType::BrownTerracotta:
		case BlockType::CyanTerracotta:
		case BlockType::GrayTerracotta:
		case BlockType::GreenTerracotta:
		case BlockType::LimeTerracotta:
		case BlockType::LightBlueTerracotta:
		case BlockType::MagentaTerracotta:
		case BlockType::PinkTerracotta:
		case BlockType::PurpleTerracotta:
		case BlockType::RedTerracotta:
		case BlockType::OrangeTerracotta:
		case BlockType::YellowTerracotta:
		case BlockType::WhiteTerracotta:

		return true;
		default: return false;
	}
}





bool cBlockInfo::FullyOccupiesVoxel(const BlockState Block)
{
	return cBlockHandler::For(Block.Type()).FullyOccupiesVoxel(Block);
	/*
	// Blocks that fully occupy their voxel - used as a guide for torch placeable blocks, amongst other things:
	switch (Block.Type())
	{
		case BlockType::Barrier:
		case BlockType::Bedrock:
		case BlockType::CoalBlock:
		case BlockType::RedstoneBlock:
		case BlockType::BoneBlock:
		case BlockType::Bookshelf:
		case BlockType::Bricks:
		case BlockType::ChainCommandBlock:
		case BlockType::Clay:
		case BlockType::CoalOre:
		case BlockType::Cobblestone:
		case BlockType::CommandBlock:
		case BlockType::BlackConcrete:
		case BlockType::BlueConcrete:
		case BlockType::BrownConcrete:
		case BlockType::CyanConcrete:
		case BlockType::GrayConcrete:
		case BlockType::GreenConcrete:
		case BlockType::LightBlueConcrete:
		case BlockType::LightGrayConcrete:
		case BlockType::LimeConcrete:
		case BlockType::MagentaConcrete:
		case BlockType::OrangeConcrete:
		case BlockType::PinkConcrete:
		case BlockType::PurpleConcrete:
		case BlockType::RedConcrete:
		case BlockType::WhiteConcrete:
		case BlockType::YellowConcrete:
		case BlockType::BlackConcretePowder:
		case BlockType::BlueConcretePowder:
		case BlockType::BrownConcretePowder:
		case BlockType::CyanConcretePowder:
		case BlockType::GrayConcretePowder:
		case BlockType::GreenConcretePowder:
		case BlockType::LightBlueConcretePowder:
		case BlockType::LightGrayConcretePowder:
		case BlockType::LimeConcretePowder:
		case BlockType::MagentaConcretePowder:
		case BlockType::OrangeConcretePowder:
		case BlockType::PinkConcretePowder:
		case BlockType::PurpleConcretePowder:
		case BlockType::RedConcretePowder:
		case BlockType::WhiteConcretePowder:
		case BlockType::YellowConcretePowder:
		case BlockType::CraftingTable:
		case BlockType::DiamondBlock:
		case BlockType::DiamondOre:

		case BlockType::Dirt:
		case BlockType::GrassBlock:
		case BlockType::CoarseDirt:

		case BlockType::Dispenser:

		case BlockType::Dropper:
		case BlockType::EmeraldBlock:
		case BlockType::EmeraldOre:
		case BlockType::EndStoneBricks:
		case BlockType::EndStone:
		case BlockType::FrostedIce:
		case BlockType::Furnace:
		case BlockType::Glowstone:
		case BlockType::GoldBlock:
		case BlockType::GoldOre:
		case BlockType::WhiteGlazedTerracotta:
		case BlockType::OrangeGlazedTerracotta:
		case BlockType::MagentaGlazedTerracotta:
		case BlockType::LightBlueGlazedTerracotta:
		case BlockType::YellowGlazedTerracotta:
		case BlockType::LimeGlazedTerracotta:
		case BlockType::PinkGlazedTerracotta:
		case BlockType::GrayGlazedTerracotta:
		case BlockType::LightGrayGlazedTerracotta:
		case BlockType::CyanGlazedTerracotta:
		case BlockType::PurpleGlazedTerracotta:
		case BlockType::BlueGlazedTerracotta:
		case BlockType::BrownGlazedTerracotta:
		case BlockType::GreenGlazedTerracotta:
		case BlockType::BlackGlazedTerracotta:
		case BlockType::RedGlazedTerracotta:

		case BlockType::Gravel:
		case BlockType::HayBale:
		case BlockType::BrownMushroomBlock:
		case BlockType::RedMushroomBlock:
		case BlockType::Ice:
		case BlockType::IronBlock:
		case BlockType::IronOre:
		case BlockType::JackOLantern:
		case BlockType::Jukebox:
		case BlockType::LapisBlock:
		case BlockType::LapisOre:
			// Log
		case BlockType::AcaciaLog:
		case BlockType::BirchLog:
		case BlockType::DarkOakLog:
		case BlockType::JungleLog:
		case BlockType::OakLog:
		case BlockType::SpruceLog:

		case BlockType::MagmaBlock:
		case BlockType::Melon:
		case BlockType::Spawner:
		case BlockType::MossyCobblestone:
		case BlockType::Mycelium:
		case BlockType::Netherrack:
		case BlockType::NetherBricks:
		case BlockType::NetherQuartzOre:
		case BlockType::NetherWartBlock:

		case BlockType::NoteBlock:
		case BlockType::Observer:
		case BlockType::Obsidian:
		case BlockType::PackedIce:
			// Planks
		case BlockType::AcaciaPlanks:
		case BlockType::BirchPlanks:
		case BlockType::CrimsonPlanks:
		case BlockType::DarkOakPlanks:
		case BlockType::JunglePlanks:
		case BlockType::OakPlanks:
		case BlockType::SprucePlanks:
		case BlockType::WarpedPlanks:

		case BlockType::Prismarine:
		case BlockType::Pumpkin:
		case BlockType::PurpurBlock:
		case BlockType::PurpurPillar:
		case BlockType::QuartzBlock:
		case BlockType::RedNetherBricks:
		case BlockType::RedSandstone:
		case BlockType::RedstoneLamp:
		case BlockType::RedstoneOre:
		case BlockType::RepeatingCommandBlock:
		case BlockType::Sandstone:
		case BlockType::Sand:

		case BlockType::InfestedCobblestone:
		case BlockType::InfestedStoneBricks:
		case BlockType::InfestedMossyStoneBricks:
		case BlockType::InfestedCrackedStoneBricks:
		case BlockType::InfestedChiseledStoneBricks:
		case BlockType::InfestedStone:

		case BlockType::Sponge:
		case BlockType::Stone:
		case BlockType::StoneBricks:
		case BlockType::StructureBlock:
			// Teracotta
		case BlockType::Terracotta:
		case BlockType::BlackTerracotta:
		case BlockType::BlueTerracotta:
		case BlockType::BrownTerracotta:
		case BlockType::CyanTerracotta:
		case BlockType::GrayTerracotta:
		case BlockType::GreenTerracotta:
		case BlockType::LimeTerracotta:
		case BlockType::LightBlueTerracotta:
		case BlockType::MagentaTerracotta:
		case BlockType::PinkTerracotta:
		case BlockType::PurpleTerracotta:
		case BlockType::RedTerracotta:
		case BlockType::OrangeTerracotta:
		case BlockType::YellowTerracotta:
		case BlockType::WhiteTerracotta:

		case BlockType::BlackWool:
		case BlockType::BlueWool:
		case BlockType::BrownWool:
		case BlockType::CyanWool:
		case BlockType::GrayWool:
		case BlockType::GreenWool:
		case BlockType::LightBlueWool:
		case BlockType::LightGrayWool:
		case BlockType::LimeWool:
		case BlockType::MagentaWool:
		case BlockType::OrangeWool:
		case BlockType::PinkWool:
		case BlockType::PurpleWool:
		case BlockType::RedWool:
		case BlockType::WhiteWool:
		case BlockType::YellowWool:
			return true;
		default: return false;
	}
	*/
}





bool cBlockInfo::IsClickedThrough(const BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Fire:
		case BlockType::SoulFire:
			return true;
		default: return false;
	}
}





bool cBlockInfo::IsOneHitDig(BlockState a_Block)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#endif

	// GetHardness returns exactly 0 for one hit break blocks:
	return GetHardness(a_Block) == 0;

#ifdef __clang__
#pragma clang diagnostic pop
#endif
}





bool cBlockInfo::IsPistonBreakable(const BlockState a_Block)
{
	// Blocks that break when pushed by piston:
	switch (a_Block.Type())
	{
		case BlockType::AcaciaButton:
		case BlockType::AcaciaDoor:
		case BlockType::AcaciaLeaves:
		case BlockType::AcaciaPressurePlate:
		case BlockType::AcaciaSapling:
		case BlockType::AcaciaSign:
		case BlockType::AcaciaTrapdoor:
		case BlockType::AcaciaWallSign:
		case BlockType::Air:
		case BlockType::Allium:
		case BlockType::AttachedMelonStem:
		case BlockType::AttachedPumpkinStem:
		case BlockType::AzureBluet:
		case BlockType::Beetroots:
		case BlockType::BirchButton:
		case BlockType::BirchDoor:
		case BlockType::BirchLeaves:
		case BlockType::BirchPressurePlate:
		case BlockType::BirchSapling:
		case BlockType::BirchSign:
		case BlockType::BirchTrapdoor:
		case BlockType::BirchWallSign:
		case BlockType::BlackBed:
		case BlockType::BlackShulkerBox:
		case BlockType::BlueBed:
		case BlockType::BlueOrchid:
		case BlockType::BlueShulkerBox:
		case BlockType::BrownBed:
		case BlockType::BrownMushroom:
		case BlockType::BrownShulkerBox:
		case BlockType::Cactus:
		case BlockType::Cake:
		case BlockType::Carrots:
		case BlockType::CaveAir:
		case BlockType::ChorusFlower:
		case BlockType::ChorusPlant:
		case BlockType::Cobweb:
		case BlockType::Cocoa:
		case BlockType::Comparator:
		case BlockType::Cornflower:
		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::CrimsonButton:
		case BlockType::CrimsonPressurePlate:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonTrapdoor:
		case BlockType::CrimsonWallSign:
		case BlockType::CyanBed:
		case BlockType::CyanShulkerBox:
		case BlockType::Dandelion:
		case BlockType::DarkOakButton:
		case BlockType::DarkOakDoor:
		case BlockType::DarkOakLeaves:
		case BlockType::DarkOakPressurePlate:
		case BlockType::DarkOakSapling:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakTrapdoor:
		case BlockType::DarkOakWallSign:
		case BlockType::DeadBush:
		case BlockType::DragonEgg:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::Fire:
		case BlockType::GrayBed:
		case BlockType::GrayShulkerBox:
		case BlockType::GreenBed:
		case BlockType::GreenShulkerBox:
		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::IronDoor:
		case BlockType::IronTrapdoor:
		case BlockType::JackOLantern:
		case BlockType::JungleButton:
		case BlockType::JungleDoor:
		case BlockType::JungleLeaves:
		case BlockType::JunglePressurePlate:
		case BlockType::JungleSapling:
		case BlockType::JungleSign:
		case BlockType::JungleTrapdoor:
		case BlockType::JungleWallSign:
		case BlockType::Ladder:
		case BlockType::LargeFern:
		case BlockType::Lava:
		case BlockType::Lever:
		case BlockType::LightBlueBed:
		case BlockType::LightBlueShulkerBox:
		case BlockType::LightGrayBed:
		case BlockType::LightGrayShulkerBox:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::Lilac:
		case BlockType::LilyOfTheValley:
		case BlockType::LilyPad:
		case BlockType::LimeBed:
		case BlockType::LimeShulkerBox:
		case BlockType::MagentaBed:
		case BlockType::MagentaShulkerBox:
		case BlockType::Melon:
		case BlockType::MelonStem:
		case BlockType::NetherWart:
		case BlockType::NetherWartBlock:
		case BlockType::OakButton:
		case BlockType::OakDoor:
		case BlockType::OakLeaves:
		case BlockType::OakPressurePlate:
		case BlockType::OakSapling:
		case BlockType::OakSign:
		case BlockType::OakTrapdoor:
		case BlockType::OakWallSign:
		case BlockType::OrangeBed:
		case BlockType::OrangeShulkerBox:
		case BlockType::OrangeTulip:
		case BlockType::OxeyeDaisy:
		case BlockType::Peony:
		case BlockType::PinkBed:
		case BlockType::PinkShulkerBox:
		case BlockType::PinkTulip:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::PolishedBlackstoneButton:
		case BlockType::PolishedBlackstonePressurePlate:
		case BlockType::Poppy:
		case BlockType::Potatoes:
		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAllium:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedCrimsonRoots:
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
		case BlockType::Pumpkin:
		case BlockType::PumpkinStem:
		case BlockType::PurpleBed:
		case BlockType::PurpleShulkerBox:
		case BlockType::RedBed:
		case BlockType::RedMushroom:
		case BlockType::RedShulkerBox:
		case BlockType::RedTulip:
		case BlockType::RedstoneTorch:
		case BlockType::RedstoneWallTorch:
		case BlockType::RedstoneWire:
		case BlockType::Repeater:
		case BlockType::RoseBush:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::Snow:
		case BlockType::SpruceButton:
		case BlockType::SpruceDoor:
		case BlockType::SpruceLeaves:
		case BlockType::SprucePressurePlate:
		case BlockType::SpruceSapling:
		case BlockType::SpruceSign:
		case BlockType::SpruceTrapdoor:
		case BlockType::SpruceWallSign:
		case BlockType::StoneButton:
		case BlockType::StonePressurePlate:
		case BlockType::SugarCane:
		case BlockType::Sunflower:
		case BlockType::TallGrass:
		case BlockType::Torch:
		case BlockType::Tripwire:
		case BlockType::TripwireHook:
		case BlockType::Vine:
		case BlockType::VoidAir:
		case BlockType::WallTorch:
		case BlockType::WarpedButton:
		case BlockType::WarpedPressurePlate:
		case BlockType::WarpedSign:
		case BlockType::WarpedTrapdoor:
		case BlockType::WarpedWallSign:
		case BlockType::Water:
		case BlockType::Wheat:
		case BlockType::WhiteBed:
		case BlockType::WhiteShulkerBox:
		case BlockType::WhiteTulip:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
		case BlockType::YellowBed:
		case BlockType::YellowShulkerBox:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:

			return true;
		default: return false;
	}
}





bool cBlockInfo::IsRainBlocker(const BlockState a_Block)
{
	// Blocks that block rain or snow's passage:
	switch (a_Block.Type())
	{
		case BlockType::AcaciaSign:
		case BlockType::AcaciaWallSign:
		case BlockType::BirchSign:
		case BlockType::BirchWallSign:
		case BlockType::BlackBanner:
		case BlockType::BlackWallBanner:
		case BlockType::BlueBanner:
		case BlockType::BlueWallBanner:
		case BlockType::BrownBanner:
		case BlockType::BrownWallBanner:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonWallSign:
		case BlockType::CyanBanner:
		case BlockType::CyanWallBanner:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakWallSign:
		case BlockType::GrayBanner:
		case BlockType::GrayWallBanner:
		case BlockType::GreenBanner:
		case BlockType::GreenWallBanner:
		case BlockType::JungleSign:
		case BlockType::JungleWallSign:
		case BlockType::LightBlueBanner:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LightGrayWallBanner:
		case BlockType::LimeBanner:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaBanner:
		case BlockType::MagentaWallBanner:
		case BlockType::OakSign:
		case BlockType::OakWallSign:
		case BlockType::OrangeBanner:
		case BlockType::OrangeWallBanner:
		case BlockType::PinkBanner:
		case BlockType::PinkWallBanner:
		case BlockType::PurpleBanner:
		case BlockType::PurpleWallBanner:
		case BlockType::RedBanner:
		case BlockType::RedWallBanner:
		case BlockType::SpruceSign:
		case BlockType::SpruceWallSign:
		case BlockType::WarpedSign:
		case BlockType::WarpedWallSign:
		case BlockType::WhiteBanner:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowBanner:
		case BlockType::YellowWallBanner: return true;
		default: return IsSolid(a_Block);
	}
}





bool cBlockInfo::IsSkylightDispersant(BlockState a_Block)
{
	// Skylight dispersant blocks:
	switch (a_Block.Type())
	{
		case BlockType::Cobweb:
		case BlockType::AcaciaLeaves:
		case BlockType::BirchLeaves:
		case BlockType::DarkOakLeaves:
		case BlockType::JungleLeaves:
		case BlockType::OakLeaves:
		case BlockType::SpruceLeaves:
			return true;
		default: return GetSpreadLightFalloff(a_Block) > 1;
	}
}





bool cBlockInfo::IsSnowable(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::AcaciaLeaves:
		case BlockType::BirchLeaves:
		case BlockType::DarkOakLeaves:
		case BlockType::JungleLeaves:
		case BlockType::OakLeaves:
		case BlockType::SpruceLeaves:
		case BlockType::Ice:
			return true;
		case BlockType::PackedIce:
			return false;
		default: return (!IsTransparent(a_Block));
	}
}





bool cBlockInfo::IsSolid(BlockState a_Block)
{
	// Nonsolid blocks:
	switch (a_Block.Type())
	{
		case BlockType::AcaciaButton:
		case BlockType::AcaciaPressurePlate:
		case BlockType::AcaciaSapling:
		case BlockType::AcaciaSign:
		case BlockType::AcaciaWallSign:
		case BlockType::ActivatorRail:
		case BlockType::Air:
		case BlockType::Allium:
		case BlockType::AttachedMelonStem:
		case BlockType::AzureBluet:
		case BlockType::Beetroots:
		case BlockType::BirchButton:
		case BlockType::BirchPressurePlate:
		case BlockType::BirchSapling:
		case BlockType::BirchSign:
		case BlockType::BirchWallSign:
		case BlockType::BlackBanner:
		case BlockType::BlackCarpet:
		case BlockType::BlackWallBanner:
		case BlockType::BlueBanner:
		case BlockType::BlueCarpet:
		case BlockType::BlueOrchid:
		case BlockType::BlueWallBanner:
		case BlockType::BrownBanner:
		case BlockType::BrownCarpet:
		case BlockType::BrownMushroom:
		case BlockType::BrownWallBanner:
		case BlockType::Carrots:
		case BlockType::CaveAir:
		case BlockType::ChorusFlower:
		case BlockType::ChorusPlant:
		case BlockType::Cobweb:
		case BlockType::Comparator:
		case BlockType::Cornflower:
		case BlockType::CrimsonButton:
		case BlockType::CrimsonPressurePlate:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonWallSign:
		case BlockType::CyanBanner:
		case BlockType::CyanCarpet:
		case BlockType::CyanWallBanner:
		case BlockType::Dandelion:
		case BlockType::DarkOakButton:
		case BlockType::DarkOakPressurePlate:
		case BlockType::DarkOakSapling:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakWallSign:
		case BlockType::DeadBush:
		case BlockType::DetectorRail:
		case BlockType::EndGateway:
		case BlockType::EndPortal:
		case BlockType::EndRod:
		case BlockType::Fire:
		case BlockType::GrayBanner:
		case BlockType::GrayCarpet:
		case BlockType::GrayWallBanner:
		case BlockType::GreenBanner:
		case BlockType::GreenCarpet:
		case BlockType::GreenWallBanner:
		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::JungleButton:
		case BlockType::JunglePressurePlate:
		case BlockType::JungleSapling:
		case BlockType::JungleSign:
		case BlockType::JungleWallSign:
		case BlockType::Ladder:
		case BlockType::LargeFern:
		case BlockType::Lava:
		case BlockType::Lever:
		case BlockType::LightBlueBanner:
		case BlockType::LightBlueCarpet:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LightGrayCarpet:
		case BlockType::LightGrayWallBanner:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::Lilac:
		case BlockType::LilyOfTheValley:
		case BlockType::LimeBanner:
		case BlockType::LimeCarpet:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaBanner:
		case BlockType::MagentaCarpet:
		case BlockType::MagentaWallBanner:
		case BlockType::MelonStem:
		case BlockType::NetherPortal:
		case BlockType::OakButton:
		case BlockType::OakPressurePlate:
		case BlockType::OakSapling:
		case BlockType::OakSign:
		case BlockType::OakWallSign:
		case BlockType::OrangeBanner:
		case BlockType::OrangeCarpet:
		case BlockType::OrangeTulip:
		case BlockType::OrangeWallBanner:
		case BlockType::OxeyeDaisy:
		case BlockType::Peony:
		case BlockType::PinkBanner:
		case BlockType::PinkCarpet:
		case BlockType::PinkTulip:
		case BlockType::PinkWallBanner:
		case BlockType::PolishedBlackstoneButton:
		case BlockType::PolishedBlackstonePressurePlate:
		case BlockType::Poppy:
		case BlockType::Potatoes:
		case BlockType::PoweredRail:
		case BlockType::PurpleBanner:
		case BlockType::PurpleCarpet:
		case BlockType::PurpleWallBanner:
		case BlockType::Rail:
		case BlockType::RedBanner:
		case BlockType::RedCarpet:
		case BlockType::RedMushroom:
		case BlockType::RedTulip:
		case BlockType::RedWallBanner:
		case BlockType::RedstoneTorch:
		case BlockType::RedstoneWallTorch:
		case BlockType::RedstoneWire:
		case BlockType::Repeater:
		case BlockType::RoseBush:
		case BlockType::Snow:
		case BlockType::SoulTorch:
		case BlockType::SoulWallTorch:
		case BlockType::SpruceButton:
		case BlockType::SprucePressurePlate:
		case BlockType::SpruceSapling:
		case BlockType::SpruceSign:
		case BlockType::SpruceWallSign:
		case BlockType::StoneButton:
		case BlockType::StonePressurePlate:
		case BlockType::SugarCane:
		case BlockType::Sunflower:
		case BlockType::TallGrass:
		case BlockType::Torch:
		case BlockType::Tripwire:
		case BlockType::TripwireHook:
		case BlockType::Vine:
		case BlockType::VoidAir:
		case BlockType::WallTorch:
		case BlockType::WarpedButton:
		case BlockType::WarpedPressurePlate:
		case BlockType::WarpedSign:
		case BlockType::WarpedWallSign:
		case BlockType::Water:
		case BlockType::Wheat:
		case BlockType::WhiteBanner:
		case BlockType::WhiteCarpet:
		case BlockType::WhiteTulip:
		case BlockType::WhiteWallBanner:
		case BlockType::YellowBanner:
		case BlockType::YellowCarpet:
		case BlockType::YellowWallBanner:
			return false;
		default: return true;
	}
}





bool cBlockInfo::IsTransparent(BlockState a_Block)
{
	// Transparent blocks:
	switch (a_Block.Type())
	{
		case BlockType::AcaciaDoor:
		case BlockType::AcaciaFence:
		case BlockType::AcaciaFenceGate:
		case BlockType::AcaciaLeaves:
		case BlockType::AcaciaSapling:
		case BlockType::AcaciaSign:
		case BlockType::AcaciaStairs:
		case BlockType::AcaciaTrapdoor:
		case BlockType::AcaciaWallSign:
		case BlockType::ActivatorRail:
		case BlockType::Air:
		case BlockType::Allium:
		case BlockType::Anvil:
		case BlockType::AttachedPumpkinStem:
		case BlockType::AzureBluet:
		case BlockType::Barrier:
		case BlockType::Beacon:
		case BlockType::Beetroots:
		case BlockType::BirchDoor:
		case BlockType::BirchFence:
		case BlockType::BirchFenceGate:
		case BlockType::BirchLeaves:
		case BlockType::BirchSapling:
		case BlockType::BirchSign:
		case BlockType::BirchStairs:
		case BlockType::BirchTrapdoor:
		case BlockType::BirchWallSign:
		case BlockType::BlackBanner:
		case BlockType::BlackBed:
		case BlockType::BlackCarpet:
		case BlockType::BlackShulkerBox:
		case BlockType::BlackStainedGlass:
		case BlockType::BlackStainedGlassPane:
		case BlockType::BlackWallBanner:
		case BlockType::BlueBanner:
		case BlockType::BlueBed:
		case BlockType::BlueCarpet:
		case BlockType::BlueOrchid:
		case BlockType::BlueShulkerBox:
		case BlockType::BlueStainedGlass:
		case BlockType::BlueStainedGlassPane:
		case BlockType::BlueWallBanner:
		case BlockType::BrewingStand:
		case BlockType::BrickStairs:
		case BlockType::BrownBanner:
		case BlockType::BrownBed:
		case BlockType::BrownCarpet:
		case BlockType::BrownMushroom:
		case BlockType::BrownShulkerBox:
		case BlockType::BrownStainedGlass:
		case BlockType::BrownStainedGlassPane:
		case BlockType::BrownWallBanner:
		case BlockType::Cactus:
		case BlockType::Cake:
		case BlockType::Carrots:
		case BlockType::Cauldron:
		case BlockType::CaveAir:
		case BlockType::Chest:
		case BlockType::ChorusFlower:
		case BlockType::ChorusPlant:
		case BlockType::ChippedAnvil:
		case BlockType::CobblestoneStairs:
		case BlockType::CobblestoneWall:
		case BlockType::Cobweb:
		case BlockType::Cocoa:
		case BlockType::Comparator:
		case BlockType::Cornflower:
		case BlockType::CreeperHead:
		case BlockType::CreeperWallHead:
		case BlockType::CrimsonSign:
		case BlockType::CrimsonTrapdoor:
		case BlockType::CrimsonWallSign:
		case BlockType::CyanBanner:
		case BlockType::CyanBed:
		case BlockType::CyanCarpet:
		case BlockType::CyanShulkerBox:
		case BlockType::CyanStainedGlass:
		case BlockType::CyanStainedGlassPane:
		case BlockType::CyanWallBanner:
		case BlockType::DamagedAnvil:
		case BlockType::Dandelion:
		case BlockType::DarkOakDoor:
		case BlockType::DarkOakFence:
		case BlockType::DarkOakFenceGate:
		case BlockType::DarkOakLeaves:
		case BlockType::DarkOakSapling:
		case BlockType::DarkOakSign:
		case BlockType::DarkOakStairs:
		case BlockType::DarkOakTrapdoor:
		case BlockType::DarkOakWallSign:
		case BlockType::DaylightDetector:
		case BlockType::DeadBush:
		case BlockType::DetectorRail:
		case BlockType::DragonEgg:
		case BlockType::DragonHead:
		case BlockType::DragonWallHead:
		case BlockType::EnchantingTable:
		case BlockType::EndGateway:
		case BlockType::EndPortal:
		case BlockType::EndPortalFrame:
		case BlockType::EndRod:
		case BlockType::EnderChest:
		case BlockType::Farmland:
		case BlockType::FrostedIce:
		case BlockType::Fire:
		case BlockType::Glass:
		case BlockType::GlassPane:
		case BlockType::Glowstone:
		case BlockType::GrayBanner:
		case BlockType::GrayBed:
		case BlockType::GrayCarpet:
		case BlockType::GrayShulkerBox:
		case BlockType::GrayStainedGlass:
		case BlockType::GrayStainedGlassPane:
		case BlockType::GrayWallBanner:
		case BlockType::ShortGrass:
		case BlockType::DirtPath:
		case BlockType::GreenBanner:
		case BlockType::GreenBed:
		case BlockType::GreenCarpet:
		case BlockType::GreenShulkerBox:
		case BlockType::GreenStainedGlass:
		case BlockType::GreenStainedGlassPane:
		case BlockType::GreenWallBanner:
		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::Hopper:
		case BlockType::Ice:
		case BlockType::IronBars:
		case BlockType::IronDoor:
		case BlockType::IronTrapdoor:
		case BlockType::JackOLantern:
		case BlockType::JungleDoor:
		case BlockType::JungleFence:
		case BlockType::JungleFenceGate:
		case BlockType::JungleLeaves:
		case BlockType::JungleSapling:
		case BlockType::JungleSign:
		case BlockType::JungleStairs:
		case BlockType::JungleTrapdoor:
		case BlockType::JungleWallSign:
		case BlockType::Ladder:
		case BlockType::LargeFern:
		case BlockType::Lava:
		case BlockType::Lever:
		case BlockType::LightBlueBanner:
		case BlockType::LightBlueBed:
		case BlockType::LightBlueCarpet:
		case BlockType::LightBlueShulkerBox:
		case BlockType::LightBlueStainedGlass:
		case BlockType::LightBlueStainedGlassPane:
		case BlockType::LightBlueWallBanner:
		case BlockType::LightGrayBanner:
		case BlockType::LightGrayBed:
		case BlockType::LightGrayCarpet:
		case BlockType::LightGrayShulkerBox:
		case BlockType::LightGrayStainedGlass:
		case BlockType::LightGrayStainedGlassPane:
		case BlockType::LightGrayWallBanner:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::Lilac:
		case BlockType::LilyOfTheValley:
		case BlockType::LilyPad:
		case BlockType::LimeBanner:
		case BlockType::LimeBed:
		case BlockType::LimeCarpet:
		case BlockType::LimeShulkerBox:
		case BlockType::LimeStainedGlass:
		case BlockType::LimeStainedGlassPane:
		case BlockType::LimeWallBanner:
		case BlockType::MagentaBanner:
		case BlockType::MagentaBed:
		case BlockType::MagentaCarpet:
		case BlockType::MagentaShulkerBox:
		case BlockType::MagentaStainedGlass:
		case BlockType::MagentaStainedGlassPane:
		case BlockType::MagentaWallBanner:
		case BlockType::MelonStem:
		case BlockType::MovingPiston:
		case BlockType::NetherBrickFence:
		case BlockType::NetherBrickStairs:
		case BlockType::NetherPortal:
		case BlockType::NetherWart:
		case BlockType::OakButton:
		case BlockType::OakDoor:
		case BlockType::OakFence:
		case BlockType::OakFenceGate:
		case BlockType::OakLeaves:
		case BlockType::OakPressurePlate:
		case BlockType::OakSapling:
		case BlockType::OakSign:
		case BlockType::OakSlab:
		case BlockType::OakStairs:
		case BlockType::OakTrapdoor:
		case BlockType::OakWallSign:
		case BlockType::Observer:
		case BlockType::OrangeBanner:
		case BlockType::OrangeBed:
		case BlockType::OrangeCarpet:
		case BlockType::OrangeShulkerBox:
		case BlockType::OrangeStainedGlass:
		case BlockType::OrangeStainedGlassPane:
		case BlockType::OrangeTulip:
		case BlockType::OrangeWallBanner:
		case BlockType::OxeyeDaisy:
		case BlockType::Peony:
		case BlockType::PinkBanner:
		case BlockType::PinkBed:
		case BlockType::PinkCarpet:
		case BlockType::PinkShulkerBox:
		case BlockType::PinkStainedGlass:
		case BlockType::PinkStainedGlassPane:
		case BlockType::PinkTulip:
		case BlockType::PinkWallBanner:
		case BlockType::Piston:
		case BlockType::PistonHead:
		case BlockType::PlayerHead:
		case BlockType::PlayerWallHead:
		case BlockType::Poppy:
		case BlockType::Potatoes:
		case BlockType::PottedAcaciaSapling:
		case BlockType::PottedAllium:
		case BlockType::PottedAzureBluet:
		case BlockType::PottedBamboo:
		case BlockType::PottedBirchSapling:
		case BlockType::PottedBlueOrchid:
		case BlockType::PottedBrownMushroom:
		case BlockType::PottedCactus:
		case BlockType::PottedCornflower:
		case BlockType::PottedCrimsonFungus:
		case BlockType::PottedCrimsonRoots:
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
		case BlockType::PoweredRail:
		case BlockType::PumpkinStem:
		case BlockType::PurpleBanner:
		case BlockType::PurpleBed:
		case BlockType::PurpleCarpet:
		case BlockType::PurpleShulkerBox:
		case BlockType::PurpleStainedGlass:
		case BlockType::PurpleStainedGlassPane:
		case BlockType::PurpleWallBanner:
		case BlockType::PurpurSlab:
		case BlockType::PurpurStairs:
		case BlockType::QuartzStairs:
		case BlockType::Rail:
		case BlockType::RedBanner:
		case BlockType::RedBed:
		case BlockType::RedCarpet:
		case BlockType::RedMushroom:
		case BlockType::RedSandstoneSlab:
		case BlockType::RedSandstoneStairs:
		case BlockType::RedShulkerBox:
		case BlockType::RedStainedGlass:
		case BlockType::RedStainedGlassPane:
		case BlockType::RedTulip:
		case BlockType::RedWallBanner:
		case BlockType::RedstoneBlock:
		case BlockType::RedstoneOre:
		case BlockType::RedstoneTorch:
		case BlockType::RedstoneWallTorch:
		case BlockType::RedstoneWire:
		case BlockType::Repeater:
		case BlockType::RoseBush:
		case BlockType::SandstoneStairs:
		case BlockType::SeaLantern:
		case BlockType::SkeletonSkull:
		case BlockType::SkeletonWallSkull:
		case BlockType::Snow:
		case BlockType::Spawner:
		case BlockType::SpruceDoor:
		case BlockType::SpruceFence:
		case BlockType::SpruceFenceGate:
		case BlockType::SpruceLeaves:
		case BlockType::SpruceSapling:
		case BlockType::SpruceSign:
		case BlockType::SpruceStairs:
		case BlockType::SpruceTrapdoor:
		case BlockType::SpruceWallSign:
		case BlockType::StickyPiston:
		case BlockType::StoneBrickStairs:
		case BlockType::StoneButton:
		case BlockType::StonePressurePlate:
		case BlockType::StoneSlab:
		case BlockType::SugarCane:
		case BlockType::Sunflower:
		case BlockType::TallGrass:
		case BlockType::Tnt:
		case BlockType::Torch:
		case BlockType::TrappedChest:
		case BlockType::Tripwire:
		case BlockType::TripwireHook:
		case BlockType::Vine:
		case BlockType::VoidAir:
		case BlockType::WallTorch:
		case BlockType::WarpedSign:
		case BlockType::WarpedTrapdoor:
		case BlockType::WarpedWallSign:
		case BlockType::Water:
		case BlockType::Wheat:
		case BlockType::WhiteBanner:
		case BlockType::WhiteBed:
		case BlockType::WhiteCarpet:
		case BlockType::WhiteShulkerBox:
		case BlockType::WhiteStainedGlass:
		case BlockType::WhiteStainedGlassPane:
		case BlockType::WhiteTulip:
		case BlockType::WhiteWallBanner:
		case BlockType::WitherSkeletonSkull:
		case BlockType::WitherSkeletonWallSkull:
		case BlockType::YellowBanner:
		case BlockType::YellowBed:
		case BlockType::YellowCarpet:
		case BlockType::YellowShulkerBox:
		case BlockType::YellowStainedGlass:
		case BlockType::YellowStainedGlassPane:
		case BlockType::YellowWallBanner:
		case BlockType::ZombieHead:
		case BlockType::ZombieWallHead:
			return true;
		default: return false;
	}
}





bool cBlockInfo::IsUseableBySpectator(BlockState a_Block)
{
	// Blocks, which a spectator is allowed to interact with:
	switch (a_Block.Type())
	{
		case BlockType::Beacon:
		case BlockType::BrewingStand:
		case BlockType::Chest:
		case BlockType::Dispenser:
		case BlockType::Dropper:
		case BlockType::Hopper: return true;
		default: return false;
	}
}





float cBlockInfo::GetBlockHeight(BlockState a_Block)
{
	if (cBlockSlabHandler::IsAnySlabType(a_Block))
	{
		if (cBlockSlabHandler::IsSlabFull(a_Block))
		{
			return 1.0;
		}
		if (cBlockSlabHandler::IsSlabTop(a_Block))
		{
			return 1.0f;
		}
		return 0.5f;
	}

	// Block heights:
	switch (a_Block.Type())
	{
		// case E_BLOCK_FARMLAND:         return 0.9375;  // prevents trampling for mobs (#2015) and older clients (MC-85162)
			// Fences
		case BlockType::AcaciaFence:
		case BlockType::BirchFence:
		case BlockType::DarkOakFence:
		case BlockType::JungleFence:
		case BlockType::NetherBrickFence:
		case BlockType::OakFence:
		case BlockType::SpruceFence:
		case BlockType::WarpedFence:
			// Fence Gates
		case BlockType::AcaciaFenceGate:
		case BlockType::BirchFenceGate:
		case BlockType::CrimsonFenceGate:
		case BlockType::DarkOakFenceGate:
		case BlockType::JungleFenceGate:
		case BlockType::OakFenceGate:
		case BlockType::SpruceFenceGate:
		case BlockType::WarpedFenceGate:
			return 1.5;
			// Beds
		case BlockType::BlackBed:
		case BlockType::BlueBed:
		case BlockType::BrownBed:
		case BlockType::CyanBed:
		case BlockType::GrayBed:
		case BlockType::GreenBed:
		case BlockType::LightBlueBed:
		case BlockType::LightGrayBed:
		case BlockType::LimeBed:
		case BlockType::MagentaBed:
		case BlockType::OrangeBed:
		case BlockType::PinkBed:
		case BlockType::PurpleBed:
		case BlockType::RedBed:
		case BlockType::WhiteBed:
		case BlockType::YellowBed:
			return 0.5625;  // 9 pixels
		case BlockType::Cake:            return 0.5;     // 8 pixels
		case BlockType::EnchantingTable: return 0.75;    // 12 pixels
		case BlockType::Snow:            return 0.125;   // one layer is 1 / 8 (2 pixels) tall
		default:                         return 1;
	}
}





float cBlockInfo::GetHardness(BlockState a_Block)
{
	// source: https://minecraft.fandom.com/wiki/Module:Hardness_values
	// Block hardness:
	switch (a_Block.Type())
	{
		case BlockType::Air:  return 0.000000f;
		case BlockType::Stone:  return 1.500000f;
		case BlockType::Granite:  return 1.500000f;
		case BlockType::PolishedGranite:  return 1.500000f;
		case BlockType::Diorite:  return 1.500000f;
		case BlockType::PolishedDiorite:  return 1.500000f;
		case BlockType::Andesite:  return 1.500000f;
		case BlockType::PolishedAndesite:  return 1.500000f;
		case BlockType::GrassBlock:  return 0.600000f;
		case BlockType::Dirt:  return 0.500000f;
		case BlockType::CoarseDirt:  return 0.500000f;
		case BlockType::Podzol:  return 0.500000f;
		case BlockType::Cobblestone:  return 2.000000f;
		case BlockType::OakPlanks:  return 2.000000f;
		case BlockType::SprucePlanks:  return 2.000000f;
		case BlockType::BirchPlanks:  return 2.000000f;
		case BlockType::JunglePlanks:  return 2.000000f;
		case BlockType::AcaciaPlanks:  return 2.000000f;
		case BlockType::CherryPlanks:  return 2.000000f;
		case BlockType::DarkOakPlanks:  return 2.000000f;
		case BlockType::PaleOakWood:  return 2.000000f;
		case BlockType::PaleOakPlanks:  return 2.000000f;
		case BlockType::MangrovePlanks:  return 2.000000f;
		case BlockType::BambooPlanks:  return 2.000000f;
		case BlockType::BambooMosaic:  return 2.000000f;
		case BlockType::OakSapling:  return 0.000000f;
		case BlockType::SpruceSapling:  return 0.000000f;
		case BlockType::BirchSapling:  return 0.000000f;
		case BlockType::JungleSapling:  return 0.000000f;
		case BlockType::AcaciaSapling:  return 0.000000f;
		case BlockType::CherrySapling:  return 0.000000f;
		case BlockType::DarkOakSapling:  return 0.000000f;
		case BlockType::PaleOakSapling:  return 0.000000f;
		case BlockType::MangrovePropagule:  return 0.000000f;
		case BlockType::Bedrock:  return -1.000000f;
		case BlockType::Water:  return 100.000000f;
		case BlockType::Lava:  return 100.000000f;
		case BlockType::Sand:  return 0.500000f;
		case BlockType::SuspiciousSand:  return 0.250000f;
		case BlockType::RedSand:  return 0.500000f;
		case BlockType::Gravel:  return 0.600000f;
		case BlockType::SuspiciousGravel:  return 0.250000f;
		case BlockType::GoldOre:  return 3.000000f;
		case BlockType::DeepslateGoldOre:  return 4.500000f;
		case BlockType::IronOre:  return 3.000000f;
		case BlockType::DeepslateIronOre:  return 4.500000f;
		case BlockType::CoalOre:  return 3.000000f;
		case BlockType::DeepslateCoalOre:  return 4.500000f;
		case BlockType::NetherGoldOre:  return 3.000000f;
		case BlockType::OakLog:  return 2.000000f;
		case BlockType::SpruceLog:  return 2.000000f;
		case BlockType::BirchLog:  return 2.000000f;
		case BlockType::JungleLog:  return 2.000000f;
		case BlockType::AcaciaLog:  return 2.000000f;
		case BlockType::CherryLog:  return 2.000000f;
		case BlockType::DarkOakLog:  return 2.000000f;
		case BlockType::PaleOakLog:  return 2.000000f;
		case BlockType::MangroveLog:  return 2.000000f;
		case BlockType::MangroveRoots:  return 0.700000f;
		case BlockType::MuddyMangroveRoots:  return 0.700000f;
		case BlockType::BambooBlock:  return 2.000000f;
		case BlockType::StrippedSpruceLog:  return 2.000000f;
		case BlockType::StrippedBirchLog:  return 2.000000f;
		case BlockType::StrippedJungleLog:  return 2.000000f;
		case BlockType::StrippedAcaciaLog:  return 2.000000f;
		case BlockType::StrippedCherryLog:  return 2.000000f;
		case BlockType::StrippedDarkOakLog:  return 2.000000f;
		case BlockType::StrippedPaleOakLog:  return 2.000000f;
		case BlockType::StrippedOakLog:  return 2.000000f;
		case BlockType::StrippedMangroveLog:  return 2.000000f;
		case BlockType::StrippedBambooBlock:  return 2.000000f;
		case BlockType::OakWood:  return 2.000000f;
		case BlockType::SpruceWood:  return 2.000000f;
		case BlockType::BirchWood:  return 2.000000f;
		case BlockType::JungleWood:  return 2.000000f;
		case BlockType::AcaciaWood:  return 2.000000f;
		case BlockType::CherryWood:  return 2.000000f;
		case BlockType::DarkOakWood:  return 2.000000f;
		case BlockType::MangroveWood:  return 2.000000f;
		case BlockType::StrippedOakWood:  return 2.000000f;
		case BlockType::StrippedSpruceWood:  return 2.000000f;
		case BlockType::StrippedBirchWood:  return 2.000000f;
		case BlockType::StrippedJungleWood:  return 2.000000f;
		case BlockType::StrippedAcaciaWood:  return 2.000000f;
		case BlockType::StrippedCherryWood:  return 2.000000f;
		case BlockType::StrippedDarkOakWood:  return 2.000000f;
		case BlockType::StrippedPaleOakWood:  return 2.000000f;
		case BlockType::StrippedMangroveWood:  return 2.000000f;
		case BlockType::OakLeaves:  return 0.200000f;
		case BlockType::SpruceLeaves:  return 0.200000f;
		case BlockType::BirchLeaves:  return 0.200000f;
		case BlockType::JungleLeaves:  return 0.200000f;
		case BlockType::AcaciaLeaves:  return 0.200000f;
		case BlockType::CherryLeaves:  return 0.200000f;
		case BlockType::DarkOakLeaves:  return 0.200000f;
		case BlockType::PaleOakLeaves:  return 0.200000f;
		case BlockType::MangroveLeaves:  return 0.200000f;
		case BlockType::AzaleaLeaves:  return 0.200000f;
		case BlockType::FloweringAzaleaLeaves:  return 0.200000f;
		case BlockType::Sponge:  return 0.600000f;
		case BlockType::WetSponge:  return 0.600000f;
		case BlockType::Glass:  return 0.300000f;
		case BlockType::LapisOre:  return 3.000000f;
		case BlockType::DeepslateLapisOre:  return 4.500000f;
		case BlockType::LapisBlock:  return 3.000000f;
		case BlockType::Dispenser:  return 3.500000f;
		case BlockType::Sandstone:  return 0.800000f;
		case BlockType::ChiseledSandstone:  return 0.800000f;
		case BlockType::CutSandstone:  return 0.800000f;
		case BlockType::NoteBlock:  return 0.800000f;
		case BlockType::WhiteBed:  return 0.200000f;
		case BlockType::OrangeBed:  return 0.200000f;
		case BlockType::MagentaBed:  return 0.200000f;
		case BlockType::LightBlueBed:  return 0.200000f;
		case BlockType::YellowBed:  return 0.200000f;
		case BlockType::LimeBed:  return 0.200000f;
		case BlockType::PinkBed:  return 0.200000f;
		case BlockType::GrayBed:  return 0.200000f;
		case BlockType::LightGrayBed:  return 0.200000f;
		case BlockType::CyanBed:  return 0.200000f;
		case BlockType::PurpleBed:  return 0.200000f;
		case BlockType::BlueBed:  return 0.200000f;
		case BlockType::BrownBed:  return 0.200000f;
		case BlockType::GreenBed:  return 0.200000f;
		case BlockType::RedBed:  return 0.200000f;
		case BlockType::BlackBed:  return 0.200000f;
		case BlockType::PoweredRail:  return 0.700000f;
		case BlockType::DetectorRail:  return 0.700000f;
		case BlockType::StickyPiston:  return 1.500000f;
		case BlockType::Cobweb:  return 4.000000f;
		case BlockType::ShortGrass:  return 0.000000f;
		case BlockType::Fern:  return 0.000000f;
		case BlockType::DeadBush:  return 0.000000f;
		case BlockType::Seagrass:  return 0.000000f;
		case BlockType::TallSeagrass:  return 0.000000f;
		case BlockType::Piston:  return 1.500000f;
		case BlockType::PistonHead:  return 1.500000f;
		case BlockType::WhiteWool:  return 0.800000f;
		case BlockType::OrangeWool:  return 0.800000f;
		case BlockType::MagentaWool:  return 0.800000f;
		case BlockType::LightBlueWool:  return 0.800000f;
		case BlockType::YellowWool:  return 0.800000f;
		case BlockType::LimeWool:  return 0.800000f;
		case BlockType::PinkWool:  return 0.800000f;
		case BlockType::GrayWool:  return 0.800000f;
		case BlockType::LightGrayWool:  return 0.800000f;
		case BlockType::CyanWool:  return 0.800000f;
		case BlockType::PurpleWool:  return 0.800000f;
		case BlockType::BlueWool:  return 0.800000f;
		case BlockType::BrownWool:  return 0.800000f;
		case BlockType::GreenWool:  return 0.800000f;
		case BlockType::RedWool:  return 0.800000f;
		case BlockType::BlackWool:  return 0.800000f;
		case BlockType::MovingPiston:  return -1.000000f;
		case BlockType::Dandelion:  return 0.000000f;
		case BlockType::Torchflower:  return 0.000000f;
		case BlockType::Poppy:  return 0.000000f;
		case BlockType::BlueOrchid:  return 0.000000f;
		case BlockType::Allium:  return 0.000000f;
		case BlockType::AzureBluet:  return 0.000000f;
		case BlockType::RedTulip:  return 0.000000f;
		case BlockType::OrangeTulip:  return 0.000000f;
		case BlockType::WhiteTulip:  return 0.000000f;
		case BlockType::PinkTulip:  return 0.000000f;
		case BlockType::OxeyeDaisy:  return 0.000000f;
		case BlockType::Cornflower:  return 0.000000f;
		case BlockType::WitherRose:  return 0.000000f;
		case BlockType::LilyOfTheValley:  return 0.000000f;
		case BlockType::BrownMushroom:  return 0.000000f;
		case BlockType::RedMushroom:  return 0.000000f;
		case BlockType::GoldBlock:  return 3.000000f;
		case BlockType::IronBlock:  return 5.000000f;
		case BlockType::Bricks:  return 2.000000f;
		case BlockType::Tnt:  return 0.000000f;
		case BlockType::Bookshelf:  return 1.500000f;
		case BlockType::ChiseledBookshelf:  return 1.500000f;
		case BlockType::MossyCobblestone:  return 2.000000f;
		case BlockType::Obsidian:  return 50.000000f;
		case BlockType::Torch:  return 0.000000f;
		case BlockType::WallTorch:  return 0.000000f;
		case BlockType::Fire:  return 0.000000f;
		case BlockType::SoulFire:  return 0.000000f;
		case BlockType::Spawner:  return 5.000000f;
		case BlockType::CreakingHeart:  return 10.000000f;
		case BlockType::OakStairs:  return 2.000000f;
		case BlockType::Chest:  return 2.500000f;
		case BlockType::RedstoneWire:  return 0.000000f;
		case BlockType::DiamondOre:  return 3.000000f;
		case BlockType::DeepslateDiamondOre:  return 4.500000f;
		case BlockType::DiamondBlock:  return 5.000000f;
		case BlockType::CraftingTable:  return 2.500000f;
		case BlockType::Wheat:  return 0.000000f;
		case BlockType::Farmland:  return 0.600000f;
		case BlockType::Furnace:  return 3.500000f;
		case BlockType::OakSign:  return 1.000000f;
		case BlockType::SpruceSign:  return 1.000000f;
		case BlockType::BirchSign:  return 1.000000f;
		case BlockType::AcaciaSign:  return 1.000000f;
		case BlockType::CherrySign:  return 1.000000f;
		case BlockType::JungleSign:  return 1.000000f;
		case BlockType::DarkOakSign:  return 1.000000f;
		case BlockType::PaleOakSign:  return 1.000000f;
		case BlockType::MangroveSign:  return 1.000000f;
		case BlockType::BambooSign:  return 1.000000f;
		case BlockType::OakDoor:  return 3.000000f;
		case BlockType::Ladder:  return 0.400000f;
		case BlockType::Rail:  return 0.700000f;
		case BlockType::CobblestoneStairs:  return 2.000000f;
		case BlockType::OakWallSign:  return 1.000000f;
		case BlockType::SpruceWallSign:  return 1.000000f;
		case BlockType::BirchWallSign:  return 1.000000f;
		case BlockType::AcaciaWallSign:  return 1.000000f;
		case BlockType::CherryWallSign:  return 1.000000f;
		case BlockType::JungleWallSign:  return 1.000000f;
		case BlockType::DarkOakWallSign:  return 1.000000f;
		case BlockType::PaleOakWallSign:  return 1.000000f;
		case BlockType::MangroveWallSign:  return 1.000000f;
		case BlockType::BambooWallSign:  return 1.000000f;
		case BlockType::OakHangingSign:  return 1.000000f;
		case BlockType::SpruceHangingSign:  return 1.000000f;
		case BlockType::BirchHangingSign:  return 1.000000f;
		case BlockType::AcaciaHangingSign:  return 1.000000f;
		case BlockType::CherryHangingSign:  return 1.000000f;
		case BlockType::JungleHangingSign:  return 1.000000f;
		case BlockType::DarkOakHangingSign:  return 1.000000f;
		case BlockType::PaleOakHangingSign:  return 1.000000f;
		case BlockType::CrimsonHangingSign:  return 1.000000f;
		case BlockType::WarpedHangingSign:  return 1.000000f;
		case BlockType::MangroveHangingSign:  return 1.000000f;
		case BlockType::BambooHangingSign:  return 1.000000f;
		case BlockType::OakWallHangingSign:  return 1.000000f;
		case BlockType::SpruceWallHangingSign:  return 1.000000f;
		case BlockType::BirchWallHangingSign:  return 1.000000f;
		case BlockType::AcaciaWallHangingSign:  return 1.000000f;
		case BlockType::CherryWallHangingSign:  return 1.000000f;
		case BlockType::JungleWallHangingSign:  return 1.000000f;
		case BlockType::DarkOakWallHangingSign:  return 1.000000f;
		case BlockType::PaleOakWallHangingSign:  return 1.000000f;
		case BlockType::MangroveWallHangingSign:  return 1.000000f;
		case BlockType::CrimsonWallHangingSign:  return 1.000000f;
		case BlockType::WarpedWallHangingSign:  return 1.000000f;
		case BlockType::BambooWallHangingSign:  return 1.000000f;
		case BlockType::Lever:  return 0.500000f;
		case BlockType::StonePressurePlate:  return 0.500000f;
		case BlockType::IronDoor:  return 5.000000f;
		case BlockType::OakPressurePlate:  return 0.500000f;
		case BlockType::SprucePressurePlate:  return 0.500000f;
		case BlockType::BirchPressurePlate:  return 0.500000f;
		case BlockType::JunglePressurePlate:  return 0.500000f;
		case BlockType::AcaciaPressurePlate:  return 0.500000f;
		case BlockType::CherryPressurePlate:  return 0.500000f;
		case BlockType::DarkOakPressurePlate:  return 0.500000f;
		case BlockType::PaleOakPressurePlate:  return 0.500000f;
		case BlockType::MangrovePressurePlate:  return 0.500000f;
		case BlockType::BambooPressurePlate:  return 0.500000f;
		case BlockType::RedstoneOre:  return 3.000000f;
		case BlockType::DeepslateRedstoneOre:  return 4.500000f;
		case BlockType::RedstoneTorch:  return 0.000000f;
		case BlockType::RedstoneWallTorch:  return 0.000000f;
		case BlockType::StoneButton:  return 0.500000f;
		case BlockType::Snow:  return 0.100000f;
		case BlockType::Ice:  return 0.500000f;
		case BlockType::SnowBlock:  return 0.200000f;
		case BlockType::Cactus:  return 0.400000f;
		case BlockType::Clay:  return 0.600000f;
		case BlockType::SugarCane:  return 0.000000f;
		case BlockType::Jukebox:  return 2.000000f;
		case BlockType::OakFence:  return 2.000000f;
		case BlockType::Netherrack:  return 0.400000f;
		case BlockType::SoulSand:  return 0.500000f;
		case BlockType::SoulSoil:  return 0.500000f;
		case BlockType::Basalt:  return 1.250000f;
		case BlockType::PolishedBasalt:  return 1.250000f;
		case BlockType::SoulTorch:  return 0.000000f;
		case BlockType::SoulWallTorch:  return 0.000000f;
		case BlockType::Glowstone:  return 0.300000f;
		case BlockType::NetherPortal:  return -1.000000f;
		case BlockType::CarvedPumpkin:  return 1.000000f;
		case BlockType::JackOLantern:  return 1.000000f;
		case BlockType::Cake:  return 0.500000f;
		case BlockType::Repeater:  return 0.000000f;
		case BlockType::WhiteStainedGlass:  return 0.300000f;
		case BlockType::OrangeStainedGlass:  return 0.300000f;
		case BlockType::MagentaStainedGlass:  return 0.300000f;
		case BlockType::LightBlueStainedGlass:  return 0.300000f;
		case BlockType::YellowStainedGlass:  return 0.300000f;
		case BlockType::LimeStainedGlass:  return 0.300000f;
		case BlockType::PinkStainedGlass:  return 0.300000f;
		case BlockType::GrayStainedGlass:  return 0.300000f;
		case BlockType::LightGrayStainedGlass:  return 0.300000f;
		case BlockType::CyanStainedGlass:  return 0.300000f;
		case BlockType::PurpleStainedGlass:  return 0.300000f;
		case BlockType::BlueStainedGlass:  return 0.300000f;
		case BlockType::BrownStainedGlass:  return 0.300000f;
		case BlockType::GreenStainedGlass:  return 0.300000f;
		case BlockType::RedStainedGlass:  return 0.300000f;
		case BlockType::BlackStainedGlass:  return 0.300000f;
		case BlockType::OakTrapdoor:  return 3.000000f;
		case BlockType::SpruceTrapdoor:  return 3.000000f;
		case BlockType::BirchTrapdoor:  return 3.000000f;
		case BlockType::JungleTrapdoor:  return 3.000000f;
		case BlockType::AcaciaTrapdoor:  return 3.000000f;
		case BlockType::CherryTrapdoor:  return 3.000000f;
		case BlockType::DarkOakTrapdoor:  return 3.000000f;
		case BlockType::PaleOakTrapdoor:  return 3.000000f;
		case BlockType::MangroveTrapdoor:  return 3.000000f;
		case BlockType::BambooTrapdoor:  return 3.000000f;
		case BlockType::StoneBricks:  return 1.500000f;
		case BlockType::MossyStoneBricks:  return 1.500000f;
		case BlockType::CrackedStoneBricks:  return 1.500000f;
		case BlockType::ChiseledStoneBricks:  return 1.500000f;
		case BlockType::PackedMud:  return 1.000000f;
		case BlockType::MudBricks:  return 1.500000f;
		case BlockType::InfestedStone:  return 0.750000f;
		case BlockType::InfestedCobblestone:  return 1.000000f;
		case BlockType::InfestedStoneBricks:  return 0.750000f;
		case BlockType::InfestedMossyStoneBricks:  return 0.750000f;
		case BlockType::InfestedCrackedStoneBricks:  return 0.750000f;
		case BlockType::InfestedChiseledStoneBricks:  return 0.750000f;
		case BlockType::BrownMushroomBlock:  return 0.200000f;
		case BlockType::RedMushroomBlock:  return 0.200000f;
		case BlockType::MushroomStem:  return 0.200000f;
		case BlockType::IronBars:  return 5.000000f;
		case BlockType::Chain:  return 5.000000f;
		case BlockType::GlassPane:  return 0.300000f;
		case BlockType::Pumpkin:  return 1.000000f;
		case BlockType::Melon:  return 1.000000f;
		case BlockType::AttachedPumpkinStem:  return 0.000000f;
		case BlockType::AttachedMelonStem:  return 0.000000f;
		case BlockType::PumpkinStem:  return 0.000000f;
		case BlockType::MelonStem:  return 0.000000f;
		case BlockType::Vine:  return 0.200000f;
		case BlockType::GlowLichen:  return 0.200000f;
		case BlockType::ResinClump:  return 0.000000f;
		case BlockType::OakFenceGate:  return 2.000000f;
		case BlockType::BrickStairs:  return 2.000000f;
		case BlockType::StoneBrickStairs:  return 1.500000f;
		case BlockType::MudBrickStairs:  return 1.500000f;
		case BlockType::Mycelium:  return 0.600000f;
		case BlockType::LilyPad:  return 0.000000f;
		case BlockType::ResinBlock:  return 0.000000f;
		case BlockType::ResinBricks:  return 1.500000f;
		case BlockType::ResinBrickStairs:  return 1.500000f;
		case BlockType::ResinBrickSlab:  return 1.500000f;
		case BlockType::ResinBrickWall:  return 1.500000f;
		case BlockType::ChiseledResinBricks:  return 1.500000f;
		case BlockType::NetherBricks:  return 2.000000f;
		case BlockType::NetherBrickFence:  return 2.000000f;
		case BlockType::NetherBrickStairs:  return 2.000000f;
		case BlockType::NetherWart:  return 0.000000f;
		case BlockType::EnchantingTable:  return 5.000000f;
		case BlockType::BrewingStand:  return 0.500000f;
		case BlockType::Cauldron:  return 2.000000f;
		case BlockType::WaterCauldron:  return 2.000000f;
		case BlockType::LavaCauldron:  return 2.000000f;
		case BlockType::PowderSnowCauldron:  return 2.000000f;
		case BlockType::EndPortal:  return -1.000000f;
		case BlockType::EndPortalFrame:  return -1.000000f;
		case BlockType::EndStone:  return 3.000000f;
		case BlockType::DragonEgg:  return 3.000000f;
		case BlockType::RedstoneLamp:  return 0.300000f;
		case BlockType::Cocoa:  return 0.200000f;
		case BlockType::SandstoneStairs:  return 0.800000f;
		case BlockType::EmeraldOre:  return 3.000000f;
		case BlockType::DeepslateEmeraldOre:  return 4.500000f;
		case BlockType::EnderChest:  return 22.500000f;
		case BlockType::TripwireHook:  return 0.000000f;
		case BlockType::Tripwire:  return 0.000000f;
		case BlockType::EmeraldBlock:  return 5.000000f;
		case BlockType::SpruceStairs:  return 2.000000f;
		case BlockType::BirchStairs:  return 2.000000f;
		case BlockType::JungleStairs:  return 2.000000f;
		case BlockType::CommandBlock:  return -1.000000f;
		case BlockType::Beacon:  return 3.000000f;
		case BlockType::CobblestoneWall:  return 2.000000f;
		case BlockType::MossyCobblestoneWall:  return 2.000000f;
		case BlockType::FlowerPot:  return 0.000000f;
		case BlockType::PottedTorchflower:  return 0.000000f;
		case BlockType::PottedOakSapling:  return 0.000000f;
		case BlockType::PottedSpruceSapling:  return 0.000000f;
		case BlockType::PottedBirchSapling:  return 0.000000f;
		case BlockType::PottedJungleSapling:  return 0.000000f;
		case BlockType::PottedAcaciaSapling:  return 0.000000f;
		case BlockType::PottedCherrySapling:  return 0.000000f;
		case BlockType::PottedDarkOakSapling:  return 0.000000f;
		case BlockType::PottedPaleOakSapling:  return 0.000000f;
		case BlockType::PottedMangrovePropagule:  return 0.000000f;
		case BlockType::PottedFern:  return 0.000000f;
		case BlockType::PottedDandelion:  return 0.000000f;
		case BlockType::PottedPoppy:  return 0.000000f;
		case BlockType::PottedBlueOrchid:  return 0.000000f;
		case BlockType::PottedAllium:  return 0.000000f;
		case BlockType::PottedAzureBluet:  return 0.000000f;
		case BlockType::PottedRedTulip:  return 0.000000f;
		case BlockType::PottedOrangeTulip:  return 0.000000f;
		case BlockType::PottedWhiteTulip:  return 0.000000f;
		case BlockType::PottedPinkTulip:  return 0.000000f;
		case BlockType::PottedOxeyeDaisy:  return 0.000000f;
		case BlockType::PottedCornflower:  return 0.000000f;
		case BlockType::PottedLilyOfTheValley:  return 0.000000f;
		case BlockType::PottedWitherRose:  return 0.000000f;
		case BlockType::PottedRedMushroom:  return 0.000000f;
		case BlockType::PottedBrownMushroom:  return 0.000000f;
		case BlockType::PottedDeadBush:  return 0.000000f;
		case BlockType::PottedCactus:  return 0.000000f;
		case BlockType::Carrots:  return 0.000000f;
		case BlockType::Potatoes:  return 0.000000f;
		case BlockType::OakButton:  return 0.500000f;
		case BlockType::SpruceButton:  return 0.500000f;
		case BlockType::BirchButton:  return 0.500000f;
		case BlockType::JungleButton:  return 0.500000f;
		case BlockType::AcaciaButton:  return 0.500000f;
		case BlockType::CherryButton:  return 0.500000f;
		case BlockType::DarkOakButton:  return 0.500000f;
		case BlockType::PaleOakButton:  return 0.500000f;
		case BlockType::MangroveButton:  return 0.500000f;
		case BlockType::BambooButton:  return 0.500000f;
		case BlockType::SkeletonSkull:  return 1.000000f;
		case BlockType::SkeletonWallSkull:  return 1.000000f;
		case BlockType::WitherSkeletonSkull:  return 1.000000f;
		case BlockType::WitherSkeletonWallSkull:  return 1.000000f;
		case BlockType::ZombieHead:  return 1.000000f;
		case BlockType::ZombieWallHead:  return 1.000000f;
		case BlockType::PlayerHead:  return 1.000000f;
		case BlockType::PlayerWallHead:  return 1.000000f;
		case BlockType::CreeperHead:  return 1.000000f;
		case BlockType::CreeperWallHead:  return 1.000000f;
		case BlockType::DragonHead:  return 1.000000f;
		case BlockType::DragonWallHead:  return 1.000000f;
		case BlockType::PiglinHead:  return 1.000000f;
		case BlockType::PiglinWallHead:  return 1.000000f;
		case BlockType::Anvil:  return 5.000000f;
		case BlockType::ChippedAnvil:  return 5.000000f;
		case BlockType::DamagedAnvil:  return 5.000000f;
		case BlockType::TrappedChest:  return 2.500000f;
		case BlockType::LightWeightedPressurePlate:  return 0.500000f;
		case BlockType::HeavyWeightedPressurePlate:  return 0.500000f;
		case BlockType::Comparator:  return 0.000000f;
		case BlockType::DaylightDetector:  return 0.200000f;
		case BlockType::RedstoneBlock:  return 5.000000f;
		case BlockType::NetherQuartzOre:  return 3.000000f;
		case BlockType::Hopper:  return 3.000000f;
		case BlockType::QuartzBlock:  return 0.800000f;
		case BlockType::ChiseledQuartzBlock:  return 0.800000f;
		case BlockType::QuartzPillar:  return 0.800000f;
		case BlockType::QuartzStairs:  return 0.800000f;
		case BlockType::ActivatorRail:  return 0.700000f;
		case BlockType::Dropper:  return 3.500000f;
		case BlockType::WhiteTerracotta:  return 1.250000f;
		case BlockType::OrangeTerracotta:  return 1.250000f;
		case BlockType::MagentaTerracotta:  return 1.250000f;
		case BlockType::LightBlueTerracotta:  return 1.250000f;
		case BlockType::YellowTerracotta:  return 1.250000f;
		case BlockType::LimeTerracotta:  return 1.250000f;
		case BlockType::PinkTerracotta:  return 1.250000f;
		case BlockType::GrayTerracotta:  return 1.250000f;
		case BlockType::LightGrayTerracotta:  return 1.250000f;
		case BlockType::CyanTerracotta:  return 1.250000f;
		case BlockType::PurpleTerracotta:  return 1.250000f;
		case BlockType::BlueTerracotta:  return 1.250000f;
		case BlockType::BrownTerracotta:  return 1.250000f;
		case BlockType::GreenTerracotta:  return 1.250000f;
		case BlockType::RedTerracotta:  return 1.250000f;
		case BlockType::BlackTerracotta:  return 1.250000f;
		case BlockType::WhiteStainedGlassPane:  return 0.300000f;
		case BlockType::OrangeStainedGlassPane:  return 0.300000f;
		case BlockType::MagentaStainedGlassPane:  return 0.300000f;
		case BlockType::LightBlueStainedGlassPane:  return 0.300000f;
		case BlockType::YellowStainedGlassPane:  return 0.300000f;
		case BlockType::LimeStainedGlassPane:  return 0.300000f;
		case BlockType::PinkStainedGlassPane:  return 0.300000f;
		case BlockType::GrayStainedGlassPane:  return 0.300000f;
		case BlockType::LightGrayStainedGlassPane:  return 0.300000f;
		case BlockType::CyanStainedGlassPane:  return 0.300000f;
		case BlockType::PurpleStainedGlassPane:  return 0.300000f;
		case BlockType::BlueStainedGlassPane:  return 0.300000f;
		case BlockType::BrownStainedGlassPane:  return 0.300000f;
		case BlockType::GreenStainedGlassPane:  return 0.300000f;
		case BlockType::RedStainedGlassPane:  return 0.300000f;
		case BlockType::BlackStainedGlassPane:  return 0.300000f;
		case BlockType::AcaciaStairs:  return 2.000000f;
		case BlockType::CherryStairs:  return 2.000000f;
		case BlockType::DarkOakStairs:  return 2.000000f;
		case BlockType::PaleOakStairs:  return 2.000000f;
		case BlockType::MangroveStairs:  return 2.000000f;
		case BlockType::BambooStairs:  return 2.000000f;
		case BlockType::BambooMosaicStairs:  return 2.000000f;
		case BlockType::SlimeBlock:  return 0.000000f;
		case BlockType::Barrier:  return -1.000000f;
		case BlockType::Light:  return -1.000000f;
		case BlockType::IronTrapdoor:  return 5.000000f;
		case BlockType::Prismarine:  return 1.500000f;
		case BlockType::PrismarineBricks:  return 1.500000f;
		case BlockType::DarkPrismarine:  return 1.500000f;
		case BlockType::PrismarineStairs:  return 1.500000f;
		case BlockType::PrismarineBrickStairs:  return 1.500000f;
		case BlockType::DarkPrismarineStairs:  return 1.500000f;
		case BlockType::PrismarineSlab:  return 1.500000f;
		case BlockType::PrismarineBrickSlab:  return 1.500000f;
		case BlockType::DarkPrismarineSlab:  return 1.500000f;
		case BlockType::SeaLantern:  return 0.300000f;
		case BlockType::HayBlock:  return 0.500000f;
		case BlockType::WhiteCarpet:  return 0.100000f;
		case BlockType::OrangeCarpet:  return 0.100000f;
		case BlockType::MagentaCarpet:  return 0.100000f;
		case BlockType::LightBlueCarpet:  return 0.100000f;
		case BlockType::YellowCarpet:  return 0.100000f;
		case BlockType::LimeCarpet:  return 0.100000f;
		case BlockType::PinkCarpet:  return 0.100000f;
		case BlockType::GrayCarpet:  return 0.100000f;
		case BlockType::LightGrayCarpet:  return 0.100000f;
		case BlockType::CyanCarpet:  return 0.100000f;
		case BlockType::PurpleCarpet:  return 0.100000f;
		case BlockType::BlueCarpet:  return 0.100000f;
		case BlockType::BrownCarpet:  return 0.100000f;
		case BlockType::GreenCarpet:  return 0.100000f;
		case BlockType::RedCarpet:  return 0.100000f;
		case BlockType::BlackCarpet:  return 0.100000f;
		case BlockType::Terracotta:  return 1.250000f;
		case BlockType::CoalBlock:  return 5.000000f;
		case BlockType::PackedIce:  return 0.500000f;
		case BlockType::Sunflower:  return 0.000000f;
		case BlockType::Lilac:  return 0.000000f;
		case BlockType::RoseBush:  return 0.000000f;
		case BlockType::Peony:  return 0.000000f;
		case BlockType::TallGrass:  return 0.000000f;
		case BlockType::LargeFern:  return 0.000000f;
		case BlockType::WhiteBanner:  return 1.000000f;
		case BlockType::OrangeBanner:  return 1.000000f;
		case BlockType::MagentaBanner:  return 1.000000f;
		case BlockType::LightBlueBanner:  return 1.000000f;
		case BlockType::YellowBanner:  return 1.000000f;
		case BlockType::LimeBanner:  return 1.000000f;
		case BlockType::PinkBanner:  return 1.000000f;
		case BlockType::GrayBanner:  return 1.000000f;
		case BlockType::LightGrayBanner:  return 1.000000f;
		case BlockType::CyanBanner:  return 1.000000f;
		case BlockType::PurpleBanner:  return 1.000000f;
		case BlockType::BlueBanner:  return 1.000000f;
		case BlockType::BrownBanner:  return 1.000000f;
		case BlockType::GreenBanner:  return 1.000000f;
		case BlockType::RedBanner:  return 1.000000f;
		case BlockType::BlackBanner:  return 1.000000f;
		case BlockType::WhiteWallBanner:  return 1.000000f;
		case BlockType::OrangeWallBanner:  return 1.000000f;
		case BlockType::MagentaWallBanner:  return 1.000000f;
		case BlockType::LightBlueWallBanner:  return 1.000000f;
		case BlockType::YellowWallBanner:  return 1.000000f;
		case BlockType::LimeWallBanner:  return 1.000000f;
		case BlockType::PinkWallBanner:  return 1.000000f;
		case BlockType::GrayWallBanner:  return 1.000000f;
		case BlockType::LightGrayWallBanner:  return 1.000000f;
		case BlockType::CyanWallBanner:  return 1.000000f;
		case BlockType::PurpleWallBanner:  return 1.000000f;
		case BlockType::BlueWallBanner:  return 1.000000f;
		case BlockType::BrownWallBanner:  return 1.000000f;
		case BlockType::GreenWallBanner:  return 1.000000f;
		case BlockType::RedWallBanner:  return 1.000000f;
		case BlockType::BlackWallBanner:  return 1.000000f;
		case BlockType::RedSandstone:  return 0.800000f;
		case BlockType::ChiseledRedSandstone:  return 0.800000f;
		case BlockType::CutRedSandstone:  return 0.800000f;
		case BlockType::RedSandstoneStairs:  return 0.800000f;
		case BlockType::OakSlab:  return 2.000000f;
		case BlockType::SpruceSlab:  return 2.000000f;
		case BlockType::BirchSlab:  return 2.000000f;
		case BlockType::JungleSlab:  return 2.000000f;
		case BlockType::AcaciaSlab:  return 2.000000f;
		case BlockType::CherrySlab:  return 2.000000f;
		case BlockType::DarkOakSlab:  return 2.000000f;
		case BlockType::PaleOakSlab:  return 2.000000f;
		case BlockType::MangroveSlab:  return 2.000000f;
		case BlockType::BambooSlab:  return 2.000000f;
		case BlockType::BambooMosaicSlab:  return 2.000000f;
		case BlockType::StoneSlab:  return 2.000000f;
		case BlockType::SmoothStoneSlab:  return 2.000000f;
		case BlockType::SandstoneSlab:  return 2.000000f;
		case BlockType::CutSandstoneSlab:  return 2.000000f;
		case BlockType::PetrifiedOakSlab:  return 2.000000f;
		case BlockType::CobblestoneSlab:  return 2.000000f;
		case BlockType::BrickSlab:  return 2.000000f;
		case BlockType::StoneBrickSlab:  return 2.000000f;
		case BlockType::MudBrickSlab:  return 1.500000f;
		case BlockType::NetherBrickSlab:  return 2.000000f;
		case BlockType::QuartzSlab:  return 2.000000f;
		case BlockType::RedSandstoneSlab:  return 2.000000f;
		case BlockType::CutRedSandstoneSlab:  return 2.000000f;
		case BlockType::PurpurSlab:  return 2.000000f;
		case BlockType::SmoothStone:  return 2.000000f;
		case BlockType::SmoothSandstone:  return 2.000000f;
		case BlockType::SmoothQuartz:  return 2.000000f;
		case BlockType::SmoothRedSandstone:  return 2.000000f;
		case BlockType::SpruceFenceGate:  return 2.000000f;
		case BlockType::BirchFenceGate:  return 2.000000f;
		case BlockType::JungleFenceGate:  return 2.000000f;
		case BlockType::AcaciaFenceGate:  return 2.000000f;
		case BlockType::CherryFenceGate:  return 2.000000f;
		case BlockType::DarkOakFenceGate:  return 2.000000f;
		case BlockType::PaleOakFenceGate:  return 2.000000f;
		case BlockType::MangroveFenceGate:  return 2.000000f;
		case BlockType::BambooFenceGate:  return 2.000000f;
		case BlockType::SpruceFence:  return 2.000000f;
		case BlockType::BirchFence:  return 2.000000f;
		case BlockType::JungleFence:  return 2.000000f;
		case BlockType::AcaciaFence:  return 2.000000f;
		case BlockType::CherryFence:  return 2.000000f;
		case BlockType::DarkOakFence:  return 2.000000f;
		case BlockType::PaleOakFence:  return 2.000000f;
		case BlockType::MangroveFence:  return 2.000000f;
		case BlockType::BambooFence:  return 2.000000f;
		case BlockType::SpruceDoor:  return 3.000000f;
		case BlockType::BirchDoor:  return 3.000000f;
		case BlockType::JungleDoor:  return 3.000000f;
		case BlockType::AcaciaDoor:  return 3.000000f;
		case BlockType::CherryDoor:  return 3.000000f;
		case BlockType::DarkOakDoor:  return 3.000000f;
		case BlockType::PaleOakDoor:  return 3.000000f;
		case BlockType::MangroveDoor:  return 3.000000f;
		case BlockType::BambooDoor:  return 3.000000f;
		case BlockType::EndRod:  return 0.000000f;
		case BlockType::ChorusPlant:  return 0.400000f;
		case BlockType::ChorusFlower:  return 0.400000f;
		case BlockType::PurpurBlock:  return 1.500000f;
		case BlockType::PurpurPillar:  return 1.500000f;
		case BlockType::PurpurStairs:  return 1.500000f;
		case BlockType::EndStoneBricks:  return 3.000000f;
		case BlockType::TorchflowerCrop:  return 0.000000f;
		case BlockType::PitcherCrop:  return 0.000000f;
		case BlockType::PitcherPlant:  return 0.000000f;
		case BlockType::Beetroots:  return 0.000000f;
		case BlockType::DirtPath:  return 0.650000f;
		case BlockType::EndGateway:  return -1.000000f;
		case BlockType::RepeatingCommandBlock:  return -1.000000f;
		case BlockType::ChainCommandBlock:  return -1.000000f;
		case BlockType::FrostedIce:  return 0.500000f;
		case BlockType::MagmaBlock:  return 0.500000f;
		case BlockType::NetherWartBlock:  return 1.000000f;
		case BlockType::RedNetherBricks:  return 2.000000f;
		case BlockType::BoneBlock:  return 2.000000f;
		case BlockType::StructureVoid:  return 0.000000f;
		case BlockType::Observer:  return 3.000000f;
		case BlockType::ShulkerBox:  return 2.000000f;
		case BlockType::WhiteShulkerBox:  return 2.000000f;
		case BlockType::OrangeShulkerBox:  return 2.000000f;
		case BlockType::MagentaShulkerBox:  return 2.000000f;
		case BlockType::LightBlueShulkerBox:  return 2.000000f;
		case BlockType::YellowShulkerBox:  return 2.000000f;
		case BlockType::LimeShulkerBox:  return 2.000000f;
		case BlockType::PinkShulkerBox:  return 2.000000f;
		case BlockType::GrayShulkerBox:  return 2.000000f;
		case BlockType::LightGrayShulkerBox:  return 2.000000f;
		case BlockType::CyanShulkerBox:  return 2.000000f;
		case BlockType::PurpleShulkerBox:  return 2.000000f;
		case BlockType::BlueShulkerBox:  return 2.000000f;
		case BlockType::BrownShulkerBox:  return 2.000000f;
		case BlockType::GreenShulkerBox:  return 2.000000f;
		case BlockType::RedShulkerBox:  return 2.000000f;
		case BlockType::BlackShulkerBox:  return 2.000000f;
		case BlockType::WhiteGlazedTerracotta:  return 1.400000f;
		case BlockType::OrangeGlazedTerracotta:  return 1.400000f;
		case BlockType::MagentaGlazedTerracotta:  return 1.400000f;
		case BlockType::LightBlueGlazedTerracotta:  return 1.400000f;
		case BlockType::YellowGlazedTerracotta:  return 1.400000f;
		case BlockType::LimeGlazedTerracotta:  return 1.400000f;
		case BlockType::PinkGlazedTerracotta:  return 1.400000f;
		case BlockType::GrayGlazedTerracotta:  return 1.400000f;
		case BlockType::LightGrayGlazedTerracotta:  return 1.400000f;
		case BlockType::CyanGlazedTerracotta:  return 1.400000f;
		case BlockType::PurpleGlazedTerracotta:  return 1.400000f;
		case BlockType::BlueGlazedTerracotta:  return 1.400000f;
		case BlockType::BrownGlazedTerracotta:  return 1.400000f;
		case BlockType::GreenGlazedTerracotta:  return 1.400000f;
		case BlockType::RedGlazedTerracotta:  return 1.400000f;
		case BlockType::BlackGlazedTerracotta:  return 1.400000f;
		case BlockType::WhiteConcrete:  return 1.800000f;
		case BlockType::OrangeConcrete:  return 1.800000f;
		case BlockType::MagentaConcrete:  return 1.800000f;
		case BlockType::LightBlueConcrete:  return 1.800000f;
		case BlockType::YellowConcrete:  return 1.800000f;
		case BlockType::LimeConcrete:  return 1.800000f;
		case BlockType::PinkConcrete:  return 1.800000f;
		case BlockType::GrayConcrete:  return 1.800000f;
		case BlockType::LightGrayConcrete:  return 1.800000f;
		case BlockType::CyanConcrete:  return 1.800000f;
		case BlockType::PurpleConcrete:  return 1.800000f;
		case BlockType::BlueConcrete:  return 1.800000f;
		case BlockType::BrownConcrete:  return 1.800000f;
		case BlockType::GreenConcrete:  return 1.800000f;
		case BlockType::RedConcrete:  return 1.800000f;
		case BlockType::BlackConcrete:  return 1.800000f;
		case BlockType::WhiteConcretePowder:  return 0.500000f;
		case BlockType::OrangeConcretePowder:  return 0.500000f;
		case BlockType::MagentaConcretePowder:  return 0.500000f;
		case BlockType::LightBlueConcretePowder:  return 0.500000f;
		case BlockType::YellowConcretePowder:  return 0.500000f;
		case BlockType::LimeConcretePowder:  return 0.500000f;
		case BlockType::PinkConcretePowder:  return 0.500000f;
		case BlockType::GrayConcretePowder:  return 0.500000f;
		case BlockType::LightGrayConcretePowder:  return 0.500000f;
		case BlockType::CyanConcretePowder:  return 0.500000f;
		case BlockType::PurpleConcretePowder:  return 0.500000f;
		case BlockType::BlueConcretePowder:  return 0.500000f;
		case BlockType::BrownConcretePowder:  return 0.500000f;
		case BlockType::GreenConcretePowder:  return 0.500000f;
		case BlockType::RedConcretePowder:  return 0.500000f;
		case BlockType::BlackConcretePowder:  return 0.500000f;
		case BlockType::Kelp:  return 0.000000f;
		case BlockType::KelpPlant:  return 0.000000f;
		case BlockType::DriedKelpBlock:  return 0.500000f;
		case BlockType::TurtleEgg:  return 0.500000f;
		case BlockType::SnifferEgg:  return 0.500000f;
		case BlockType::DeadTubeCoralBlock:  return 1.500000f;
		case BlockType::DeadBrainCoralBlock:  return 1.500000f;
		case BlockType::DeadBubbleCoralBlock:  return 1.500000f;
		case BlockType::DeadFireCoralBlock:  return 1.500000f;
		case BlockType::DeadHornCoralBlock:  return 1.500000f;
		case BlockType::TubeCoralBlock:  return 1.500000f;
		case BlockType::BrainCoralBlock:  return 1.500000f;
		case BlockType::BubbleCoralBlock:  return 1.500000f;
		case BlockType::FireCoralBlock:  return 1.500000f;
		case BlockType::HornCoralBlock:  return 1.500000f;
		case BlockType::DeadTubeCoral:  return 0.000000f;
		case BlockType::DeadBrainCoral:  return 0.000000f;
		case BlockType::DeadBubbleCoral:  return 0.000000f;
		case BlockType::DeadFireCoral:  return 0.000000f;
		case BlockType::DeadHornCoral:  return 0.000000f;
		case BlockType::TubeCoral:  return 0.000000f;
		case BlockType::BrainCoral:  return 0.000000f;
		case BlockType::BubbleCoral:  return 0.000000f;
		case BlockType::FireCoral:  return 0.000000f;
		case BlockType::HornCoral:  return 0.000000f;
		case BlockType::DeadTubeCoralFan:  return 0.000000f;
		case BlockType::DeadBrainCoralFan:  return 0.000000f;
		case BlockType::DeadBubbleCoralFan:  return 0.000000f;
		case BlockType::DeadFireCoralFan:  return 0.000000f;
		case BlockType::DeadHornCoralFan:  return 0.000000f;
		case BlockType::TubeCoralFan:  return 0.000000f;
		case BlockType::BrainCoralFan:  return 0.000000f;
		case BlockType::BubbleCoralFan:  return 0.000000f;
		case BlockType::FireCoralFan:  return 0.000000f;
		case BlockType::HornCoralFan:  return 0.000000f;
		case BlockType::DeadTubeCoralWallFan:  return 0.000000f;
		case BlockType::DeadBrainCoralWallFan:  return 0.000000f;
		case BlockType::DeadBubbleCoralWallFan:  return 0.000000f;
		case BlockType::DeadFireCoralWallFan:  return 0.000000f;
		case BlockType::DeadHornCoralWallFan:  return 0.000000f;
		case BlockType::TubeCoralWallFan:  return 0.000000f;
		case BlockType::BrainCoralWallFan:  return 0.000000f;
		case BlockType::BubbleCoralWallFan:  return 0.000000f;
		case BlockType::FireCoralWallFan:  return 0.000000f;
		case BlockType::HornCoralWallFan:  return 0.000000f;
		case BlockType::SeaPickle:  return 0.000000f;
		case BlockType::BlueIce:  return 2.800000f;
		case BlockType::Conduit:  return 3.000000f;
		case BlockType::BambooSapling:  return 1.000000f;
		case BlockType::Bamboo:  return 1.000000f;
		case BlockType::PottedBamboo:  return 0.000000f;
		case BlockType::VoidAir:  return 0.000000f;
		case BlockType::CaveAir:  return 0.000000f;
		case BlockType::BubbleColumn:  return 0.000000f;
		case BlockType::PolishedGraniteStairs:  return 1.500000f;
		case BlockType::SmoothRedSandstoneStairs:  return 2.000000f;
		case BlockType::MossyStoneBrickStairs:  return 1.500000f;
		case BlockType::PolishedDioriteStairs:  return 1.500000f;
		case BlockType::MossyCobblestoneStairs:  return 2.000000f;
		case BlockType::EndStoneBrickStairs:  return 3.000000f;
		case BlockType::StoneStairs:  return 1.500000f;
		case BlockType::SmoothSandstoneStairs:  return 2.000000f;
		case BlockType::SmoothQuartzStairs:  return 2.000000f;
		case BlockType::GraniteStairs:  return 1.500000f;
		case BlockType::AndesiteStairs:  return 1.500000f;
		case BlockType::RedNetherBrickStairs:  return 2.000000f;
		case BlockType::PolishedAndesiteStairs:  return 1.500000f;
		case BlockType::DioriteStairs:  return 1.500000f;
		case BlockType::PolishedGraniteSlab:  return 1.500000f;
		case BlockType::SmoothRedSandstoneSlab:  return 2.000000f;
		case BlockType::MossyStoneBrickSlab:  return 1.500000f;
		case BlockType::PolishedDioriteSlab:  return 1.500000f;
		case BlockType::MossyCobblestoneSlab:  return 2.000000f;
		case BlockType::EndStoneBrickSlab:  return 3.000000f;
		case BlockType::SmoothSandstoneSlab:  return 2.000000f;
		case BlockType::SmoothQuartzSlab:  return 2.000000f;
		case BlockType::GraniteSlab:  return 1.500000f;
		case BlockType::AndesiteSlab:  return 1.500000f;
		case BlockType::RedNetherBrickSlab:  return 2.000000f;
		case BlockType::PolishedAndesiteSlab:  return 1.500000f;
		case BlockType::DioriteSlab:  return 1.500000f;
		case BlockType::BrickWall:  return 2.000000f;
		case BlockType::PrismarineWall:  return 1.500000f;
		case BlockType::RedSandstoneWall:  return 0.800000f;
		case BlockType::MossyStoneBrickWall:  return 1.500000f;
		case BlockType::GraniteWall:  return 1.500000f;
		case BlockType::StoneBrickWall:  return 1.500000f;
		case BlockType::MudBrickWall:  return 1.500000f;
		case BlockType::NetherBrickWall:  return 2.000000f;
		case BlockType::AndesiteWall:  return 1.500000f;
		case BlockType::RedNetherBrickWall:  return 2.000000f;
		case BlockType::SandstoneWall:  return 0.800000f;
		case BlockType::EndStoneBrickWall:  return 3.000000f;
		case BlockType::DioriteWall:  return 1.500000f;
		case BlockType::Scaffolding:  return 0.000000f;
		case BlockType::Loom:  return 2.500000f;
		case BlockType::Barrel:  return 2.500000f;
		case BlockType::Smoker:  return 3.500000f;
		case BlockType::BlastFurnace:  return 3.500000f;
		case BlockType::CartographyTable:  return 2.500000f;
		case BlockType::FletchingTable:  return 2.500000f;
		case BlockType::Grindstone:  return 2.000000f;
		case BlockType::Lectern:  return 2.500000f;
		case BlockType::SmithingTable:  return 2.500000f;
		case BlockType::Stonecutter:  return 3.500000f;
		case BlockType::Bell:  return 5.000000f;
		case BlockType::Lantern:  return 3.500000f;
		case BlockType::SoulLantern:  return 3.500000f;
		case BlockType::Campfire:  return 2.000000f;
		case BlockType::SoulCampfire:  return 2.000000f;
		case BlockType::SweetBerryBush:  return 0.000000f;
		case BlockType::WarpedStem:  return 2.000000f;
		case BlockType::StrippedWarpedStem:  return 2.000000f;
		case BlockType::WarpedHyphae:  return 2.000000f;
		case BlockType::StrippedWarpedHyphae:  return 2.000000f;
		case BlockType::WarpedNylium:  return 0.400000f;
		case BlockType::WarpedFungus:  return 0.000000f;
		case BlockType::WarpedWartBlock:  return 1.000000f;
		case BlockType::WarpedRoots:  return 0.000000f;
		case BlockType::NetherSprouts:  return 0.000000f;
		case BlockType::CrimsonStem:  return 2.000000f;
		case BlockType::StrippedCrimsonStem:  return 2.000000f;
		case BlockType::CrimsonHyphae:  return 2.000000f;
		case BlockType::StrippedCrimsonHyphae:  return 2.000000f;
		case BlockType::CrimsonNylium:  return 0.400000f;
		case BlockType::CrimsonFungus:  return 0.000000f;
		case BlockType::Shroomlight:  return 1.000000f;
		case BlockType::WeepingVines:  return 0.000000f;
		case BlockType::WeepingVinesPlant:  return 0.000000f;
		case BlockType::TwistingVines:  return 0.000000f;
		case BlockType::TwistingVinesPlant:  return 0.000000f;
		case BlockType::CrimsonRoots:  return 0.000000f;
		case BlockType::CrimsonPlanks:  return 2.000000f;
		case BlockType::WarpedPlanks:  return 2.000000f;
		case BlockType::CrimsonSlab:  return 2.000000f;
		case BlockType::WarpedSlab:  return 2.000000f;
		case BlockType::CrimsonPressurePlate:  return 0.500000f;
		case BlockType::WarpedPressurePlate:  return 0.500000f;
		case BlockType::CrimsonFence:  return 2.000000f;
		case BlockType::WarpedFence:  return 2.000000f;
		case BlockType::CrimsonTrapdoor:  return 3.000000f;
		case BlockType::WarpedTrapdoor:  return 3.000000f;
		case BlockType::CrimsonFenceGate:  return 2.000000f;
		case BlockType::WarpedFenceGate:  return 2.000000f;
		case BlockType::CrimsonStairs:  return 2.000000f;
		case BlockType::WarpedStairs:  return 2.000000f;
		case BlockType::CrimsonButton:  return 0.500000f;
		case BlockType::WarpedButton:  return 0.500000f;
		case BlockType::CrimsonDoor:  return 3.000000f;
		case BlockType::WarpedDoor:  return 3.000000f;
		case BlockType::CrimsonSign:  return 1.000000f;
		case BlockType::WarpedSign:  return 1.000000f;
		case BlockType::CrimsonWallSign:  return 1.000000f;
		case BlockType::WarpedWallSign:  return 1.000000f;
		case BlockType::StructureBlock:  return -1.000000f;
		case BlockType::Jigsaw:  return -1.000000f;
		case BlockType::Composter:  return 0.600000f;
		case BlockType::Target:  return 0.500000f;
		case BlockType::BeeNest:  return 0.300000f;
		case BlockType::Beehive:  return 0.600000f;
		case BlockType::HoneyBlock:  return 0.000000f;
		case BlockType::HoneycombBlock:  return 0.600000f;
		case BlockType::NetheriteBlock:  return 50.000000f;
		case BlockType::AncientDebris:  return 30.000000f;
		case BlockType::CryingObsidian:  return 50.000000f;
		case BlockType::RespawnAnchor:  return 50.000000f;
		case BlockType::PottedCrimsonFungus:  return 0.000000f;
		case BlockType::PottedWarpedFungus:  return 0.000000f;
		case BlockType::PottedCrimsonRoots:  return 0.000000f;
		case BlockType::PottedWarpedRoots:  return 0.000000f;
		case BlockType::Lodestone:  return 3.500000f;
		case BlockType::Blackstone:  return 1.500000f;
		case BlockType::BlackstoneStairs:  return 1.500000f;
		case BlockType::BlackstoneWall:  return 1.500000f;
		case BlockType::BlackstoneSlab:  return 2.000000f;
		case BlockType::PolishedBlackstone:  return 2.000000f;
		case BlockType::PolishedBlackstoneBricks:  return 1.500000f;
		case BlockType::CrackedPolishedBlackstoneBricks:  return 1.500000f;
		case BlockType::ChiseledPolishedBlackstone:  return 1.500000f;
		case BlockType::PolishedBlackstoneBrickSlab:  return 2.000000f;
		case BlockType::PolishedBlackstoneBrickStairs:  return 1.500000f;
		case BlockType::PolishedBlackstoneBrickWall:  return 1.500000f;
		case BlockType::GildedBlackstone:  return 1.500000f;
		case BlockType::PolishedBlackstoneStairs:  return 2.000000f;
		case BlockType::PolishedBlackstoneSlab:  return 2.000000f;
		case BlockType::PolishedBlackstonePressurePlate:  return 0.500000f;
		case BlockType::PolishedBlackstoneButton:  return 0.500000f;
		case BlockType::PolishedBlackstoneWall:  return 2.000000f;
		case BlockType::ChiseledNetherBricks:  return 2.000000f;
		case BlockType::CrackedNetherBricks:  return 2.000000f;
		case BlockType::QuartzBricks:  return 0.800000f;
		case BlockType::Candle:  return 0.100000f;
		case BlockType::WhiteCandle:  return 0.100000f;
		case BlockType::OrangeCandle:  return 0.100000f;
		case BlockType::MagentaCandle:  return 0.100000f;
		case BlockType::LightBlueCandle:  return 0.100000f;
		case BlockType::YellowCandle:  return 0.100000f;
		case BlockType::LimeCandle:  return 0.100000f;
		case BlockType::PinkCandle:  return 0.100000f;
		case BlockType::GrayCandle:  return 0.100000f;
		case BlockType::LightGrayCandle:  return 0.100000f;
		case BlockType::CyanCandle:  return 0.100000f;
		case BlockType::PurpleCandle:  return 0.100000f;
		case BlockType::BlueCandle:  return 0.100000f;
		case BlockType::BrownCandle:  return 0.100000f;
		case BlockType::GreenCandle:  return 0.100000f;
		case BlockType::RedCandle:  return 0.100000f;
		case BlockType::BlackCandle:  return 0.100000f;
		case BlockType::CandleCake:  return 0.500000f;
		case BlockType::WhiteCandleCake:  return 0.500000f;
		case BlockType::OrangeCandleCake:  return 0.500000f;
		case BlockType::MagentaCandleCake:  return 0.500000f;
		case BlockType::LightBlueCandleCake:  return 0.500000f;
		case BlockType::YellowCandleCake:  return 0.500000f;
		case BlockType::LimeCandleCake:  return 0.500000f;
		case BlockType::PinkCandleCake:  return 0.500000f;
		case BlockType::GrayCandleCake:  return 0.500000f;
		case BlockType::LightGrayCandleCake:  return 0.500000f;
		case BlockType::CyanCandleCake:  return 0.500000f;
		case BlockType::PurpleCandleCake:  return 0.500000f;
		case BlockType::BlueCandleCake:  return 0.500000f;
		case BlockType::BrownCandleCake:  return 0.500000f;
		case BlockType::GreenCandleCake:  return 0.500000f;
		case BlockType::RedCandleCake:  return 0.500000f;
		case BlockType::BlackCandleCake:  return 0.500000f;
		case BlockType::AmethystBlock:  return 1.500000f;
		case BlockType::BuddingAmethyst:  return 1.500000f;
		case BlockType::AmethystCluster:  return 1.500000f;
		case BlockType::LargeAmethystBud:  return 1.500000f;
		case BlockType::MediumAmethystBud:  return 1.500000f;
		case BlockType::SmallAmethystBud:  return 1.500000f;
		case BlockType::Tuff:  return 1.500000f;
		case BlockType::TuffSlab:  return 1.500000f;
		case BlockType::TuffStairs:  return 1.500000f;
		case BlockType::TuffWall:  return 1.500000f;
		case BlockType::PolishedTuff:  return 1.500000f;
		case BlockType::PolishedTuffSlab:  return 1.500000f;
		case BlockType::PolishedTuffStairs:  return 1.500000f;
		case BlockType::PolishedTuffWall:  return 1.500000f;
		case BlockType::ChiseledTuff:  return 1.500000f;
		case BlockType::TuffBricks:  return 1.500000f;
		case BlockType::TuffBrickSlab:  return 1.500000f;
		case BlockType::TuffBrickStairs:  return 1.500000f;
		case BlockType::TuffBrickWall:  return 1.500000f;
		case BlockType::ChiseledTuffBricks:  return 1.500000f;
		case BlockType::Calcite:  return 0.750000f;
		case BlockType::TintedGlass:  return 0.300000f;
		case BlockType::PowderSnow:  return 0.250000f;
		case BlockType::SculkSensor:  return 1.500000f;
		case BlockType::CalibratedSculkSensor:  return 1.500000f;
		case BlockType::Sculk:  return 0.200000f;
		case BlockType::SculkVein:  return 0.200000f;
		case BlockType::SculkCatalyst:  return 3.000000f;
		case BlockType::SculkShrieker:  return 3.000000f;
		case BlockType::CopperBlock:  return 3.000000f;
		case BlockType::ExposedCopper:  return 3.000000f;
		case BlockType::WeatheredCopper:  return 3.000000f;
		case BlockType::OxidizedCopper:  return 3.000000f;
		case BlockType::CopperOre:  return 3.000000f;
		case BlockType::DeepslateCopperOre:  return 4.500000f;
		case BlockType::OxidizedCutCopper:  return 3.000000f;
		case BlockType::WeatheredCutCopper:  return 3.000000f;
		case BlockType::ExposedCutCopper:  return 3.000000f;
		case BlockType::CutCopper:  return 3.000000f;
		case BlockType::OxidizedChiseledCopper:  return 3.000000f;
		case BlockType::WeatheredChiseledCopper:  return 3.000000f;
		case BlockType::ExposedChiseledCopper:  return 3.000000f;
		case BlockType::ChiseledCopper:  return 3.000000f;
		case BlockType::WaxedOxidizedChiseledCopper:  return 3.000000f;
		case BlockType::WaxedWeatheredChiseledCopper:  return 3.000000f;
		case BlockType::WaxedExposedChiseledCopper:  return 3.000000f;
		case BlockType::WaxedChiseledCopper:  return 3.000000f;
		case BlockType::OxidizedCutCopperStairs:  return 3.000000f;
		case BlockType::WeatheredCutCopperStairs:  return 3.000000f;
		case BlockType::ExposedCutCopperStairs:  return 3.000000f;
		case BlockType::CutCopperStairs:  return 3.000000f;
		case BlockType::OxidizedCutCopperSlab:  return 3.000000f;
		case BlockType::WeatheredCutCopperSlab:  return 3.000000f;
		case BlockType::ExposedCutCopperSlab:  return 3.000000f;
		case BlockType::CutCopperSlab:  return 3.000000f;
		case BlockType::WaxedCopperBlock:  return 3.000000f;
		case BlockType::WaxedWeatheredCopper:  return 3.000000f;
		case BlockType::WaxedExposedCopper:  return 3.000000f;
		case BlockType::WaxedOxidizedCopper:  return 3.000000f;
		case BlockType::WaxedOxidizedCutCopper:  return 3.000000f;
		case BlockType::WaxedWeatheredCutCopper:  return 3.000000f;
		case BlockType::WaxedExposedCutCopper:  return 3.000000f;
		case BlockType::WaxedCutCopper:  return 3.000000f;
		case BlockType::WaxedOxidizedCutCopperStairs:  return 3.000000f;
		case BlockType::WaxedWeatheredCutCopperStairs:  return 3.000000f;
		case BlockType::WaxedExposedCutCopperStairs:  return 3.000000f;
		case BlockType::WaxedCutCopperStairs:  return 3.000000f;
		case BlockType::WaxedOxidizedCutCopperSlab:  return 3.000000f;
		case BlockType::WaxedWeatheredCutCopperSlab:  return 3.000000f;
		case BlockType::WaxedExposedCutCopperSlab:  return 3.000000f;
		case BlockType::WaxedCutCopperSlab:  return 3.000000f;
		case BlockType::CopperDoor:  return 3.000000f;
		case BlockType::ExposedCopperDoor:  return 3.000000f;
		case BlockType::OxidizedCopperDoor:  return 3.000000f;
		case BlockType::WeatheredCopperDoor:  return 3.000000f;
		case BlockType::WaxedCopperDoor:  return 3.000000f;
		case BlockType::WaxedExposedCopperDoor:  return 3.000000f;
		case BlockType::WaxedOxidizedCopperDoor:  return 3.000000f;
		case BlockType::WaxedWeatheredCopperDoor:  return 3.000000f;
		case BlockType::CopperTrapdoor:  return 3.000000f;
		case BlockType::ExposedCopperTrapdoor:  return 3.000000f;
		case BlockType::OxidizedCopperTrapdoor:  return 3.000000f;
		case BlockType::WeatheredCopperTrapdoor:  return 3.000000f;
		case BlockType::WaxedCopperTrapdoor:  return 3.000000f;
		case BlockType::WaxedExposedCopperTrapdoor:  return 3.000000f;
		case BlockType::WaxedOxidizedCopperTrapdoor:  return 3.000000f;
		case BlockType::WaxedWeatheredCopperTrapdoor:  return 3.000000f;
		case BlockType::CopperGrate:  return 3.000000f;
		case BlockType::ExposedCopperGrate:  return 3.000000f;
		case BlockType::WeatheredCopperGrate:  return 3.000000f;
		case BlockType::OxidizedCopperGrate:  return 3.000000f;
		case BlockType::WaxedCopperGrate:  return 3.000000f;
		case BlockType::WaxedExposedCopperGrate:  return 3.000000f;
		case BlockType::WaxedWeatheredCopperGrate:  return 3.000000f;
		case BlockType::WaxedOxidizedCopperGrate:  return 3.000000f;
		case BlockType::CopperBulb:  return 3.000000f;
		case BlockType::ExposedCopperBulb:  return 3.000000f;
		case BlockType::WeatheredCopperBulb:  return 3.000000f;
		case BlockType::OxidizedCopperBulb:  return 3.000000f;
		case BlockType::WaxedCopperBulb:  return 3.000000f;
		case BlockType::WaxedExposedCopperBulb:  return 3.000000f;
		case BlockType::WaxedWeatheredCopperBulb:  return 3.000000f;
		case BlockType::WaxedOxidizedCopperBulb:  return 3.000000f;
		case BlockType::LightningRod:  return 3.000000f;
		case BlockType::PointedDripstone:  return 1.500000f;
		case BlockType::DripstoneBlock:  return 1.500000f;
		case BlockType::CaveVines:  return 0.000000f;
		case BlockType::CaveVinesPlant:  return 0.000000f;
		case BlockType::SporeBlossom:  return 0.000000f;
		case BlockType::Azalea:  return 0.000000f;
		case BlockType::FloweringAzalea:  return 0.000000f;
		case BlockType::MossCarpet:  return 0.100000f;
		case BlockType::PinkPetals:  return 0.000000f;
		case BlockType::MossBlock:  return 0.100000f;
		case BlockType::BigDripleaf:  return 0.100000f;
		case BlockType::BigDripleafStem:  return 0.100000f;
		case BlockType::SmallDripleaf:  return 0.000000f;
		case BlockType::HangingRoots:  return 0.000000f;
		case BlockType::RootedDirt:  return 0.500000f;
		case BlockType::Mud:  return 0.500000f;
		case BlockType::Deepslate:  return 3.000000f;
		case BlockType::CobbledDeepslate:  return 3.500000f;
		case BlockType::CobbledDeepslateStairs:  return 3.500000f;
		case BlockType::CobbledDeepslateSlab:  return 3.500000f;
		case BlockType::CobbledDeepslateWall:  return 3.500000f;
		case BlockType::PolishedDeepslate:  return 3.500000f;
		case BlockType::PolishedDeepslateStairs:  return 3.500000f;
		case BlockType::PolishedDeepslateSlab:  return 3.500000f;
		case BlockType::PolishedDeepslateWall:  return 3.500000f;
		case BlockType::DeepslateTiles:  return 3.500000f;
		case BlockType::DeepslateTileStairs:  return 3.500000f;
		case BlockType::DeepslateTileSlab:  return 3.500000f;
		case BlockType::DeepslateTileWall:  return 3.500000f;
		case BlockType::DeepslateBricks:  return 3.500000f;
		case BlockType::DeepslateBrickStairs:  return 3.500000f;
		case BlockType::DeepslateBrickSlab:  return 3.500000f;
		case BlockType::DeepslateBrickWall:  return 3.500000f;
		case BlockType::ChiseledDeepslate:  return 3.500000f;
		case BlockType::CrackedDeepslateBricks:  return 3.500000f;
		case BlockType::CrackedDeepslateTiles:  return 3.500000f;
		case BlockType::InfestedDeepslate:  return 1.500000f;
		case BlockType::SmoothBasalt:  return 1.250000f;
		case BlockType::RawIronBlock:  return 5.000000f;
		case BlockType::RawCopperBlock:  return 5.000000f;
		case BlockType::RawGoldBlock:  return 5.000000f;
		case BlockType::PottedAzaleaBush:  return 0.000000f;
		case BlockType::PottedFloweringAzaleaBush:  return 0.000000f;
		case BlockType::OchreFroglight:  return 0.300000f;
		case BlockType::VerdantFroglight:  return 0.300000f;
		case BlockType::PearlescentFroglight:  return 0.300000f;
		case BlockType::Frogspawn:  return 0.000000f;
		case BlockType::ReinforcedDeepslate:  return 55.000000f;
		case BlockType::DecoratedPot:  return 0.000000f;
		case BlockType::Crafter:  return 1.500000f;
		case BlockType::TrialSpawner:  return 50.000000f;
		case BlockType::Vault:  return 50.000000f;
		case BlockType::HeavyCore:  return 10.000000f;
		case BlockType::PaleMossBlock:  return 0.100000f;
		case BlockType::PaleMossCarpet:  return 0.100000f;
		case BlockType::PaleHangingMoss:  return 0.000000f;
		case BlockType::OpenEyeblossom:  return 0.000000f;
		case BlockType::ClosedEyeblossom:  return 0.000000f;
		case BlockType::PottedOpenEyeblossom:  return 0.000000f;
		case BlockType::PottedClosedEyeblossom:  return 0.000000f;
	}
	UNREACHABLE("Unhandled block type");
}
