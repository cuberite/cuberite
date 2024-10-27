
#pragma once

#include "../Blocks/BlockAnvil.h"
#include "ItemHandler.h"
#include "../Protocol/Palettes/Upgrade.h"


class cItemPickaxeHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) const override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 2;
			case dlaBreakBlock:         return 1;
			case dlaBreakBlockInstant:  return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
	}



	char PickaxeLevel() const
	{
		switch (m_ItemType)
		{
			case Item::WoodenPickaxe:  return 1;
			case Item::GoldenPickaxe:  return 1;
			case Item::StonePickaxe:   return 2;
			case Item::IronPickaxe:    return 3;
			case Item::DiamondPickaxe: return 4;
			case Item::NetheritePickaxe: return 5;
			default: return 0;
		}
	}

	virtual bool CanHarvestBlock(BlockState a_Block) const override
	{
		// NOTICE: Make sure to update cItemHandler::CanHarvestBlock() if adding new blocks here!
		switch (a_Block.Type())
		{
			case BlockType::Obsidian:
			case BlockType::AncientDebris:
			case BlockType::RespawnAnchor:
			case BlockType::CryingObsidian:
			{
				return PickaxeLevel() >= 4;
			}

			case BlockType::DiamondBlock:
			case BlockType::DiamondOre:
			case BlockType::EmeraldOre:
			case BlockType::GoldBlock:
			case BlockType::GoldOre:
			case BlockType::NetherGoldOre:
			case BlockType::RedstoneOre:
			case BlockType::RawGoldBlock:
			case BlockType::DeepslateDiamondOre:
			case BlockType::DeepslateEmeraldOre:
			case BlockType::DeepslateRedstoneOre:
			case BlockType::DeepslateGoldOre:
			{
				return PickaxeLevel() >= 3;
			}

			case BlockType::IronBlock:
			case BlockType::IronOre:
			case BlockType::LapisOre:
			case BlockType::LapisBlock:
			case BlockType::RawCopperBlock:
			case BlockType::RawIronBlock:
			case BlockType::DeepslateIronOre:
			case BlockType::DeepslateCopperOre:
			{
				return PickaxeLevel() >= 2;
			}

			case BlockType::Anvil:
			case BlockType::ChippedAnvil:
			case BlockType::DamagedAnvil:
			case BlockType::BlackGlazedTerracotta:
			case BlockType::CoalBlock:
			case BlockType::RedstoneBlock:
			case BlockType::BlueGlazedTerracotta:
			case BlockType::BoneBlock:
			case BlockType::BrewingStand:
			case BlockType::Bricks:
			case BlockType::BrownGlazedTerracotta:
			case BlockType::Cauldron:
			case BlockType::CoalOre:
			case BlockType::Cobblestone:
			case BlockType::CobblestoneWall:
			case BlockType::CyanGlazedTerracotta:
			case BlockType::Dispenser:
			case BlockType::Dropper:
			case BlockType::EnchantingTable:
			case BlockType::EndStoneBricks:
			case BlockType::EndStone:
			case BlockType::Furnace:
			case BlockType::GrayGlazedTerracotta:
			case BlockType::GreenGlazedTerracotta:
			case BlockType::HeavyWeightedPressurePlate:
			case BlockType::Hopper:
			case BlockType::IronBars:
			case BlockType::IronDoor:
			case BlockType::IronTrapdoor:
			case BlockType::LightBlueGlazedTerracotta:
			case BlockType::LightGrayGlazedTerracotta:
			case BlockType::LightWeightedPressurePlate:
			case BlockType::LimeGlazedTerracotta:
			case BlockType::MagentaGlazedTerracotta:
			case BlockType::Spawner:
			case BlockType::MossyCobblestone:
			case BlockType::MagmaBlock:
			case BlockType::NetherBricks:
			case BlockType::NetherBrickFence:
			case BlockType::NetherQuartzOre:
			case BlockType::Netherrack:
			case BlockType::Observer:
			case BlockType::OrangeGlazedTerracotta:
			case BlockType::Prismarine:
			case BlockType::PinkGlazedTerracotta:
			case BlockType::PurpleGlazedTerracotta:
			case BlockType::PurpurBlock:
			case BlockType::PurpurPillar:
			case BlockType::QuartzBlock:
			case BlockType::RedGlazedTerracotta:
			case BlockType::RedNetherBricks:
			case BlockType::RedSandstone:
			case BlockType::Sandstone:
			case BlockType::Stone:
			case BlockType::StoneBricks:
			case BlockType::StonePressurePlate:
			case BlockType::WhiteGlazedTerracotta:
			case BlockType::YellowGlazedTerracotta:

				// Concrete
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
				// Slabs
			case BlockType::AcaciaSlab:
			case BlockType::AndesiteSlab:
			case BlockType::BirchSlab:
			case BlockType::BlackstoneSlab:
			case BlockType::BrickSlab:
			case BlockType::CobblestoneSlab:
			case BlockType::CrimsonSlab:
			case BlockType::CutRedSandstoneSlab:
			case BlockType::CutSandstoneSlab:
			case BlockType::DarkOakSlab:
			case BlockType::DarkPrismarineSlab:
			case BlockType::DioriteSlab:
			case BlockType::EndStoneBrickSlab:
			case BlockType::GraniteSlab:
			case BlockType::JungleSlab:
			case BlockType::MossyCobblestoneSlab:
			case BlockType::MossyStoneBrickSlab:
			case BlockType::NetherBrickSlab:
			case BlockType::OakSlab:
			case BlockType::PetrifiedOakSlab:
			case BlockType::PolishedAndesiteSlab:
			case BlockType::PolishedBlackstoneBrickSlab:
			case BlockType::PolishedBlackstoneSlab:
			case BlockType::PolishedDioriteSlab:
			case BlockType::PolishedGraniteSlab:
			case BlockType::PrismarineBrickSlab:
			case BlockType::PrismarineSlab:
			case BlockType::PurpurSlab:
			case BlockType::QuartzSlab:
			case BlockType::RedNetherBrickSlab:
			case BlockType::RedSandstoneSlab:
			case BlockType::SandstoneSlab:
			case BlockType::SmoothQuartzSlab:
			case BlockType::SmoothRedSandstoneSlab:
			case BlockType::SmoothSandstoneSlab:
			case BlockType::SmoothStoneSlab:
			case BlockType::SpruceSlab:
			case BlockType::StoneBrickSlab:
			case BlockType::StoneSlab:
			case BlockType::WarpedSlab:
				// Stairs
			case BlockType::AcaciaStairs:
			case BlockType::AndesiteStairs:
			case BlockType::BirchStairs:
			case BlockType::BlackstoneStairs:
			case BlockType::BrickStairs:
			case BlockType::CobblestoneStairs:
			case BlockType::CrimsonStairs:
			case BlockType::DarkOakStairs:
			case BlockType::DarkPrismarineStairs:
			case BlockType::DioriteStairs:
			case BlockType::EndStoneBrickStairs:
			case BlockType::GraniteStairs:
			case BlockType::JungleStairs:
			case BlockType::MossyCobblestoneStairs:
			case BlockType::MossyStoneBrickStairs:
			case BlockType::NetherBrickStairs:
			case BlockType::OakStairs:
			case BlockType::PolishedAndesiteStairs:
			case BlockType::PolishedBlackstoneBrickStairs:
			case BlockType::PolishedBlackstoneStairs:
			case BlockType::PolishedDioriteStairs:
			case BlockType::PolishedGraniteStairs:
			case BlockType::PrismarineBrickStairs:
			case BlockType::PrismarineStairs:
			case BlockType::QuartzStairs:
			case BlockType::PurpurStairs:
			case BlockType::RedNetherBrickStairs:
			case BlockType::RedSandstoneStairs:
			case BlockType::SandstoneStairs:
			case BlockType::SmoothQuartzStairs:
			case BlockType::SmoothRedSandstoneStairs:
			case BlockType::SmoothSandstoneStairs:
			case BlockType::SpruceStairs:
			case BlockType::StoneBrickStairs:
			case BlockType::StoneStairs:
			case BlockType::WarpedStairs:
			{
				return PickaxeLevel() >= 1;
			}
			default: return Super::CanHarvestBlock(a_Block);
		}
	}

	virtual bool CanRepairWithRawMaterial(const cItem & a_Item) const override
	{
		switch (m_ItemType)
		{
			case Item::WoodenPickaxe:
			{
				switch (a_Item.m_ItemType)
				{
					case Item::AcaciaPlanks:
					case Item::BirchPlanks:
					case Item::CrimsonPlanks:
					case Item::DarkOakPlanks:
					case Item::JunglePlanks:
					case Item::OakPlanks:
					case Item::SprucePlanks:
					case Item::WarpedPlanks:
					case Item::BambooPlanks:
					case Item::CherryPlanks:
					case Item::MangrovePlanks:
						return true;
					default: return false;
				}
			}
			case Item::StonePickaxe:
			{
				switch (a_Item.m_ItemType)
				{
					case Item::Cobblestone:
					case Item::CobbledDeepslate:
					case Item::Blackstone:
						return true;
					default: return false;
				}
			}
			case Item::IronPickaxe:    return (a_Item.m_ItemType == Item::IronIngot);
			case Item::GoldenPickaxe:  return (a_Item.m_ItemType == Item::GoldIngot);
			case Item::DiamondPickaxe: return (a_Item.m_ItemType == Item::Diamond);
			case Item::NetheritePickaxe: return (a_Item.m_ItemType == Item::NetheriteIngot);
			default: return false;
		}
	}


	virtual float GetBlockBreakingStrength(BlockState a_Block) const override
	{
		if (!IsBlockMaterialIron(a_Block) && (IsBlockAnvil(a_Block)) && !IsBlockMaterialRock(a_Block))
		{
			return Super::GetBlockBreakingStrength(a_Block);
		}
		else
		{
			switch (m_ItemType)
			{
				case Item::WoodenPickaxe:  return 2.0f;
				case Item::StonePickaxe:   return 4.0f;
				case Item::IronPickaxe:    return 6.0f;
				case Item::GoldenPickaxe:  return 12.0f;
				case Item::DiamondPickaxe: return 8.0f;
				case Item::NetheritePickaxe: return 9.0f;
				default: return 0.0f;
			}
		}
		ASSERT(!"Something is wrong here... Maybe they are pickaxes out of a new material?");
		return 1.0f;
	}

} ;
