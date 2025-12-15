
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemArmorHandler final :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;





	/** Move the armor to the armor slot of the player's inventory */
	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
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
		a_Player->GetInventory().RemoveOneEquippedItem();
		return true;
	}





	virtual bool CanRepairWithRawMaterial(const cItem & a_Item) const override
	{
		switch (m_ItemType)
		{
			case Item::ChainmailBoots:
			case Item::ChainmailChestplate:
			case Item::ChainmailHelmet:
			case Item::ChainmailLeggings:
			{
				return (a_Item.m_ItemType == Item::IronIngot);
			}
			case Item::DiamondBoots:
			case Item::DiamondChestplate:
			case Item::DiamondHelmet:
			case Item::DiamondLeggings:
			{
				return (a_Item.m_ItemType == Item::Diamond);
			}
			case Item::IronBoots:
			case Item::IronChestplate:
			case Item::IronHelmet:
			case Item::IronLeggings:
			{
				return (a_Item.m_ItemType == Item::IronIngot);
			}
			case Item::GoldenBoots:
			case Item::GoldenChestplate:
			case Item::GoldenHelmet:
			case Item::GoldenLeggings:
			{
				return (a_Item.m_ItemType == Item::GoldIngot);
			}
			case Item::Elytra:  // TODO: require Phantom Membrane instead of leather starting from protocol version 369 or 1.13 release
			case Item::LeatherBoots:
			case Item::LeatherHelmet:
			case Item::LeatherLeggings:
			case Item::LeatherChestplate:
			{
				return (a_Item.m_ItemType == Item::Leather);
			}
			default:
			{
				LOGWARNING("{}: Item type not handled {}.", __FUNCTION__, a_Item.m_ItemType);
				return false;
			}
		}
		return false;
	}

} ;




