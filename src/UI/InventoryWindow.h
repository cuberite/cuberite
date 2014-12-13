
// InventoryWindow.h

// Representing the UI window for the player inventory





#pragma once

#include "Window.h"





class cInventoryWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cInventoryWindow(cPlayer & a_Player) :
		cWindow(wtInventory, "Inventory"),
		m_Player(a_Player)
	{
		m_SlotAreas.push_back(new cSlotAreaCrafting(2, *this));  // The creative inventory doesn't display it, but it's still counted into slot numbers
		m_SlotAreas.push_back(new cSlotAreaArmor(*this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	}
	
protected:
	cPlayer & m_Player;
};




