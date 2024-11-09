
// BrewingstandWindow.cpp

// Representing the UI window for the brewing stand block

#include "Globals.h"
#include "BrewingstandWindow.h"
#include "SlotArea.h"
#include "../BrewingRecipes.h"
#include "../Root.h"





cBrewingstandWindow::cBrewingstandWindow(cBrewingstandEntity * a_Brewingstand):
	Super(wtBrewery, "Brewingstand")
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
		if ((a_Slot >= 0) && (a_Slot <= 4))
		{
			// Brewing stand Area
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
		}
		else
		{
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar */
			Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
		}
	}
	else
	{
		cBrewingRecipes * BR = cRoot::Get()->GetBrewingRecipes();
		if ((BR->IsBottle(a_ItemStack)) || (BR->IsIngredient(a_ItemStack)) || BR->IsFuel(a_ItemStack))
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

		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}
