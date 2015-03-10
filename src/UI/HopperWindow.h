
// HopperWindow.h

// Representing the UI window for the hopper block





#pragma once

#include "Window.h"





class cHopperWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cHopperWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cHopperEntity * a_Hopper);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

};




