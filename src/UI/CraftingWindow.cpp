
// CraftingWindow.cpp

// Representing the UI window for the crafting block

#include "Globals.h"
#include "CraftingWindow.h"
#include "SlotArea.h"




cCraftingWindow::cCraftingWindow() :
	cWindow(wtWorkbench, "Crafting Table")
{
	m_SlotAreas.push_back(new cSlotAreaCrafting(3, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cCraftingWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Crafting Area
		if (a_Slot == 0)
		{
			// Result Slot
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		}
		else
		{
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
		}
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, (a_Slot == 0));
	}
	else if (a_ClickedArea == m_SlotAreas[1])
	{
		// Inventory Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
	else
	{
		// Hotbar
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}





void cCraftingWindow::LoadRecipe(cPlayer & a_Player, UInt32 a_RecipeId)
{
	auto slotAreaCrafting = static_cast<cSlotAreaCrafting *>(m_SlotAreas[0]);
	slotAreaCrafting->LoadRecipe(a_Player, a_RecipeId);
}
