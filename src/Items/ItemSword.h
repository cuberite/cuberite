
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"
#include "../Blocks/BlockLeaves.h"





class cItemSwordHandler final :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool CanHarvestBlock(BlockState a_Block) const override
	{
		if (a_Block.Type() == BlockType::Cobweb)
		{
			return true;
		}
		return Super::CanHarvestBlock(a_Block);
	}


	virtual bool CanRepairWithRawMaterial(const cItem & a_Item) const override
	{
		switch (m_ItemType)
		{
			case Item::WoodenSword:
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
			case Item::StoneSword:
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
			case Item::IronSword:    return (a_Item.m_ItemType == Item::IronIngot);
			case Item::GoldenSword:  return (a_Item.m_ItemType == Item::GoldIngot);
			case Item::DiamondSword: return (a_Item.m_ItemType == Item::Diamond);
			case Item::NetheriteSword: return (a_Item.m_ItemType == Item::NetheriteIngot);
			default: return false;
		}
	}


	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) const override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 1;
			case dlaBreakBlock:         return 2;
			case dlaBreakBlockInstant:  return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
	}



	virtual float GetBlockBreakingStrength(BlockState a_Block) const override
	{
		if (a_Block.Type() == BlockType::Cobweb)
		{
			return 15.0f;
		}
		else
		{
			if (
				IsBlockMaterialPlants(a_Block) ||
				IsBlockMaterialVine(a_Block)   ||
				cBlockLeavesHandler::IsBlockLeaves(a_Block) ||
				IsBlockMaterialGourd(a_Block)
			)
			{
				return 1.5f;
			}
			else
			{
				return 1.0f;
			}
		}
	}

} ;
