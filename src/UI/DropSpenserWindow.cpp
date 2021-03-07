
// DropSpenserWindow.cpp

// Representing the UI window for the dropper / dispenser block

#include "Globals.h"
#include "DropSpenserWindow.h"
#include "SlotArea.h"





cDropSpenserWindow::cDropSpenserWindow(cDropSpenserEntity * a_DropSpenser):
	Super(wtDropSpenser, (a_DropSpenser->GetBlockType() == E_BLOCK_DISPENSER) ? "Dispenser" : "Dropper")
{
	m_SlotAreas.push_back(new cSlotAreaItemGrid(a_DropSpenser->GetContents(), *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cDropSpenserWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// DropSpenser Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		// Inventory or Hotbar
		AreasInOrder.push_back(m_SlotAreas[0]);  /* DropSpenser */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}




