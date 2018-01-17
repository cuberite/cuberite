
// FurnaceWindow.h

// Representing the UI window for the furnace block





#pragma once

#include "Window.h"





class cFurnaceWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cFurnaceWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cFurnaceEntity * a_Furnace);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

};




