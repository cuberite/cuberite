
// CraftingWindow.h

// Representing the UI window for the crafting block





#pragma once

#include "Window.h"





class cCraftingWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cCraftingWindow(int a_BlockX, int a_BlockY, int a_BlockZ) :
		cWindow(wtWorkbench, "Crafting Table")
	{
		m_SlotAreas.push_back(new cSlotAreaCrafting(3, *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	}
};




