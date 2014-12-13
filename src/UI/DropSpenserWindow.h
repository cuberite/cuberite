
// DropSpenserWindow.h

// Representing the UI window for the dropper/dispenser block





#pragma once

#include "Window.h"
#include "../BlockEntities/DropSpenserEntity.h"





class cDropSpenserWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cDropSpenserWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserEntity * a_DropSpenser) :
		cWindow(wtDropSpenser, (a_DropSpenser->GetBlockType() == E_BLOCK_DISPENSER) ? "Dispenser" : "Dropper")
	{
		m_ShouldDistributeToHotbarFirst = false;
		m_SlotAreas.push_back(new cSlotAreaItemGrid(a_DropSpenser->GetContents(), *this));
		m_SlotAreas.push_back(new cSlotAreaInventory(*this));
		m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
	}
};




