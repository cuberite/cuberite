
// HopperWindow.cpp

// Representing the UI window for the hopper block

#include "Globals.h"
#include "../BlockEntities/HopperEntity.h"
#include "HopperWindow.h"
#include "../BlockEntities/DropperEntity.h"





cHopperWindow::cHopperWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cHopperEntity * a_Hopper) :
	super(wtHopper, "Hopper")
{
	m_SlotAreas.push_back(new cSlotAreaItemGrid(a_Hopper->GetContents(), *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cHopperWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Hopper Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		// Inventory or Hotbar
		AreasInOrder.push_back(m_SlotAreas[0]);  /* Hopper */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}




