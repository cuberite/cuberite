
// BrewingstandWindow.cpp

// Representing the UI window for the brewing stand block

#include "Globals.h"
#include "BrewingstandWindow.h"
#include "SlotArea.h"
#include "../BrewingRecipes.h"
#include "../Root.h"





cBrewingstandWindow::cBrewingstandWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cBrewingstandEntity * a_Brewingstand) :
	cWindow(wtBrewery, "Brewingstand")
{
	m_SlotAreas.push_back(new cSlotAreaBrewingstand(a_Brewingstand, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cBrewingstandWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Brewing stand Area
		if ((a_Slot >= 0) && (a_Slot < 3))
		{
			// Bottle slots
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
		}
		else
		{
			// Ingredient slot
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
			super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
		}
	}
	else
	{
		auto & BR = cRoot::Get()->GetBrewingRecipes();
		if ((BR.IsBottle(a_ItemStack)) || (BR.IsIngredient(a_ItemStack)))
		{
			AreasInOrder.push_back(m_SlotAreas[0]);  /* brewing stand Area */
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
