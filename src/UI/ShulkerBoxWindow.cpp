
// ShulkerBoxWindow.cpp

// Representing the UI window for the shulker box block

#include "Globals.h"
#include "ShulkerBoxWindow.h"
#include "../BlockEntities/ShulkerBoxEntity.h"
#include "../Entities/Player.h"
#include "SlotArea.h"
#include "../ItemGrid.h"

class cItemGrid;





cShulkerBoxWindow::cShulkerBoxWindow(cShulkerBoxEntity * a_ShulkerBox, const AString & CustomName):
	cWindow(wtShulkerBox, CustomName.length() > 0 ? CustomName : "Shulker Box"),
	m_World(a_ShulkerBox->GetWorld()),
	m_BlockPos(a_ShulkerBox->GetPos()),
	m_ShulkerBox(a_ShulkerBox)
{
	m_SlotAreas.push_back(new cSlotAreaItemGrid(a_ShulkerBox->GetContents(), *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));

	// Play the opening sound:
	m_World->BroadcastSoundEffect("block.shulker_box.open", m_BlockPos, 1, 1);

	// Send out the shulker box open packet:
	m_World->BroadcastBlockAction(m_BlockPos, 1, 1, a_ShulkerBox->GetBlockType());
}





cShulkerBoxWindow::~cShulkerBoxWindow()
{
	// Send out the shulker box close packet:
	m_World->BroadcastBlockAction(m_BlockPos, 1, 0, m_ShulkerBox->GetBlockType());

	m_World->BroadcastSoundEffect("block.shulker_box.close", m_BlockPos, 1, 1);
}





bool cShulkerBoxWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	cWindow::ClosedByPlayer(a_Player, a_CanRefuse);
	return true;
}





void cShulkerBoxWindow::OpenedByPlayer(cPlayer & a_Player)
{
	m_ShulkerBox->SetNumberOfPlayers(m_ShulkerBox->GetNumberOfPlayers() + 1);

	cWindow::OpenedByPlayer(a_Player);
}





void cShulkerBoxWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Shulker box Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		if (cShulkerBoxEntity::IsShulkerBox(a_ItemStack.m_ItemType))
		{
			return;
		}

		// Hotbar or Inventory
		AreasInOrder.push_back(m_SlotAreas[0]);  /* Shulker box */
		Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}





void cShulkerBoxWindow::Clicked(cPlayer & a_Player, int a_WindowID, short a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem)
{
	if ((a_WindowID == GetWindowID()) && (a_SlotNum >= 0) && (a_SlotNum < m_SlotAreas[0]->GetNumSlots()))
	{
		cItem & DraggingItem = a_Player.GetDraggingItem();

		if (cShulkerBoxEntity::IsShulkerBox(DraggingItem.m_ItemType))
		{
			return;
		}
	}

	Super::Clicked(a_Player, a_WindowID, a_SlotNum, a_ClickAction, a_ClickedItem);
}
