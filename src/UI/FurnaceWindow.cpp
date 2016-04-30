
// FurnaceWindow.cpp

// Representing the UI window for the furnace block

#include "Globals.h"
#include "FurnaceWindow.h"
#include "SlotArea.h"
#include "../FurnaceRecipe.h"
#include "../Root.h"





cFurnaceWindow::cFurnaceWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceEntity * a_Furnace) :
	cWindow(wtFurnace, "Furnace")
{
	m_SlotAreas.push_back(new cSlotAreaFurnace(a_Furnace, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cFurnaceWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Furnace Area
		if (a_Slot == 2)
		{
			// Result Slot
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
		}
		else
		{
			// Furnace Input / Fuel Slot
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
		}
	}
	else
	{
		auto & FurnaceRecipes = cRoot::Get()->GetFurnaceRecipe();
		if ((FurnaceRecipes.GetRecipeFrom(a_ItemStack) != nullptr) || (FurnaceRecipes.IsFuel(a_ItemStack)))
		{
			// The item is a valid input item or fuel
			AreasInOrder.push_back(m_SlotAreas[0]);  /* Furnace Area */
		}
		else if (a_ClickedArea == m_SlotAreas[1])
		{
			// Inventory Area
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
		}
		else
		{
			// Hotbar Area
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		}
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}




