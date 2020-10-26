
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemArmorHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemArmorHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	/** Move the armor to the armor slot of the player's inventory */
	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		int SlotNum;
		if (ItemCategory::IsHelmet(a_HeldItem.m_ItemType))
		{
			SlotNum = 0;
		}
		else if (ItemCategory::IsChestPlate(a_HeldItem.m_ItemType))
		{
			SlotNum = 1;
		}
		else if (ItemCategory::IsLeggings(a_HeldItem.m_ItemType))
		{
			SlotNum = 2;
		}
		else if (ItemCategory::IsBoots(a_HeldItem.m_ItemType))
		{
			SlotNum = 3;
		}
		else
		{
			LOGWARNING("Used unknown armor: %i", a_HeldItem.m_ItemType);
			return false;
		}

		if (!a_Player->GetInventory().GetArmorSlot(SlotNum).IsEmpty())
		{
			return false;
		}

		a_Player->GetInventory().SetArmorSlot(SlotNum, a_HeldItem.CopyOne());

		cItem Item(a_HeldItem);
		Item.m_ItemCount--;
		if (Item.m_ItemCount <= 0)
		{
			Item.Empty();
		}
		a_Player->GetInventory().SetEquippedItem(Item);
		return true;
	}





	virtual bool CanRepairWithRawMaterial(short a_ItemType) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_CHAIN_BOOTS:
			case E_ITEM_CHAIN_CHESTPLATE:
			case E_ITEM_CHAIN_HELMET:
			case E_ITEM_CHAIN_LEGGINGS:
			{
				return (a_ItemType == E_ITEM_IRON);
			}
			case E_ITEM_DIAMOND_BOOTS:
			case E_ITEM_DIAMOND_CHESTPLATE:
			case E_ITEM_DIAMOND_HELMET:
			case E_ITEM_DIAMOND_LEGGINGS:
			{
				return (a_ItemType == E_ITEM_DIAMOND);
			}
			case E_ITEM_IRON_BOOTS:
			case E_ITEM_IRON_CHESTPLATE:
			case E_ITEM_IRON_HELMET:
			case E_ITEM_IRON_LEGGINGS:
			{
				return (a_ItemType == E_ITEM_IRON);
			}
			case E_ITEM_GOLD_BOOTS:
			case E_ITEM_GOLD_CHESTPLATE:
			case E_ITEM_GOLD_HELMET:
			case E_ITEM_GOLD_LEGGINGS:
			{
				return (a_ItemType == E_ITEM_GOLD);
			}
			case E_ITEM_LEATHER_BOOTS:
			case E_ITEM_LEATHER_CAP:
			case E_ITEM_LEATHER_PANTS:
			case E_ITEM_LEATHER_TUNIC:
			{
				return (a_ItemType == E_ITEM_LEATHER);
			}
		}
		return false;
	}

} ;




