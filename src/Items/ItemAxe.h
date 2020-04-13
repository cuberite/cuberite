
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"





class cItemAxeHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemAxeHandler(int a_ItemType):
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



	virtual float GetBlockBreakingStrength(BLOCKTYPE a_Block) override
	{
		if (!IsBlockMaterialWood(a_Block) && !IsBlockMaterialPlants(a_Block) && !IsBlockMaterialVine(a_Block))
		{
			return Super::GetBlockBreakingStrength(a_Block);
		}
		else
		{
			switch (m_ItemType)
			{
				case E_ITEM_WOODEN_AXE: return 2.0f;
				case E_ITEM_STONE_AXE:  return 4.0f;
				case E_ITEM_IRON_AXE:   return 6.0f;
				case E_ITEM_GOLD_AXE:   return 12.0f;
				case E_ITEM_DIAMOND_AXE: return 8.0f;
			}
		}
		ASSERT(!"Something is wrong here... Maybe they are axes out of a new material?");
		return 1.0f;
	}

} ;
