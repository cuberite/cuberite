
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"


#include "../BlockInServerPluginInterface.h"





class cItemShovelHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemShovelHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}



	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:      return 2;
			case dlaBreakBlock:        return 1;
			case dlaBreakBlockInstant: return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
	}





	virtual bool CanHarvestBlock(BlockState a_Block) override
	{
		if (a_Block.Type() == BlockType::SnowBlock)
		{
			return true;
		}
		return Super::CanHarvestBlock(a_Block);
	}





	virtual bool CanRepairWithRawMaterial(short a_ItemType) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_SHOVEL:
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
			case E_ITEM_STONE_SHOVEL:
			{
				auto NewItem = PaletteUpgrade::FromItem(a_ItemType, 0);
				switch (NewItem)
				{
					case Item::Cobblestone:
						return true;
					default: return false;
				}
			}
			case E_ITEM_IRON_SHOVEL:    return (a_ItemType == E_ITEM_IRON);
			case E_ITEM_GOLD_SHOVEL:    return (a_ItemType == E_ITEM_GOLD);
			case E_ITEM_DIAMOND_SHOVEL: return (a_ItemType == E_ITEM_DIAMOND);
		}
		return false;
	}





	virtual float GetBlockBreakingStrength(BlockState a_Block) override
	{
		switch (a_Block.Type())
		{
			case BlockType::Clay:
			case BlockType::Dirt:
			case BlockType::Farmland:
			case BlockType::GrassBlock:
			case BlockType::GrassPath:
			case BlockType::Gravel:
			case BlockType::Mycelium:
			case BlockType::Sand:
			case BlockType::Snow:
			case BlockType::SnowBlock:
			case BlockType::SoulSand:

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
			{
				switch (m_ItemType)
				{
					case E_ITEM_WOODEN_SHOVEL:  return 2.0f;
					case E_ITEM_STONE_SHOVEL:   return 4.0f;
					case E_ITEM_IRON_SHOVEL:    return 6.0f;
					case E_ITEM_GOLD_SHOVEL:    return 12.0f;
					case E_ITEM_DIAMOND_SHOVEL: return 8.0f;
				}
				break;
			}
			default: break;
		}
		return Super::GetBlockBreakingStrength(a_Block);
	}
};
