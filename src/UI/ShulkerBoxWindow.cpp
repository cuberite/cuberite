
// ShulkerBoxWindow.cpp

// Representing the UI window for the Shulker Box block

#include "Globals.h"
#include "ShulkerBoxWindow.h"
#include "../BlockEntities/ShulkerBoxEntity.h"
#include "../Entities/Player.h"
#include "SlotArea.h"





cShulkerBoxWindow::cShulkerBoxWindow(cShulkerBoxEntity * a_ShulkerBox):
	cWindow(wtShulkerBox, "Shulker Box"),
	m_World(a_ShulkerBox->GetWorld()),
	m_BlockPos(a_ShulkerBox->GetPos()),
	m_ShulkerBox(a_ShulkerBox)
{
	m_SlotAreas.push_back(new cSlotAreaShulkerBox(a_ShulkerBox, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));

	// Play the opening sound:
	m_World->BroadcastSoundEffect("block.shulker_box.open", m_BlockPos, 1, 1);

	// Send out the Shulker Box-open packet:
	m_World->BroadcastBlockAction(m_BlockPos, 1, 1, a_ShulkerBox->GetBlockType());
}





cShulkerBoxWindow::~cShulkerBoxWindow()
{
	// Send out the Shulker Box-close packet:
	m_World->BroadcastBlockAction(m_BlockPos, 1, 0, m_ShulkerBox->GetBlockType());

	m_World->BroadcastSoundEffect("block.shulker_box.close", m_BlockPos, 1, 1);
}





bool cShulkerBoxWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	m_ShulkerBox->SetNumberOfPlayers(m_ShulkerBox->GetNumberOfPlayers() - 1);
	m_ShulkerBox->GetWorld()->WakeUpSimulators(m_ShulkerBox->GetPos());

	cWindow::ClosedByPlayer(a_Player, a_CanRefuse);
	return true;
}





void cShulkerBoxWindow::OpenedByPlayer(cPlayer & a_Player)
{
	m_ShulkerBox->SetNumberOfPlayers(m_ShulkerBox->GetNumberOfPlayers() + 1);
	m_ShulkerBox->GetWorld()->WakeUpSimulators(m_ShulkerBox->GetPos());

	cWindow::OpenedByPlayer(a_Player);
}





void cShulkerBoxWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// ShulkerBox Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		// Hotbar or Inventory
		AreasInOrder.push_back(m_SlotAreas[0]);  /* ShulkerBox */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}




