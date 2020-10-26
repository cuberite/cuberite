
// BeaconWindow.cpp

// Representing the UI window for the beacon block

#include "Globals.h"
#include "BeaconWindow.h"
#include "SlotArea.h"
#include "../BlockEntities/BeaconEntity.h"
#include "../ClientHandle.h"





cBeaconWindow::cBeaconWindow(cBeaconEntity * a_Beacon):
	cWindow(wtBeacon, "Beacon"),
	m_Beacon(a_Beacon)
{
	m_SlotAreas.push_back(new cSlotAreaBeacon(m_Beacon, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cBeaconWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Beacon Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		if (cSlotAreaBeacon::IsPlaceableItem(a_ItemStack.m_ItemType) && (a_ItemStack.m_ItemCount == 1))
		{
			AreasInOrder.push_back(m_SlotAreas[0]);  /* Beacon */
		}

		if (a_ClickedArea == m_SlotAreas[1])
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





void cBeaconWindow::OpenedByPlayer(cPlayer & a_Player)
{
	Super::OpenedByPlayer(a_Player);

	a_Player.GetClientHandle()->SendWindowProperty(*this, 0, m_Beacon->GetBeaconLevel());
	a_Player.GetClientHandle()->SendWindowProperty(*this, 1, m_Beacon->GetPrimaryEffect());
	a_Player.GetClientHandle()->SendWindowProperty(*this, 2, m_Beacon->GetSecondaryEffect());
}




