
// HorseWindow.cpp

// Representing the UI window for a horse entity

#include "Globals.h"
#include "../Mobs/Horse.h"
#include "../UI/HorseWindow.h"
#include "../UI/SlotArea.h"





cHorseWindow::cHorseWindow(cHorse & a_Horse):
	Super(wtAnimalChest, "Horse"),
	m_Horse(a_Horse)
{
	m_SlotAreas.push_back(new cSlotAreaHorse(a_Horse, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cHorseWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Horse Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		// Inventory or Hotbar
		if (ItemCategory::IsHorseArmor(a_ItemStack.m_ItemType) || (a_ItemStack.m_ItemType == E_ITEM_SADDLE))
		{
			AreasInOrder.push_back(m_SlotAreas[0]);  /* Horse */
			Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
		}
	}
}





UInt32 cHorseWindow::GetHorseID() const
{
	return m_Horse.GetUniqueID();
}




