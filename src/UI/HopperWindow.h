
// HopperWindow.h

// Representing the UI window for the hopper block





#pragma once

#include "Window.h"
#include "../BlockEntities/DropperEntity.h"





class cHopperWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cHopperWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cHopperEntity * a_Hopper) :
		super(wtHopper, "Hopper")
	{
		m_ShouldDistributeToHotbarFirst = false;
		m_SlotAreas.push_back(new cSlotAreaItemGrid(a_Hopper->GetContents(), *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	}
};




