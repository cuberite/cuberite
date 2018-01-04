
#pragma once

#include "ItemHandler.h"





class cItemSwordHandler :
	public cItemHandler
{
	typedef cItemHandler super;
public:
	cItemSwordHandler(int a_ItemType)
	: cItemHandler(a_ItemType)
	{
	}


	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		if (a_BlockType == E_BLOCK_COBWEB)
		{
			return true;
		}
		return super::CanHarvestBlock(a_BlockType);
	}


	virtual bool CanRepairWithRawMaterial(short a_ItemType) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_SWORD:  return (a_ItemType == E_BLOCK_PLANKS);
			case E_ITEM_STONE_SWORD:   return (a_ItemType == E_BLOCK_COBBLESTONE);
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
			default:                    return 0;
		}
	}



	virtual float GetBlockBreakingStrength(BLOCKTYPE a_Block) override
	{
		if (a_Block == E_BLOCK_COBWEB)
		{
			return 15.0f;
		}
		else
		{
			if (
				IsBlockMaterialPlants(a_Block) ||
				IsBlockMaterialVine(a_Block)   ||
				IsBlockMaterialCoral(a_Block)  ||
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
