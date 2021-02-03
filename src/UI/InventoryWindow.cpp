
// InventoryWindow.cpp

// Representing the UI window for the player inventory

#include "Globals.h"
#include "InventoryWindow.h"
#include "SlotArea.h"
#include "../Entities/Player.h"





cInventoryWindow::cInventoryWindow(cPlayer & a_Player) :
	cWindow(wtInventory, "Inventory"),
	m_Player(a_Player)
{
	m_SlotAreas.push_back(new cSlotAreaCrafting(2, *this));  // The creative inventory doesn't display it, but it's still counted into slot numbers
	m_SlotAreas.push_back(new cSlotAreaArmor(*this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	m_SlotAreas.push_back(new cSlotAreaShield(*this));
}





void cInventoryWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if ((a_ItemStack.m_ItemType == E_ITEM_ELYTRA) && ((a_ClickedArea == m_SlotAreas[2]) || (a_ClickedArea == m_SlotAreas[3])) && m_SlotAreas[1]->GetSlot(1, a_Player)->IsEmpty())
	{
		a_Player.GetWorld()->BroadcastSoundEffect("item.armor.equip_elytra", a_Player.GetPosition(), 1.0, 1.0);
	}

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Crafting Area
		if (a_Slot == 0)
		{
			// Result Slot
			AreasInOrder.push_back(m_SlotAreas[3]);  /* Hotbar    */
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Inventory */
		}
		else
		{
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Inventory */
			AreasInOrder.push_back(m_SlotAreas[3]);  /* Hotbar    */
		}
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, (a_Slot == 0));
	}
	else if (a_ClickedArea == m_SlotAreas[1])
	{
		if ((a_ItemStack.m_ItemType == E_ITEM_ELYTRA) && a_Player.IsElytraFlying())
		{
			a_Player.SetElytraFlight(false);
		}

		// Armor Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Inventory */
		AreasInOrder.push_back(m_SlotAreas[3]);  /* Hotbar    */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
	else if (a_ClickedArea == m_SlotAreas[2])
	{
		// Inventory Area
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Armor  */
		AreasInOrder.push_back(m_SlotAreas[3]);  /* Hotbar */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
	else
	{
		// Hotbar
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Armor     */
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Inventory */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}

}





void cInventoryWindow::Clicked(cPlayer & a_Player, int a_WindowID, short a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	if ((a_SlotNum == 6) && (a_Player.GetDraggingItem().m_ItemType == E_ITEM_ELYTRA) && ((a_ClickAction == caLeftClick) || (a_ClickAction == caRightClick)))
	{
		a_Player.GetWorld()->BroadcastSoundEffect("item.armor.equip_elytra", a_Player.GetPosition(), 1.0, 1.0);
	}

	Super::Clicked(a_Player, a_WindowID, a_SlotNum, a_ClickAction, a_ClickedItem);

	if ((a_SlotNum == 6) && (a_Player.GetDraggingItem().m_ItemType == E_ITEM_ELYTRA) && ((a_ClickAction == caLeftClick) || (a_ClickAction == caRightClick)))
	{
		a_Player.SetElytraFlight(false);
	}

}





void cInventoryWindow::LoadRecipe(cPlayer & a_Player, UInt32 a_RecipeId)
{
	auto slotAreaCrafting = static_cast<cSlotAreaCrafting *>(m_SlotAreas[0]);
	slotAreaCrafting->LoadRecipe(a_Player, a_RecipeId);
}
