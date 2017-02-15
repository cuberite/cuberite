
// EnderChestWindow.cpp

// Representing the UI window for the enderchest block

#include "Globals.h"
#include "../World.h"
#include "EnderChestWindow.h"
#include "SlotArea.h"





cEnderChestWindow::cEnderChestWindow(cEnderChestEntity * a_EnderChest) :
	cWindow(wtChest, "Ender Chest"),
	m_World(a_EnderChest->GetWorld()),
	m_BlockX(a_EnderChest->GetPosX()),
	m_BlockY(a_EnderChest->GetPosY()),
	m_BlockZ(a_EnderChest->GetPosZ())
{
	m_SlotAreas.push_back(new cSlotAreaEnderChest(a_EnderChest, *this));
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));

	// Play the opening sound:
	m_World->BroadcastSoundEffect(
		"block.enderchest.open",
		static_cast<double>(m_BlockX),
		static_cast<double>(m_BlockY),
		static_cast<double>(m_BlockZ),
		1,
		1
	);

	// Send out the chest-open packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, E_BLOCK_ENDER_CHEST);
}





cEnderChestWindow::~cEnderChestWindow()
{
	// Send out the chest-close packet:
	m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 0, E_BLOCK_ENDER_CHEST);

	// Play the closing sound
	m_World->BroadcastSoundEffect(
		"block.enderchest.close",
		static_cast<double>(m_BlockX),
		static_cast<double>(m_BlockY),
		static_cast<double>(m_BlockZ),
		1, 1
	);
}





void cEnderChestWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		// Chest Area
		AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, true);
	}
	else
	{
		// Hotbar or Inventory
		AreasInOrder.push_back(m_SlotAreas[0]);  /* Chest */
		super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
	}
}




