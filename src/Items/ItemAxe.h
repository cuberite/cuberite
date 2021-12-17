
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"





class cItemAxeHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemAxeHandler(Item a_ItemType):
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



	virtual float GetBlockBreakingStrength(BlockState a_Block) override
	{
		if (!IsBlockMaterialWood(a_Block) && !IsBlockMaterialPlants(a_Block) && !IsBlockMaterialVine(a_Block))
		{
			return Super::GetBlockBreakingStrength(a_Block);
		}
		else
		{
			switch (m_ItemType)
			{
				case Item::WoodenAxe:  return 2.0f;
				case Item::StoneAxe:   return 4.0f;
				case Item::IronAxe:    return 6.0f;
				case Item::GoldenAxe:  return 12.0f;
				case Item::DiamondAxe: return 8.0f;
				default:
				{
					FLOGWARNING("{}: Item type not handled {}.", __FUNCTION__, m_ItemType);
					return 1.0f;
				}
			}
		}
		ASSERT(!"Something is wrong here... Maybe they are axes out of a new material?");
		return 1.0f;
	}

} ;
