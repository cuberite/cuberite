
#pragma once

#include "../Blocks/BlockAnvil.h"
#include "ItemHandler.h"
#include "../Protocol/Palettes/Upgrade.h"


class cItemPickaxeHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemPickaxeHandler(int a_ItemType):
		Super(a_ItemType)
	{

	}



	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 2;
			case dlaBreakBlock:         return 1;
			case dlaBreakBlockInstant:  return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
	}



	char PickaxeLevel()
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_PICKAXE:  return 1;
			case E_ITEM_GOLD_PICKAXE:    return 1;
			case E_ITEM_STONE_PICKAXE:   return 2;
			case E_ITEM_IRON_PICKAXE:    return 3;
			case E_ITEM_DIAMOND_PICKAXE: return 4;

			default: return 0;
		}
	}

	virtual bool CanHarvestBlock(BlockState a_Block) override
	{
		// NOTICE: Make sure to update cItemHandler::CanHarvestBlock() if adding new blocks here!
		switch (a_Block.Type())
		{
			case BlockType::Obsidian:
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
			{
				return PickaxeLevel() >= 3;
			}

			case BlockType::IronBlock:
			case BlockType::IronOre:
			case BlockType::LapisOre:
			case BlockType::LapisBlock:
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

	virtual bool CanRepairWithRawMaterial(short a_ItemType) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_PICKAXE:
			{
				auto NewItem = PaletteUpgrade::FromItem(a_ItemType, 0);
				switch (NewItem)
				{
					case Item::AcaciaPlanks:
					case Item::BirchPlanks:
					case Item::CrimsonPlanks:
					case Item::DarkOakPlanks:
					case Item::JunglePlanks:
					case Item::OakPlanks:
					case Item::SprucePlanks:
					case Item::WarpedPlanks:
						return true;
					default: return false;
				}
			}
			case E_ITEM_STONE_PICKAXE:
			{
				auto NewItem = PaletteUpgrade::FromItem(a_ItemType, 0);
				switch (NewItem)
				{
					case Item::Cobblestone:
						return true;
					default: return false;
				}
			}
			case E_ITEM_IRON_PICKAXE:    return (a_ItemType == E_ITEM_IRON);
			case E_ITEM_GOLD_PICKAXE:    return (a_ItemType == E_ITEM_GOLD);
			case E_ITEM_DIAMOND_PICKAXE: return (a_ItemType == E_ITEM_DIAMOND);
		}
		return false;
	}


	virtual float GetBlockBreakingStrength(BlockState a_Block) override
	{
		if (!IsBlockMaterialIron(a_Block) && (cBlockAnvilHandler::IsBlockAnvil(a_Block)) && !IsBlockMaterialRock(a_Block))
		{
			return Super::GetBlockBreakingStrength(a_Block);
		}
		else
		{
			switch (m_ItemType)
			{
				case E_ITEM_WOODEN_PICKAXE:  return 2.0f;
				case E_ITEM_STONE_PICKAXE:   return 4.0f;
				case E_ITEM_IRON_PICKAXE:    return 6.0f;
				case E_ITEM_GOLD_PICKAXE:    return 12.0f;
				case E_ITEM_DIAMOND_PICKAXE: return 8.0f;
			}
		}
		ASSERT(!"Something is wrong here... Maybe they are pickaxes out of a new material?");
		return 1.0f;
	}

} ;
