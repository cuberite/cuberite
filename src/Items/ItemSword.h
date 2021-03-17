
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"





class cItemSwordHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemSwordHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}


	virtual bool CanHarvestBlock(BlockState a_Block) override
	{
		if (a_Block.Type() == BlockType::Cobweb)
		{
			return true;
		}
		return Super::CanHarvestBlock(a_Block);
	}


	virtual bool CanRepairWithRawMaterial(short a_ItemType) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_SWORD:
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
			case E_ITEM_STONE_SWORD:
			{
				auto NewItem = PaletteUpgrade::FromItem(a_ItemType, 0);
				switch (NewItem)
				{
					case Item::Cobblestone:
						return true;
					default: return false;
				}
			}
			case E_ITEM_IRON_SWORD:    return (a_ItemType == E_ITEM_IRON);
			case E_ITEM_GOLD_SWORD:    return (a_ItemType == E_ITEM_GOLD);
			case E_ITEM_DIAMOND_SWORD: return (a_ItemType == E_ITEM_DIAMOND);
		}
		return false;
	}


	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 1;
			case dlaBreakBlock:         return 2;
			case dlaBreakBlockInstant:  return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
	}



	virtual float GetBlockBreakingStrength(BlockState a_Block) override
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
				IsBlockMaterialLeaves(a_Block) ||
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
