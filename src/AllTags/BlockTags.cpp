#include "BlockTags.h"

namespace BlockTags
{
	bool AcaciaLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::AcaciaLog:
			case BlockType::AcaciaWood:
			case BlockType::StrippedAcaciaLog:
			case BlockType::StrippedAcaciaWood:
				return true;
			default: return false;
		}
	}
	bool Air(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Air:
			case BlockType::VoidAir:
			case BlockType::CaveAir:
				return true;
			default: return false;
		}
	}
	bool AllHangingSigns(BlockType a_block)
	{
		return CeilingHangingSigns(a_block) || WallHangingSigns(a_block);
	}
	bool AllSigns(BlockType a_block)
	{
		return Signs(a_block) || AllHangingSigns(a_block);
	}
	bool AncientCityReplaceable(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Deepslate:
			case BlockType::DeepslateBricks:
			case BlockType::DeepslateTiles:
			case BlockType::DeepslateBrickSlab:
			case BlockType::DeepslateTileSlab:
			case BlockType::DeepslateBrickStairs:
			case BlockType::DeepslateTileWall:
			case BlockType::DeepslateBrickWall:
			case BlockType::CobbledDeepslate:
			case BlockType::CrackedDeepslateBricks:
			case BlockType::CrackedDeepslateTiles:
			case BlockType::GrayWool:
				return true;
			default: return false;
		}
	}
	bool AnimalsSpawnableOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::GrassBlock:
				return true;
			default: return false;
		}
	}
	bool Anvil(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Anvil:
			case BlockType::ChippedAnvil:
			case BlockType::DamagedAnvil:
				return true;
			default: return false;
		}
	}
	bool ArmadilloSpawnableOn(BlockType a_block)
	{
		if (AnimalsSpawnableOn(a_block) || BadlandsTerracotta(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::RedSand:
			case BlockType::CoarseDirt:
				return true;
			default: return false;
		}
	}
	bool AxolotlsSpawnableOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Clay:
				return true;
			default: return false;
		}
	}
	bool AzaleaGrowsOn(BlockType a_block)
	{
		if (Dirt(a_block) || Sand(a_block) || Terracotta(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::SnowBlock:
			case BlockType::PowderSnow:
				return true;
			default: return false;
		}
	}
	bool AzaleaRootReplaceable(BlockType a_block)
	{
		if (BaseStoneOverworld(a_block) || Dirt(a_block) || Terracotta(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::RedSand:
			case BlockType::Clay:
			case BlockType::Gravel:
			case BlockType::Sand:
			case BlockType::SnowBlock:
			case BlockType::PowderSnow:
				return true;
			default: return false;
		}
	}
	bool BadlandsTerracotta(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Terracotta:
			case BlockType::WhiteTerracotta:
			case BlockType::YellowTerracotta:
			case BlockType::OrangeTerracotta:
			case BlockType::RedTerracotta:
			case BlockType::BrownTerracotta:
			case BlockType::LightGrayTerracotta:
				return true;
			default: return false;
		}
	}
	bool BambooBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::BambooBlock:
			case BlockType::StrippedBambooBlock:
				return true;
			default: return false;
		}
	}
	bool BambooPlantableOn(BlockType a_block)
	{
		if (Sand(a_block) || Dirt(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Bamboo:
			case BlockType::BambooSapling:
			case BlockType::Gravel:
			case BlockType::SuspiciousGravel:
				return true;
			default: return false;
		}
	}
	bool Banners(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::WhiteBanner:
			case BlockType::OrangeBanner:
			case BlockType::MagentaBanner:
			case BlockType::LightBlueBanner:
			case BlockType::YellowBanner:
			case BlockType::LimeBanner:
			case BlockType::PinkBanner:
			case BlockType::GrayBanner:
			case BlockType::LightGrayBanner:
			case BlockType::CyanBanner:
			case BlockType::PurpleBanner:
			case BlockType::BlueBanner:
			case BlockType::BrownBanner:
			case BlockType::GreenBanner:
			case BlockType::RedBanner:
			case BlockType::BlackBanner:
			case BlockType::WhiteWallBanner:
			case BlockType::OrangeWallBanner:
			case BlockType::MagentaWallBanner:
			case BlockType::LightBlueWallBanner:
			case BlockType::YellowWallBanner:
			case BlockType::LimeWallBanner:
			case BlockType::PinkWallBanner:
			case BlockType::GrayWallBanner:
			case BlockType::LightGrayWallBanner:
			case BlockType::CyanWallBanner:
			case BlockType::PurpleWallBanner:
			case BlockType::BlueWallBanner:
			case BlockType::BrownWallBanner:
			case BlockType::GreenWallBanner:
			case BlockType::RedWallBanner:
			case BlockType::BlackWallBanner:
				return true;
			default: return false;
		}
	}
	bool BaseStoneNether(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Netherrack:
			case BlockType::Basalt:
			case BlockType::Blackstone:
				return true;
			default: return false;
		}
	}
	bool BaseStoneOverworld(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Stone:
			case BlockType::Granite:
			case BlockType::Diorite:
			case BlockType::Andesite:
			case BlockType::Tuff:
			case BlockType::Deepslate:
				return true;
			default: return false;
		}
	}
	bool BatsSpawnableOn(BlockType a_block)
	{
		return BaseStoneOverworld(a_block);
	}
	bool BeaconBaseBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::NetheriteBlock:
			case BlockType::EmeraldBlock:
			case BlockType::DiamondBlock:
			case BlockType::GoldBlock:
			case BlockType::IronBlock:
				return true;
			default: return false;
		}
	}
	bool Beds(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::RedBed:
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
			case BlockType::WhiteBed:
			case BlockType::YellowBed:
				return true;
			default: return false;
		}
	}
	bool Beehives(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::BeeNest:
			case BlockType::Beehive:
				return true;
			default: return false;
		}
	}
	bool BeeGrowables(BlockType a_block)
	{
		if (Crops(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::SweetBerryBush:
			case BlockType::CaveVines:
			case BlockType::CaveVinesPlant:
				return true;
			default: return false;
		}
	}
	bool BigDripleafPlaceable(BlockType a_block)
	{
		if (SmallDripleafPlaceable(a_block) || Dirt(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Farmland:
				return true;
			default: return false;
		}
	}
	bool BirchLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::BirchLog:
			case BlockType::BirchWood:
			case BlockType::StrippedBirchLog:
			case BlockType::StrippedBirchWood:
				return true;
			default: return false;
		}
	}
	bool BlocksWindChargeExplosions(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Barrier:
			case BlockType::Bedrock:
				return true;
			default: return false;
		}
	}
	bool Buttons(BlockType a_block)
	{
		return WoodenButtons(a_block) || StoneButtons(a_block);
	}
	bool CamelSandStepSoundBlocks(BlockType a_block)
	{
		return Sand(a_block) || ConcretePowder(a_block);
	}
	bool Campfires(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Campfire:
			case BlockType::SoulCampfire:
				return true;
			default: return false;
		}
	}
	bool Candles(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Candle:
			case BlockType::WhiteCandle:
			case BlockType::OrangeCandle:
			case BlockType::MagentaCandle:
			case BlockType::LightBlueCandle:
			case BlockType::YellowCandle:
			case BlockType::LimeCandle:
			case BlockType::PinkCandle:
			case BlockType::GrayCandle:
			case BlockType::LightGrayCandle:
			case BlockType::CyanCandle:
			case BlockType::PurpleCandle:
			case BlockType::BlueCandle:
			case BlockType::BrownCandle:
			case BlockType::GreenCandle:
			case BlockType::RedCandle:
			case BlockType::BlackCandle:
				return true;
			default: return false;
		}
	}
	bool CandleCakes(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::CandleCake:
			case BlockType::WhiteCandleCake:
			case BlockType::OrangeCandleCake:
			case BlockType::MagentaCandleCake:
			case BlockType::LightBlueCandleCake:
			case BlockType::YellowCandleCake:
			case BlockType::LimeCandleCake:
			case BlockType::PinkCandleCake:
			case BlockType::GrayCandleCake:
			case BlockType::LightGrayCandleCake:
			case BlockType::CyanCandleCake:
			case BlockType::PurpleCandleCake:
			case BlockType::BlueCandleCake:
			case BlockType::BrownCandleCake:
			case BlockType::GreenCandleCake:
			case BlockType::RedCandleCake:
			case BlockType::BlackCandleCake:
				return true;
			default: return false;
		}
	}
	bool Cauldrons(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Cauldron:
			case BlockType::WaterCauldron:
			case BlockType::LavaCauldron:
			case BlockType::PowderSnowCauldron:
				return true;
			default: return false;
		}
	}
	bool CaveVines(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::CaveVinesPlant:
			case BlockType::CaveVines:
				return true;
			default: return false;
		}
	}
	bool CeilingHangingSigns(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakHangingSign:
			case BlockType::SpruceHangingSign:
			case BlockType::BirchHangingSign:
			case BlockType::AcaciaHangingSign:
			case BlockType::CherryHangingSign:
			case BlockType::JungleHangingSign:
			case BlockType::DarkOakHangingSign:
			case BlockType::CrimsonHangingSign:
			case BlockType::WarpedHangingSign:
			case BlockType::MangroveHangingSign:
			case BlockType::BambooHangingSign:
				return true;
			default: return false;
		}
	}
	bool CherryLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::CherryLog:
			case BlockType::CherryWood:
			case BlockType::StrippedCherryLog:
			case BlockType::StrippedCherryWood:
				return true;
			default: return false;
		}
	}
	bool Climbable(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Ladder:
			case BlockType::Vine:
			case BlockType::Scaffolding:
			case BlockType::WeepingVines:
			case BlockType::WeepingVinesPlant:
			case BlockType::TwistingVines:
			case BlockType::TwistingVinesPlant:
			case BlockType::CaveVines:
			case BlockType::CaveVinesPlant:
				return true;
			default: return false;
		}
	}
	bool CoalOres(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::CoalOre:
			case BlockType::DeepslateCoalOre:
				return true;
			default: return false;
		}
	}
	bool CombinationStepSoundBlocks(BlockType a_block)
	{
		if (WoolCarpets(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::MossCarpet:
			case BlockType::Snow:
			case BlockType::NetherSprouts:
			case BlockType::WarpedRoots:
			case BlockType::CrimsonRoots:
				return true;
			default: return false;
		}
	}
	bool CompletesFindTreeTutorial(BlockType a_block)
	{
		return Logs(a_block) || Leaves(a_block) || WartBlocks(a_block);
	}
	bool ConcretePowder(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::WhiteConcretePowder:
			case BlockType::OrangeConcretePowder:
			case BlockType::MagentaConcretePowder:
			case BlockType::LightBlueConcretePowder:
			case BlockType::YellowConcretePowder:
			case BlockType::LimeConcretePowder:
			case BlockType::PinkConcretePowder:
			case BlockType::GrayConcretePowder:
			case BlockType::LightGrayConcretePowder:
			case BlockType::CyanConcretePowder:
			case BlockType::PurpleConcretePowder:
			case BlockType::BlueConcretePowder:
			case BlockType::BrownConcretePowder:
			case BlockType::GreenConcretePowder:
			case BlockType::RedConcretePowder:
			case BlockType::BlackConcretePowder:
				return true;
			default: return false;
		}
	}
	bool ConvertableToMud(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Dirt:
			case BlockType::CoarseDirt:
			case BlockType::RootedDirt:
				return true;
			default: return false;
		}
	}
	bool CopperOres(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::CopperOre:
			case BlockType::DeepslateCopperOre:
				return true;
			default: return false;
		}
	}
	bool Corals(BlockType a_block)
	{
		if (CoralPlants(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::TubeCoralFan:
			case BlockType::BrainCoralFan:
			case BlockType::BubbleCoralFan:
			case BlockType::FireCoralFan:
			case BlockType::HornCoralFan:
				return true;
			default: return false;
		}
	}
	bool CoralBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::TubeCoralBlock:
			case BlockType::BrainCoralBlock:
			case BlockType::BubbleCoralBlock:
			case BlockType::FireCoralBlock:
			case BlockType::HornCoralBlock:
				return true;
			default: return false;
		}
	}
	bool CoralPlants(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::TubeCoral:
			case BlockType::BrainCoral:
			case BlockType::BubbleCoral:
			case BlockType::FireCoral:
			case BlockType::HornCoral:
				return true;
			default: return false;
		}
	}
	bool CrimsonStems(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::CrimsonStem:
			case BlockType::StrippedCrimsonStem:
			case BlockType::CrimsonHyphae:
			case BlockType::StrippedCrimsonHyphae:
				return true;
			default: return false;
		}
	}
	bool Crops(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Beetroots:
			case BlockType::Carrots:
			case BlockType::Potatoes:
			case BlockType::Wheat:
			case BlockType::MelonStem:
			case BlockType::PumpkinStem:
			case BlockType::TorchflowerCrop:
			case BlockType::PitcherCrop:
				return true;
			default: return false;
		}
	}
	bool CrystalSoundBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::AmethystBlock:
			case BlockType::BuddingAmethyst:
				return true;
			default: return false;
		}
	}
	bool DampensVibrations(BlockType a_block)
	{
		return Wool(a_block) || WoolCarpets(a_block);
	}
	bool DarkOakLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::DarkOakLog:
			case BlockType::DarkOakWood:
			case BlockType::StrippedDarkOakLog:
			case BlockType::StrippedDarkOakWood:
				return true;
			default: return false;
		}
	}
	bool DeadBushMayPlaceOn(BlockType a_block)
	{
		return Sand(a_block) || Terracotta(a_block) || Dirt(a_block);
	}
	bool DeepslateOreReplaceables(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Deepslate:
			case BlockType::Tuff:
				return true;
			default: return false;
		}
	}
	bool DiamondOres(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::DiamondOre:
			case BlockType::DeepslateDiamondOre:
				return true;
			default: return false;
		}
	}
	bool Dirt(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Dirt:
			case BlockType::GrassBlock:
			case BlockType::Podzol:
			case BlockType::CoarseDirt:
			case BlockType::Mycelium:
			case BlockType::RootedDirt:
			case BlockType::MossBlock:
			case BlockType::Mud:
			case BlockType::MuddyMangroveRoots:
				return true;
			default: return false;
		}
	}
	bool DoesNotBlockHoppers(BlockType a_block)
	{
		return Beehives(a_block);
	}
	bool Doors(BlockType a_block)
	{
		if (WoodenDoors(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::CopperDoor:
			case BlockType::ExposedCopperDoor:
			case BlockType::WeatheredCopperDoor:
			case BlockType::OxidizedCopperDoor:
			case BlockType::WaxedCopperDoor:
			case BlockType::WaxedExposedCopperDoor:
			case BlockType::WaxedWeatheredCopperDoor:
			case BlockType::WaxedOxidizedCopperDoor:
			case BlockType::IronDoor:
				return true;
			default: return false;
		}
	}
	bool DragonImmune(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Barrier:
			case BlockType::Bedrock:
			case BlockType::EndPortal:
			case BlockType::EndPortalFrame:
			case BlockType::EndGateway:
			case BlockType::CommandBlock:
			case BlockType::RepeatingCommandBlock:
			case BlockType::ChainCommandBlock:
			case BlockType::StructureBlock:
			case BlockType::Jigsaw:
			case BlockType::MovingPiston:
			case BlockType::Obsidian:
			case BlockType::CryingObsidian:
			case BlockType::EndStone:
			case BlockType::IronBars:
			case BlockType::RespawnAnchor:
			case BlockType::ReinforcedDeepslate:
				return true;
			default: return false;
		}
	}
	bool DragonTransparent(BlockType a_block)
	{
		if (Fire(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Light:
				return true;
			default: return false;
		}
	}
	bool DripstoneReplaceableBlocks(BlockType a_block)
	{
		return BaseStoneOverworld(a_block);
	}
	bool EmeraldOres(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::EmeraldOre:
			case BlockType::DeepslateEmeraldOre:
				return true;
			default: return false;
		}
	}
	bool EnchantmentPowerProvider(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Bookshelf:
				return true;
			default: return false;
		}
	}
	bool EnchantmentPowerTransmitter(BlockType a_block)
	{
		return Replaceable(a_block);
	}
	bool EndermanHoldable(BlockType a_block)
	{
		if (SmallFlowers(a_block) || Dirt(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Sand:
			case BlockType::RedSand:
			case BlockType::Gravel:
			case BlockType::BrownMushroom:
			case BlockType::RedMushroom:
			case BlockType::Tnt:
			case BlockType::Cactus:
			case BlockType::Clay:
			case BlockType::Pumpkin:
			case BlockType::CarvedPumpkin:
			case BlockType::Melon:
			case BlockType::CrimsonFungus:
			case BlockType::CrimsonNylium:
			case BlockType::CrimsonRoots:
			case BlockType::WarpedFungus:
			case BlockType::WarpedNylium:
			case BlockType::WarpedRoots:
				return true;
			default: return false;
		}
	}
	bool FallDamageResetting(BlockType a_block)
	{
		if (Climbable(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::SweetBerryBush:
			case BlockType::Cobweb:
				return true;
			default: return false;
		}
	}
	bool FeaturesCannotReplace(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Bedrock:
			case BlockType::Spawner:
			case BlockType::Chest:
			case BlockType::EndPortalFrame:
			case BlockType::ReinforcedDeepslate:
			case BlockType::TrialSpawner:
			case BlockType::Vault:
				return true;
			default: return false;
		}
	}
	bool Fences(BlockType a_block)
	{
		if (WoodenFences(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::NetherBrickFence:
				return true;
			default: return false;
		}
	}
	bool FenceGates(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::AcaciaFenceGate:
			case BlockType::BirchFenceGate:
			case BlockType::DarkOakFenceGate:
			case BlockType::JungleFenceGate:
			case BlockType::OakFenceGate:
			case BlockType::SpruceFenceGate:
			case BlockType::CrimsonFenceGate:
			case BlockType::WarpedFenceGate:
			case BlockType::MangroveFenceGate:
			case BlockType::BambooFenceGate:
			case BlockType::CherryFenceGate:
				return true;
			default: return false;
		}
	}
	bool Fire(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Fire:
			case BlockType::SoulFire:
				return true;
			default: return false;
		}
	}
	bool Flowers(BlockType a_block)
	{
		if (SmallFlowers(a_block) || TallFlowers(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::FloweringAzaleaLeaves:
			case BlockType::FloweringAzalea:
			case BlockType::MangrovePropagule:
			case BlockType::CherryLeaves:
			case BlockType::PinkPetals:
			case BlockType::ChorusFlower:
			case BlockType::SporeBlossom:
				return true;
			default: return false;
		}
	}
	bool FlowerPots(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::FlowerPot:
			case BlockType::PottedPoppy:
			case BlockType::PottedBlueOrchid:
			case BlockType::PottedAllium:
			case BlockType::PottedAzureBluet:
			case BlockType::PottedRedTulip:
			case BlockType::PottedOrangeTulip:
			case BlockType::PottedWhiteTulip:
			case BlockType::PottedPinkTulip:
			case BlockType::PottedOxeyeDaisy:
			case BlockType::PottedDandelion:
			case BlockType::PottedOakSapling:
			case BlockType::PottedSpruceSapling:
			case BlockType::PottedBirchSapling:
			case BlockType::PottedJungleSapling:
			case BlockType::PottedAcaciaSapling:
			case BlockType::PottedDarkOakSapling:
			case BlockType::PottedRedMushroom:
			case BlockType::PottedBrownMushroom:
			case BlockType::PottedDeadBush:
			case BlockType::PottedFern:
			case BlockType::PottedCactus:
			case BlockType::PottedCornflower:
			case BlockType::PottedLilyOfTheValley:
			case BlockType::PottedWitherRose:
			case BlockType::PottedBamboo:
			case BlockType::PottedCrimsonFungus:
			case BlockType::PottedWarpedFungus:
			case BlockType::PottedCrimsonRoots:
			case BlockType::PottedWarpedRoots:
			case BlockType::PottedAzaleaBush:
			case BlockType::PottedFloweringAzaleaBush:
			case BlockType::PottedMangrovePropagule:
			case BlockType::PottedCherrySapling:
			case BlockType::PottedTorchflower:
				return true;
			default: return false;
		}
	}
	bool FoxesSpawnableOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::GrassBlock:
			case BlockType::Snow:
			case BlockType::SnowBlock:
			case BlockType::Podzol:
			case BlockType::CoarseDirt:
				return true;
			default: return false;
		}
	}
	bool FrogsSpawnableOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::GrassBlock:
			case BlockType::Mud:
			case BlockType::MangroveRoots:
			case BlockType::MuddyMangroveRoots:
				return true;
			default: return false;
		}
	}
	bool FrogPreferJumpTo(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::LilyPad:
			case BlockType::BigDripleaf:
				return true;
			default: return false;
		}
	}
	bool GeodeInvalidBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Bedrock:
			case BlockType::Water:
			case BlockType::Lava:
			case BlockType::Ice:
			case BlockType::PackedIce:
			case BlockType::BlueIce:
				return true;
			default: return false;
		}
	}
	bool GoatsSpawnableOn(BlockType a_block)
	{
		if (AnimalsSpawnableOn(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Stone:
			case BlockType::Snow:
			case BlockType::SnowBlock:
			case BlockType::PackedIce:
			case BlockType::Gravel:
				return true;
			default: return false;
		}
	}
	bool GoldOres(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::GoldOre:
			case BlockType::NetherGoldOre:
			case BlockType::DeepslateGoldOre:
				return true;
			default: return false;
		}
	}
	bool GuardedByPiglins(BlockType a_block)
	{
		if (ShulkerBoxes(a_block) || GoldOres(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::GoldBlock:
			case BlockType::Barrel:
			case BlockType::Chest:
			case BlockType::EnderChest:
			case BlockType::GildedBlackstone:
			case BlockType::TrappedChest:
			case BlockType::RawGoldBlock:
				return true;
			default: return false;
		}
	}
	bool HoglinRepellents(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::WarpedFungus:
			case BlockType::PottedWarpedFungus:
			case BlockType::NetherPortal:
			case BlockType::RespawnAnchor:
				return true;
			default: return false;
		}
	}
	bool Ice(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Ice:
			case BlockType::PackedIce:
			case BlockType::BlueIce:
			case BlockType::FrostedIce:
				return true;
			default: return false;
		}
	}
	bool Impermeable(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Glass:
			case BlockType::WhiteStainedGlass:
			case BlockType::OrangeStainedGlass:
			case BlockType::MagentaStainedGlass:
			case BlockType::LightBlueStainedGlass:
			case BlockType::YellowStainedGlass:
			case BlockType::LimeStainedGlass:
			case BlockType::PinkStainedGlass:
			case BlockType::GrayStainedGlass:
			case BlockType::LightGrayStainedGlass:
			case BlockType::CyanStainedGlass:
			case BlockType::PurpleStainedGlass:
			case BlockType::BlueStainedGlass:
			case BlockType::BrownStainedGlass:
			case BlockType::GreenStainedGlass:
			case BlockType::RedStainedGlass:
			case BlockType::BlackStainedGlass:
			case BlockType::TintedGlass:
				return true;
			default: return false;
		}
	}
	bool IncorrectForDiamondTool(BlockType a_block)
	{
		return false;
	}
	bool IncorrectForGoldTool(BlockType a_block)
	{
		return NeedsDiamondTool(a_block) || NeedsIronTool(a_block) || NeedsStoneTool(a_block);
	}
	bool IncorrectForIronTool(BlockType a_block)
	{
		return NeedsDiamondTool(a_block);
	}
	bool IncorrectForNetheriteTool(BlockType a_block)
	{
		return false;
	}
	bool IncorrectForStoneTool(BlockType a_block)
	{
		return NeedsDiamondTool(a_block) || NeedsIronTool(a_block);
	}
	bool IncorrectForWoodenTool(BlockType a_block)
	{
		return NeedsDiamondTool(a_block) || NeedsIronTool(a_block) || NeedsStoneTool(a_block);
	}
	bool InfiniburnEnd(BlockType a_block)
	{
		if (InfiniburnOverworld(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Bedrock:
				return true;
			default: return false;
		}
	}
	bool InfiniburnNether(BlockType a_block)
	{
		return InfiniburnOverworld(a_block);
	}
	bool InfiniburnOverworld(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Netherrack:
			case BlockType::MagmaBlock:
				return true;
			default: return false;
		}
	}
	bool InsideStepSoundBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::PowderSnow:
			case BlockType::SculkVein:
			case BlockType::GlowLichen:
			case BlockType::LilyPad:
			case BlockType::SmallAmethystBud:
			case BlockType::PinkPetals:
				return true;
			default: return false;
		}
	}
	bool InvalidSpawnInside(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::EndPortal:
			case BlockType::EndGateway:
				return true;
			default: return false;
		}
	}
	bool IronOres(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::IronOre:
			case BlockType::DeepslateIronOre:
				return true;
			default: return false;
		}
	}
	bool JungleLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::JungleLog:
			case BlockType::JungleWood:
			case BlockType::StrippedJungleLog:
			case BlockType::StrippedJungleWood:
				return true;
			default: return false;
		}
	}
	bool LapisOres(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::LapisOre:
			case BlockType::DeepslateLapisOre:
				return true;
			default: return false;
		}
	}
	bool LavaPoolStoneCannotReplace(BlockType a_block)
	{
		return FeaturesCannotReplace(a_block) || Leaves(a_block) || Logs(a_block);
	}
	bool Leaves(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::JungleLeaves:
			case BlockType::OakLeaves:
			case BlockType::SpruceLeaves:
			case BlockType::DarkOakLeaves:
			case BlockType::AcaciaLeaves:
			case BlockType::BirchLeaves:
			case BlockType::AzaleaLeaves:
			case BlockType::FloweringAzaleaLeaves:
			case BlockType::MangroveLeaves:
			case BlockType::CherryLeaves:
				return true;
			default: return false;
		}
	}
	bool Logs(BlockType a_block)
	{
		return LogsThatBurn(a_block) || CrimsonStems(a_block) || WarpedStems(a_block);
	}
	bool LogsThatBurn(BlockType a_block)
	{
		return DarkOakLogs(a_block) || OakLogs(a_block) || AcaciaLogs(a_block) || BirchLogs(a_block) || JungleLogs(a_block) || SpruceLogs(a_block) || MangroveLogs(a_block) || CherryLogs(a_block);
	}
	bool LushGroundReplaceable(BlockType a_block)
	{
		if (MossReplaceable(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Clay:
			case BlockType::Gravel:
			case BlockType::Sand:
				return true;
			default: return false;
		}
	}
	bool MaintainsFarmland(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::PumpkinStem:
			case BlockType::AttachedPumpkinStem:
			case BlockType::MelonStem:
			case BlockType::AttachedMelonStem:
			case BlockType::Beetroots:
			case BlockType::Carrots:
			case BlockType::Potatoes:
			case BlockType::TorchflowerCrop:
			case BlockType::Torchflower:
			case BlockType::PitcherCrop:
			case BlockType::Wheat:
				return true;
			default: return false;
		}
	}
	bool MangroveLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::MangroveLog:
			case BlockType::MangroveWood:
			case BlockType::StrippedMangroveLog:
			case BlockType::StrippedMangroveWood:
				return true;
			default: return false;
		}
	}
	bool MangroveLogsCanGrowThrough(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Mud:
			case BlockType::MuddyMangroveRoots:
			case BlockType::MangroveRoots:
			case BlockType::MangroveLeaves:
			case BlockType::MangroveLog:
			case BlockType::MangrovePropagule:
			case BlockType::MossCarpet:
			case BlockType::Vine:
				return true;
			default: return false;
		}
	}
	bool MangroveRootsCanGrowThrough(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Mud:
			case BlockType::MuddyMangroveRoots:
			case BlockType::MangroveRoots:
			case BlockType::MossCarpet:
			case BlockType::Vine:
			case BlockType::MangrovePropagule:
			case BlockType::Snow:
				return true;
			default: return false;
		}
	}
	bool MobInteractableDoors(BlockType a_block)
	{
		if (WoodenDoors(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::CopperDoor:
			case BlockType::ExposedCopperDoor:
			case BlockType::WeatheredCopperDoor:
			case BlockType::OxidizedCopperDoor:
			case BlockType::WaxedCopperDoor:
			case BlockType::WaxedExposedCopperDoor:
			case BlockType::WaxedWeatheredCopperDoor:
			case BlockType::WaxedOxidizedCopperDoor:
				return true;
			default: return false;
		}
	}
	bool MooshroomsSpawnableOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Mycelium:
				return true;
			default: return false;
		}
	}
	bool MossReplaceable(BlockType a_block)
	{
		return BaseStoneOverworld(a_block) || CaveVines(a_block) || Dirt(a_block);
	}
	bool MushroomGrowBlock(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Mycelium:
			case BlockType::Podzol:
			case BlockType::CrimsonNylium:
			case BlockType::WarpedNylium:
				return true;
			default: return false;
		}
	}
	bool NeedsDiamondTool(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Obsidian:
			case BlockType::CryingObsidian:
			case BlockType::NetheriteBlock:
			case BlockType::RespawnAnchor:
			case BlockType::AncientDebris:
				return true;
			default: return false;
		}
	}
	bool NeedsIronTool(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::DiamondBlock:
			case BlockType::DiamondOre:
			case BlockType::DeepslateDiamondOre:
			case BlockType::EmeraldOre:
			case BlockType::DeepslateEmeraldOre:
			case BlockType::EmeraldBlock:
			case BlockType::GoldBlock:
			case BlockType::RawGoldBlock:
			case BlockType::GoldOre:
			case BlockType::DeepslateGoldOre:
			case BlockType::RedstoneOre:
			case BlockType::DeepslateRedstoneOre:
				return true;
			default: return false;
		}
	}
	bool NeedsStoneTool(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::IronBlock:
			case BlockType::RawIronBlock:
			case BlockType::IronOre:
			case BlockType::DeepslateIronOre:
			case BlockType::LapisBlock:
			case BlockType::LapisOre:
			case BlockType::DeepslateLapisOre:
			case BlockType::CopperBlock:
			case BlockType::RawCopperBlock:
			case BlockType::CopperOre:
			case BlockType::DeepslateCopperOre:
			case BlockType::CutCopperSlab:
			case BlockType::CutCopperStairs:
			case BlockType::CutCopper:
			case BlockType::WeatheredCopper:
			case BlockType::WeatheredCutCopperSlab:
			case BlockType::WeatheredCutCopperStairs:
			case BlockType::WeatheredCutCopper:
			case BlockType::OxidizedCopper:
			case BlockType::OxidizedCutCopperSlab:
			case BlockType::OxidizedCutCopperStairs:
			case BlockType::OxidizedCutCopper:
			case BlockType::ExposedCopper:
			case BlockType::ExposedCutCopperSlab:
			case BlockType::ExposedCutCopperStairs:
			case BlockType::ExposedCutCopper:
			case BlockType::WaxedCopperBlock:
			case BlockType::WaxedCutCopperSlab:
			case BlockType::WaxedCutCopperStairs:
			case BlockType::WaxedCutCopper:
			case BlockType::WaxedWeatheredCopper:
			case BlockType::WaxedWeatheredCutCopperSlab:
			case BlockType::WaxedWeatheredCutCopperStairs:
			case BlockType::WaxedWeatheredCutCopper:
			case BlockType::WaxedExposedCopper:
			case BlockType::WaxedExposedCutCopperSlab:
			case BlockType::WaxedExposedCutCopperStairs:
			case BlockType::WaxedExposedCutCopper:
			case BlockType::WaxedOxidizedCopper:
			case BlockType::WaxedOxidizedCutCopperSlab:
			case BlockType::WaxedOxidizedCutCopperStairs:
			case BlockType::WaxedOxidizedCutCopper:
			case BlockType::LightningRod:
			case BlockType::Crafter:
			case BlockType::ChiseledCopper:
			case BlockType::ExposedChiseledCopper:
			case BlockType::WeatheredChiseledCopper:
			case BlockType::OxidizedChiseledCopper:
			case BlockType::WaxedChiseledCopper:
			case BlockType::WaxedExposedChiseledCopper:
			case BlockType::WaxedWeatheredChiseledCopper:
			case BlockType::WaxedOxidizedChiseledCopper:
			case BlockType::CopperGrate:
			case BlockType::ExposedCopperGrate:
			case BlockType::WeatheredCopperGrate:
			case BlockType::OxidizedCopperGrate:
			case BlockType::WaxedCopperGrate:
			case BlockType::WaxedExposedCopperGrate:
			case BlockType::WaxedWeatheredCopperGrate:
			case BlockType::WaxedOxidizedCopperGrate:
			case BlockType::CopperBulb:
			case BlockType::ExposedCopperBulb:
			case BlockType::WeatheredCopperBulb:
			case BlockType::OxidizedCopperBulb:
			case BlockType::WaxedCopperBulb:
			case BlockType::WaxedExposedCopperBulb:
			case BlockType::WaxedWeatheredCopperBulb:
			case BlockType::WaxedOxidizedCopperBulb:
			case BlockType::CopperTrapdoor:
			case BlockType::ExposedCopperTrapdoor:
			case BlockType::WeatheredCopperTrapdoor:
			case BlockType::OxidizedCopperTrapdoor:
			case BlockType::WaxedCopperTrapdoor:
			case BlockType::WaxedExposedCopperTrapdoor:
			case BlockType::WaxedWeatheredCopperTrapdoor:
			case BlockType::WaxedOxidizedCopperTrapdoor:
			case BlockType::CopperDoor:
			case BlockType::ExposedCopperDoor:
			case BlockType::WeatheredCopperDoor:
			case BlockType::OxidizedCopperDoor:
			case BlockType::WaxedCopperDoor:
			case BlockType::WaxedExposedCopperDoor:
			case BlockType::WaxedWeatheredCopperDoor:
			case BlockType::WaxedOxidizedCopperDoor:
				return true;
			default: return false;
		}
	}
	bool NetherCarverReplaceables(BlockType a_block)
	{
		if (BaseStoneOverworld(a_block) || BaseStoneNether(a_block) || Dirt(a_block) || Nylium(a_block) || WartBlocks(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::SoulSand:
			case BlockType::SoulSoil:
				return true;
			default: return false;
		}
	}
	bool Nylium(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::CrimsonNylium:
			case BlockType::WarpedNylium:
				return true;
			default: return false;
		}
	}
	bool OakLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakLog:
			case BlockType::OakWood:
			case BlockType::StrippedOakLog:
			case BlockType::StrippedOakWood:
				return true;
			default: return false;
		}
	}
	bool OccludesVibrationSignals(BlockType a_block)
	{
		return Wool(a_block);
	}
	bool OverworldCarverReplaceables(BlockType a_block)
	{
		if (BaseStoneOverworld(a_block) || Dirt(a_block) || Sand(a_block) || Terracotta(a_block) || IronOres(a_block) || CopperOres(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Water:
			case BlockType::Gravel:
			case BlockType::SuspiciousGravel:
			case BlockType::Sandstone:
			case BlockType::RedSandstone:
			case BlockType::Calcite:
			case BlockType::Snow:
			case BlockType::PackedIce:
			case BlockType::RawIronBlock:
			case BlockType::RawCopperBlock:
				return true;
			default: return false;
		}
	}
	bool OverworldNaturalLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::AcaciaLog:
			case BlockType::BirchLog:
			case BlockType::OakLog:
			case BlockType::JungleLog:
			case BlockType::SpruceLog:
			case BlockType::DarkOakLog:
			case BlockType::MangroveLog:
			case BlockType::CherryLog:
				return true;
			default: return false;
		}
	}
	bool ParrotsSpawnableOn(BlockType a_block)
	{
		if (Leaves(a_block) || Logs(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::GrassBlock:
			case BlockType::Air:
				return true;
			default: return false;
		}
	}
	bool PiglinRepellents(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::SoulFire:
			case BlockType::SoulTorch:
			case BlockType::SoulLantern:
			case BlockType::SoulWallTorch:
			case BlockType::SoulCampfire:
				return true;
			default: return false;
		}
	}
	bool Planks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakPlanks:
			case BlockType::SprucePlanks:
			case BlockType::BirchPlanks:
			case BlockType::JunglePlanks:
			case BlockType::AcaciaPlanks:
			case BlockType::DarkOakPlanks:
			case BlockType::CrimsonPlanks:
			case BlockType::WarpedPlanks:
			case BlockType::MangrovePlanks:
			case BlockType::BambooPlanks:
			case BlockType::CherryPlanks:
				return true;
			default: return false;
		}
	}
	bool PolarBearsSpawnableOnAlternate(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Ice:
				return true;
			default: return false;
		}
	}
	bool Portals(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::NetherPortal:
			case BlockType::EndPortal:
			case BlockType::EndGateway:
				return true;
			default: return false;
		}
	}
	bool PressurePlates(BlockType a_block)
	{
		if (WoodenPressurePlates(a_block) || StonePressurePlates(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::LightWeightedPressurePlate:
			case BlockType::HeavyWeightedPressurePlate:
				return true;
			default: return false;
		}
	}
	bool PreventMobSpawningInside(BlockType a_block)
	{
		return Rails(a_block);
	}
	bool RabbitsSpawnableOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::GrassBlock:
			case BlockType::Snow:
			case BlockType::SnowBlock:
			case BlockType::Sand:
				return true;
			default: return false;
		}
	}
	bool Rails(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Rail:
			case BlockType::PoweredRail:
			case BlockType::DetectorRail:
			case BlockType::ActivatorRail:
				return true;
			default: return false;
		}
	}
	bool RedstoneOres(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::RedstoneOre:
			case BlockType::DeepslateRedstoneOre:
				return true;
			default: return false;
		}
	}
	bool Replaceable(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Air:
			case BlockType::Water:
			case BlockType::Lava:
			case BlockType::ShortGrass:
			case BlockType::Fern:
			case BlockType::DeadBush:
			case BlockType::Seagrass:
			case BlockType::TallSeagrass:
			case BlockType::Fire:
			case BlockType::SoulFire:
			case BlockType::Snow:
			case BlockType::Vine:
			case BlockType::GlowLichen:
			case BlockType::Light:
			case BlockType::TallGrass:
			case BlockType::LargeFern:
			case BlockType::StructureVoid:
			case BlockType::VoidAir:
			case BlockType::CaveAir:
			case BlockType::BubbleColumn:
			case BlockType::WarpedRoots:
			case BlockType::NetherSprouts:
			case BlockType::CrimsonRoots:
			case BlockType::HangingRoots:
				return true;
			default: return false;
		}
	}
	bool ReplaceableByTrees(BlockType a_block)
	{
		if (Leaves(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::ShortGrass:
			case BlockType::Fern:
			case BlockType::DeadBush:
			case BlockType::Vine:
			case BlockType::GlowLichen:
			case BlockType::Sunflower:
			case BlockType::Lilac:
			case BlockType::RoseBush:
			case BlockType::Peony:
			case BlockType::TallGrass:
			case BlockType::LargeFern:
			case BlockType::HangingRoots:
			case BlockType::PitcherPlant:
			case BlockType::Water:
			case BlockType::Seagrass:
			case BlockType::TallSeagrass:
			case BlockType::WarpedRoots:
			case BlockType::NetherSprouts:
			case BlockType::CrimsonRoots:
				return true;
			default: return false;
		}
	}
	bool Sand(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Sand:
			case BlockType::RedSand:
			case BlockType::SuspiciousSand:
				return true;
			default: return false;
		}
	}
	bool Saplings(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakSapling:
			case BlockType::SpruceSapling:
			case BlockType::BirchSapling:
			case BlockType::JungleSapling:
			case BlockType::AcaciaSapling:
			case BlockType::DarkOakSapling:
			case BlockType::Azalea:
			case BlockType::FloweringAzalea:
			case BlockType::MangrovePropagule:
			case BlockType::CherrySapling:
				return true;
			default: return false;
		}
	}
	bool SculkReplaceable(BlockType a_block)
	{
		if (BaseStoneOverworld(a_block) || Dirt(a_block) || Terracotta(a_block) || Nylium(a_block) || BaseStoneNether(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Sand:
			case BlockType::RedSand:
			case BlockType::Gravel:
			case BlockType::SoulSand:
			case BlockType::SoulSoil:
			case BlockType::Calcite:
			case BlockType::SmoothBasalt:
			case BlockType::Clay:
			case BlockType::DripstoneBlock:
			case BlockType::EndStone:
			case BlockType::RedSandstone:
			case BlockType::Sandstone:
				return true;
			default: return false;
		}
	}
	bool SculkReplaceableWorldGen(BlockType a_block)
	{
		if (SculkReplaceable(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::DeepslateBricks:
			case BlockType::DeepslateTiles:
			case BlockType::CobbledDeepslate:
			case BlockType::CrackedDeepslateBricks:
			case BlockType::CrackedDeepslateTiles:
			case BlockType::PolishedDeepslate:
				return true;
			default: return false;
		}
	}
	bool ShulkerBoxes(BlockType a_block)
	{
		switch (a_block)
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
	bool Signs(BlockType a_block)
	{
		return StandingSigns(a_block) || WallSigns(a_block);
	}
	bool Slabs(BlockType a_block)
	{
		if (WoodenSlabs(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::BambooMosaicSlab:
			case BlockType::StoneSlab:
			case BlockType::SmoothStoneSlab:
			case BlockType::StoneBrickSlab:
			case BlockType::SandstoneSlab:
			case BlockType::PurpurSlab:
			case BlockType::QuartzSlab:
			case BlockType::RedSandstoneSlab:
			case BlockType::BrickSlab:
			case BlockType::CobblestoneSlab:
			case BlockType::NetherBrickSlab:
			case BlockType::PetrifiedOakSlab:
			case BlockType::PrismarineSlab:
			case BlockType::PrismarineBrickSlab:
			case BlockType::DarkPrismarineSlab:
			case BlockType::PolishedGraniteSlab:
			case BlockType::SmoothRedSandstoneSlab:
			case BlockType::MossyStoneBrickSlab:
			case BlockType::PolishedDioriteSlab:
			case BlockType::MossyCobblestoneSlab:
			case BlockType::EndStoneBrickSlab:
			case BlockType::SmoothSandstoneSlab:
			case BlockType::SmoothQuartzSlab:
			case BlockType::GraniteSlab:
			case BlockType::AndesiteSlab:
			case BlockType::RedNetherBrickSlab:
			case BlockType::PolishedAndesiteSlab:
			case BlockType::DioriteSlab:
			case BlockType::CutSandstoneSlab:
			case BlockType::CutRedSandstoneSlab:
			case BlockType::BlackstoneSlab:
			case BlockType::PolishedBlackstoneBrickSlab:
			case BlockType::PolishedBlackstoneSlab:
			case BlockType::CobbledDeepslateSlab:
			case BlockType::PolishedDeepslateSlab:
			case BlockType::DeepslateTileSlab:
			case BlockType::DeepslateBrickSlab:
			case BlockType::WaxedWeatheredCutCopperSlab:
			case BlockType::WaxedExposedCutCopperSlab:
			case BlockType::WaxedCutCopperSlab:
			case BlockType::OxidizedCutCopperSlab:
			case BlockType::WeatheredCutCopperSlab:
			case BlockType::ExposedCutCopperSlab:
			case BlockType::CutCopperSlab:
			case BlockType::WaxedOxidizedCutCopperSlab:
			case BlockType::MudBrickSlab:
			case BlockType::TuffSlab:
			case BlockType::PolishedTuffSlab:
			case BlockType::TuffBrickSlab:
				return true;
			default: return false;
		}
	}
	bool SmallDripleafPlaceable(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Clay:
			case BlockType::MossBlock:
				return true;
			default: return false;
		}
	}
	bool SmallFlowers(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Dandelion:
			case BlockType::Poppy:
			case BlockType::BlueOrchid:
			case BlockType::Allium:
			case BlockType::AzureBluet:
			case BlockType::RedTulip:
			case BlockType::OrangeTulip:
			case BlockType::WhiteTulip:
			case BlockType::PinkTulip:
			case BlockType::OxeyeDaisy:
			case BlockType::Cornflower:
			case BlockType::LilyOfTheValley:
			case BlockType::WitherRose:
			case BlockType::Torchflower:
				return true;
			default: return false;
		}
	}
	bool SmeltsToGlass(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Sand:
			case BlockType::RedSand:
				return true;
			default: return false;
		}
	}
	bool SnapsGoatHorn(BlockType a_block)
	{
		if (OverworldNaturalLogs(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Stone:
			case BlockType::PackedIce:
			case BlockType::IronOre:
			case BlockType::CoalOre:
			case BlockType::CopperOre:
			case BlockType::EmeraldOre:
				return true;
			default: return false;
		}
	}
	bool SnifferDiggableBlock(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Dirt:
			case BlockType::GrassBlock:
			case BlockType::Podzol:
			case BlockType::CoarseDirt:
			case BlockType::RootedDirt:
			case BlockType::MossBlock:
			case BlockType::Mud:
			case BlockType::MuddyMangroveRoots:
				return true;
			default: return false;
		}
	}
	bool SnifferEggHatchBoost(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::MossBlock:
				return true;
			default: return false;
		}
	}
	bool Snow(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Snow:
			case BlockType::SnowBlock:
			case BlockType::PowderSnow:
				return true;
			default: return false;
		}
	}
	bool SnowLayerCannotSurviveOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Ice:
			case BlockType::PackedIce:
			case BlockType::Barrier:
				return true;
			default: return false;
		}
	}
	bool SnowLayerCanSurviveOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::HoneyBlock:
			case BlockType::SoulSand:
			case BlockType::Mud:
				return true;
			default: return false;
		}
	}
	bool SoulFireBaseBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::SoulSand:
			case BlockType::SoulSoil:
				return true;
			default: return false;
		}
	}
	bool SoulSpeedBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::SoulSand:
			case BlockType::SoulSoil:
				return true;
			default: return false;
		}
	}
	bool SpruceLogs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::SpruceLog:
			case BlockType::SpruceWood:
			case BlockType::StrippedSpruceLog:
			case BlockType::StrippedSpruceWood:
				return true;
			default: return false;
		}
	}
	bool Stairs(BlockType a_block)
	{
		if (WoodenStairs(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::BambooMosaicStairs:
			case BlockType::CobblestoneStairs:
			case BlockType::SandstoneStairs:
			case BlockType::NetherBrickStairs:
			case BlockType::StoneBrickStairs:
			case BlockType::BrickStairs:
			case BlockType::PurpurStairs:
			case BlockType::QuartzStairs:
			case BlockType::RedSandstoneStairs:
			case BlockType::PrismarineBrickStairs:
			case BlockType::PrismarineStairs:
			case BlockType::DarkPrismarineStairs:
			case BlockType::PolishedGraniteStairs:
			case BlockType::SmoothRedSandstoneStairs:
			case BlockType::MossyStoneBrickStairs:
			case BlockType::PolishedDioriteStairs:
			case BlockType::MossyCobblestoneStairs:
			case BlockType::EndStoneBrickStairs:
			case BlockType::StoneStairs:
			case BlockType::SmoothSandstoneStairs:
			case BlockType::SmoothQuartzStairs:
			case BlockType::GraniteStairs:
			case BlockType::AndesiteStairs:
			case BlockType::RedNetherBrickStairs:
			case BlockType::PolishedAndesiteStairs:
			case BlockType::DioriteStairs:
			case BlockType::BlackstoneStairs:
			case BlockType::PolishedBlackstoneBrickStairs:
			case BlockType::PolishedBlackstoneStairs:
			case BlockType::CobbledDeepslateStairs:
			case BlockType::PolishedDeepslateStairs:
			case BlockType::DeepslateTileStairs:
			case BlockType::DeepslateBrickStairs:
			case BlockType::OxidizedCutCopperStairs:
			case BlockType::WeatheredCutCopperStairs:
			case BlockType::ExposedCutCopperStairs:
			case BlockType::CutCopperStairs:
			case BlockType::WaxedWeatheredCutCopperStairs:
			case BlockType::WaxedExposedCutCopperStairs:
			case BlockType::WaxedCutCopperStairs:
			case BlockType::WaxedOxidizedCutCopperStairs:
			case BlockType::MudBrickStairs:
			case BlockType::TuffStairs:
			case BlockType::PolishedTuffStairs:
			case BlockType::TuffBrickStairs:
				return true;
			default: return false;
		}
	}
	bool StandingSigns(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakSign:
			case BlockType::SpruceSign:
			case BlockType::BirchSign:
			case BlockType::AcaciaSign:
			case BlockType::JungleSign:
			case BlockType::DarkOakSign:
			case BlockType::CrimsonSign:
			case BlockType::WarpedSign:
			case BlockType::MangroveSign:
			case BlockType::BambooSign:
			case BlockType::CherrySign:
				return true;
			default: return false;
		}
	}
	bool StoneBricks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::StoneBricks:
			case BlockType::MossyStoneBricks:
			case BlockType::CrackedStoneBricks:
			case BlockType::ChiseledStoneBricks:
				return true;
			default: return false;
		}
	}
	bool StoneButtons(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::StoneButton:
			case BlockType::PolishedBlackstoneButton:
				return true;
			default: return false;
		}
	}
	bool StoneOreReplaceables(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Stone:
			case BlockType::Granite:
			case BlockType::Diorite:
			case BlockType::Andesite:
				return true;
			default: return false;
		}
	}
	bool StonePressurePlates(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::StonePressurePlate:
			case BlockType::PolishedBlackstonePressurePlate:
				return true;
			default: return false;
		}
	}
	bool StriderWarmBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Lava:
				return true;
			default: return false;
		}
	}
	bool SwordEfficient(BlockType a_block)
	{
		if (Leaves(a_block) || Saplings(a_block) || SmallFlowers(a_block) || Crops(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::ShortGrass:
			case BlockType::Fern:
			case BlockType::DeadBush:
			case BlockType::Vine:
			case BlockType::GlowLichen:
			case BlockType::Sunflower:
			case BlockType::Lilac:
			case BlockType::RoseBush:
			case BlockType::Peony:
			case BlockType::TallGrass:
			case BlockType::LargeFern:
			case BlockType::HangingRoots:
			case BlockType::PitcherPlant:
			case BlockType::BrownMushroom:
			case BlockType::RedMushroom:
			case BlockType::SugarCane:
			case BlockType::Pumpkin:
			case BlockType::CarvedPumpkin:
			case BlockType::JackOLantern:
			case BlockType::Melon:
			case BlockType::AttachedPumpkinStem:
			case BlockType::AttachedMelonStem:
			case BlockType::LilyPad:
			case BlockType::Cocoa:
			case BlockType::PitcherCrop:
			case BlockType::SweetBerryBush:
			case BlockType::CaveVines:
			case BlockType::CaveVinesPlant:
			case BlockType::SporeBlossom:
			case BlockType::MossCarpet:
			case BlockType::PinkPetals:
			case BlockType::BigDripleaf:
			case BlockType::BigDripleafStem:
			case BlockType::SmallDripleaf:
			case BlockType::NetherWart:
			case BlockType::WarpedFungus:
			case BlockType::WarpedRoots:
			case BlockType::NetherSprouts:
			case BlockType::CrimsonFungus:
			case BlockType::WeepingVines:
			case BlockType::WeepingVinesPlant:
			case BlockType::TwistingVines:
			case BlockType::TwistingVinesPlant:
			case BlockType::CrimsonRoots:
			case BlockType::ChorusPlant:
			case BlockType::ChorusFlower:
				return true;
			default: return false;
		}
	}
	bool TallFlowers(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Sunflower:
			case BlockType::Lilac:
			case BlockType::Peony:
			case BlockType::RoseBush:
			case BlockType::PitcherPlant:
				return true;
			default: return false;
		}
	}
	bool Terracotta(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Terracotta:
			case BlockType::WhiteTerracotta:
			case BlockType::OrangeTerracotta:
			case BlockType::MagentaTerracotta:
			case BlockType::LightBlueTerracotta:
			case BlockType::YellowTerracotta:
			case BlockType::LimeTerracotta:
			case BlockType::PinkTerracotta:
			case BlockType::GrayTerracotta:
			case BlockType::LightGrayTerracotta:
			case BlockType::CyanTerracotta:
			case BlockType::PurpleTerracotta:
			case BlockType::BlueTerracotta:
			case BlockType::BrownTerracotta:
			case BlockType::GreenTerracotta:
			case BlockType::RedTerracotta:
			case BlockType::BlackTerracotta:
				return true;
			default: return false;
		}
	}
	bool TrailRuinsReplaceable(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Gravel:
				return true;
			default: return false;
		}
	}
	bool Trapdoors(BlockType a_block)
	{
		if (WoodenTrapdoors(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::IronTrapdoor:
			case BlockType::CopperTrapdoor:
			case BlockType::ExposedCopperTrapdoor:
			case BlockType::WeatheredCopperTrapdoor:
			case BlockType::OxidizedCopperTrapdoor:
			case BlockType::WaxedCopperTrapdoor:
			case BlockType::WaxedExposedCopperTrapdoor:
			case BlockType::WaxedWeatheredCopperTrapdoor:
			case BlockType::WaxedOxidizedCopperTrapdoor:
				return true;
			default: return false;
		}
	}
	bool UnderwaterBonemeals(BlockType a_block)
	{
		if (Corals(a_block) || WallCorals(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Seagrass:
				return true;
			default: return false;
		}
	}
	bool UnstableBottomCenter(BlockType a_block)
	{
		return FenceGates(a_block);
	}
	bool ValidSpawn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::GrassBlock:
			case BlockType::Podzol:
				return true;
			default: return false;
		}
	}
	bool VibrationResonators(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::AmethystBlock:
				return true;
			default: return false;
		}
	}
	bool Walls(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::CobblestoneWall:
			case BlockType::MossyCobblestoneWall:
			case BlockType::BrickWall:
			case BlockType::PrismarineWall:
			case BlockType::RedSandstoneWall:
			case BlockType::MossyStoneBrickWall:
			case BlockType::GraniteWall:
			case BlockType::StoneBrickWall:
			case BlockType::NetherBrickWall:
			case BlockType::AndesiteWall:
			case BlockType::RedNetherBrickWall:
			case BlockType::SandstoneWall:
			case BlockType::EndStoneBrickWall:
			case BlockType::DioriteWall:
			case BlockType::BlackstoneWall:
			case BlockType::PolishedBlackstoneBrickWall:
			case BlockType::PolishedBlackstoneWall:
			case BlockType::CobbledDeepslateWall:
			case BlockType::PolishedDeepslateWall:
			case BlockType::DeepslateTileWall:
			case BlockType::DeepslateBrickWall:
			case BlockType::MudBrickWall:
			case BlockType::TuffWall:
			case BlockType::PolishedTuffWall:
			case BlockType::TuffBrickWall:
				return true;
			default: return false;
		}
	}
	bool WallCorals(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::TubeCoralWallFan:
			case BlockType::BrainCoralWallFan:
			case BlockType::BubbleCoralWallFan:
			case BlockType::FireCoralWallFan:
			case BlockType::HornCoralWallFan:
				return true;
			default: return false;
		}
	}
	bool WallHangingSigns(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakWallHangingSign:
			case BlockType::SpruceWallHangingSign:
			case BlockType::BirchWallHangingSign:
			case BlockType::AcaciaWallHangingSign:
			case BlockType::CherryWallHangingSign:
			case BlockType::JungleWallHangingSign:
			case BlockType::DarkOakWallHangingSign:
			case BlockType::CrimsonWallHangingSign:
			case BlockType::WarpedWallHangingSign:
			case BlockType::MangroveWallHangingSign:
			case BlockType::BambooWallHangingSign:
				return true;
			default: return false;
		}
	}
	bool WallPostOverride(BlockType a_block)
	{
		if (Signs(a_block) || Banners(a_block) || PressurePlates(a_block))
		{
			return true;
		}
		switch (a_block)
		{
			case BlockType::Torch:
			case BlockType::SoulTorch:
			case BlockType::RedstoneTorch:
			case BlockType::Tripwire:
				return true;
			default: return false;
		}
	}
	bool WallSigns(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakWallSign:
			case BlockType::SpruceWallSign:
			case BlockType::BirchWallSign:
			case BlockType::AcaciaWallSign:
			case BlockType::JungleWallSign:
			case BlockType::DarkOakWallSign:
			case BlockType::CrimsonWallSign:
			case BlockType::WarpedWallSign:
			case BlockType::MangroveWallSign:
			case BlockType::BambooWallSign:
			case BlockType::CherryWallSign:
				return true;
			default: return false;
		}
	}
	bool WarpedStems(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::WarpedStem:
			case BlockType::StrippedWarpedStem:
			case BlockType::WarpedHyphae:
			case BlockType::StrippedWarpedHyphae:
				return true;
			default: return false;
		}
	}
	bool WartBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::NetherWartBlock:
			case BlockType::WarpedWartBlock:
				return true;
			default: return false;
		}
	}
	bool WitherImmune(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::Barrier:
			case BlockType::Bedrock:
			case BlockType::EndPortal:
			case BlockType::EndPortalFrame:
			case BlockType::EndGateway:
			case BlockType::CommandBlock:
			case BlockType::RepeatingCommandBlock:
			case BlockType::ChainCommandBlock:
			case BlockType::StructureBlock:
			case BlockType::Jigsaw:
			case BlockType::MovingPiston:
			case BlockType::Light:
			case BlockType::ReinforcedDeepslate:
				return true;
			default: return false;
		}
	}
	bool WitherSummonBaseBlocks(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::SoulSand:
			case BlockType::SoulSoil:
				return true;
			default: return false;
		}
	}
	bool WolvesSpawnableOn(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::GrassBlock:
			case BlockType::Snow:
			case BlockType::SnowBlock:
			case BlockType::CoarseDirt:
			case BlockType::Podzol:
				return true;
			default: return false;
		}
	}
	bool WoodenButtons(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakButton:
			case BlockType::SpruceButton:
			case BlockType::BirchButton:
			case BlockType::JungleButton:
			case BlockType::AcaciaButton:
			case BlockType::DarkOakButton:
			case BlockType::CrimsonButton:
			case BlockType::WarpedButton:
			case BlockType::MangroveButton:
			case BlockType::BambooButton:
			case BlockType::CherryButton:
				return true;
			default: return false;
		}
	}
	bool WoodenDoors(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakDoor:
			case BlockType::SpruceDoor:
			case BlockType::BirchDoor:
			case BlockType::JungleDoor:
			case BlockType::AcaciaDoor:
			case BlockType::DarkOakDoor:
			case BlockType::CrimsonDoor:
			case BlockType::WarpedDoor:
			case BlockType::MangroveDoor:
			case BlockType::BambooDoor:
			case BlockType::CherryDoor:
				return true;
			default: return false;
		}
	}
	bool WoodenFences(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakFence:
			case BlockType::AcaciaFence:
			case BlockType::DarkOakFence:
			case BlockType::SpruceFence:
			case BlockType::BirchFence:
			case BlockType::JungleFence:
			case BlockType::CrimsonFence:
			case BlockType::WarpedFence:
			case BlockType::MangroveFence:
			case BlockType::BambooFence:
			case BlockType::CherryFence:
				return true;
			default: return false;
		}
	}
	bool WoodenPressurePlates(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakPressurePlate:
			case BlockType::SprucePressurePlate:
			case BlockType::BirchPressurePlate:
			case BlockType::JunglePressurePlate:
			case BlockType::AcaciaPressurePlate:
			case BlockType::DarkOakPressurePlate:
			case BlockType::CrimsonPressurePlate:
			case BlockType::WarpedPressurePlate:
			case BlockType::MangrovePressurePlate:
			case BlockType::BambooPressurePlate:
			case BlockType::CherryPressurePlate:
				return true;
			default: return false;
		}
	}
	bool WoodenSlabs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakSlab:
			case BlockType::SpruceSlab:
			case BlockType::BirchSlab:
			case BlockType::JungleSlab:
			case BlockType::AcaciaSlab:
			case BlockType::DarkOakSlab:
			case BlockType::CrimsonSlab:
			case BlockType::WarpedSlab:
			case BlockType::MangroveSlab:
			case BlockType::BambooSlab:
			case BlockType::CherrySlab:
				return true;
			default: return false;
		}
	}
	bool WoodenStairs(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::OakStairs:
			case BlockType::SpruceStairs:
			case BlockType::BirchStairs:
			case BlockType::JungleStairs:
			case BlockType::AcaciaStairs:
			case BlockType::DarkOakStairs:
			case BlockType::CrimsonStairs:
			case BlockType::WarpedStairs:
			case BlockType::MangroveStairs:
			case BlockType::BambooStairs:
			case BlockType::CherryStairs:
				return true;
			default: return false;
		}
	}
	bool WoodenTrapdoors(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::AcaciaTrapdoor:
			case BlockType::BirchTrapdoor:
			case BlockType::DarkOakTrapdoor:
			case BlockType::JungleTrapdoor:
			case BlockType::OakTrapdoor:
			case BlockType::SpruceTrapdoor:
			case BlockType::CrimsonTrapdoor:
			case BlockType::WarpedTrapdoor:
			case BlockType::MangroveTrapdoor:
			case BlockType::BambooTrapdoor:
			case BlockType::CherryTrapdoor:
				return true;
			default: return false;
		}
	}
	bool Wool(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::WhiteWool:
			case BlockType::OrangeWool:
			case BlockType::MagentaWool:
			case BlockType::LightBlueWool:
			case BlockType::YellowWool:
			case BlockType::LimeWool:
			case BlockType::PinkWool:
			case BlockType::GrayWool:
			case BlockType::LightGrayWool:
			case BlockType::CyanWool:
			case BlockType::PurpleWool:
			case BlockType::BlueWool:
			case BlockType::BrownWool:
			case BlockType::GreenWool:
			case BlockType::RedWool:
			case BlockType::BlackWool:
				return true;
			default: return false;
		}
	}
	bool WoolCarpets(BlockType a_block)
	{
		switch (a_block)
		{
			case BlockType::WhiteCarpet:
			case BlockType::OrangeCarpet:
			case BlockType::MagentaCarpet:
			case BlockType::LightBlueCarpet:
			case BlockType::YellowCarpet:
			case BlockType::LimeCarpet:
			case BlockType::PinkCarpet:
			case BlockType::GrayCarpet:
			case BlockType::LightGrayCarpet:
			case BlockType::CyanCarpet:
			case BlockType::PurpleCarpet:
			case BlockType::BlueCarpet:
			case BlockType::BrownCarpet:
			case BlockType::GreenCarpet:
			case BlockType::RedCarpet:
			case BlockType::BlackCarpet:
				return true;
			default: return false;
		}
	}
}
