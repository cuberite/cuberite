
// EnderChestWindow.h

// Representing the UI window for the enderchest block





#pragma once

#include "Window.h"
#include "../BlockEntities/EnderChestEntity.h"





class cEnderChestWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cEnderChestWindow(cEnderChestEntity * a_EnderChest) :
		cWindow(wtChest, "Ender Chest"),
		m_World(a_EnderChest->GetWorld()),
		m_BlockX(a_EnderChest->GetPosX()),
		m_BlockY(a_EnderChest->GetPosY()),
		m_BlockZ(a_EnderChest->GetPosZ())
	{
		m_ShouldDistributeToHotbarFirst = false;
		m_SlotAreas.push_back(new cSlotAreaEnderChest(a_EnderChest, *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));

		// Play the opening sound:
		m_World->BroadcastSoundEffect("random.chestopen", (double)m_BlockX, (double)m_BlockY, (double)m_BlockZ, 1, 1);

		// Send out the chest-open packet:
		m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 1, E_BLOCK_ENDER_CHEST);
	}

	~cEnderChestWindow()
	{
		// Send out the chest-close packet:
		m_World->BroadcastBlockAction(m_BlockX, m_BlockY, m_BlockZ, 1, 0, E_BLOCK_ENDER_CHEST);

		// Play the closing sound
		m_World->BroadcastSoundEffect("random.chestclosed", (double)m_BlockX, (double)m_BlockY, (double)m_BlockZ, 1, 1);
	}

protected:
	cWorld * m_World;
	int m_BlockX, m_BlockY, m_BlockZ;  // Position of the enderchest, for the window-close packet
};




