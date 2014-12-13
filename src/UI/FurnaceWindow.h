
// FurnaceWindow.h

// Representing the UI window for the furnace block





#pragma once

#include "Window.h"





class cFurnaceWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cFurnaceWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceEntity * a_Furnace) :
		cWindow(wtFurnace, "Furnace")
	{
		m_ShouldDistributeToHotbarFirst = false;
		m_SlotAreas.push_back(new cSlotAreaFurnace(a_Furnace, *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	}
};




