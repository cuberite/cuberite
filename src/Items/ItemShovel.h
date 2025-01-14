
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"


#include "../BlockInServerPluginInterface.h"





class cItemShovelHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;



	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) const override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:      return 2;
			case dlaBreakBlock:        return 1;
			case dlaBreakBlockInstant: return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
	}





	virtual bool CanHarvestBlock(BlockState a_Block) const override
	{
		if (a_Block.Type() == BlockType::SnowBlock)
		{
			return true;
		}
		return Super::CanHarvestBlock(a_Block);
	}





	virtual bool CanRepairWithRawMaterial(const cItem & a_Item) const override
	{
		switch (m_ItemType)
		{
			case Item::WoodenShovel:
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
			case Item::StoneShovel:
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
			case Item::IronShovel:    return (a_Item.m_ItemType == Item::IronIngot);
			case Item::GoldenShovel:  return (a_Item.m_ItemType == Item::GoldIngot);
			case Item::DiamondShovel: return (a_Item.m_ItemType == Item::Diamond);
			case Item::NetheriteShovel: return (a_Item.m_ItemType == Item::NetheriteIngot);
			default:
			{
				LOGWARNING("{}: Item type not handled {}.", __FUNCTION__, m_ItemType);
				return false;
			}
		}
		return false;
	}





	virtual float GetBlockBreakingStrength(BlockState a_Block) const override
	{
		switch (a_Block.Type())
		{
			case BlockType::Clay:
			case BlockType::Dirt:
			case BlockType::Farmland:
			case BlockType::GrassBlock:
			case BlockType::DirtPath:
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
					case Item::WoodenShovel:  return 2.0f;
					case Item::StoneShovel:   return 4.0f;
					case Item::IronShovel:    return 6.0f;
					case Item::GoldenShovel:  return 12.0f;
					case Item::DiamondShovel: return 8.0f;
					default: return 0.0f;
				}
			}
			default: break;
		}
		return Super::GetBlockBreakingStrength(a_Block);
	}
};
