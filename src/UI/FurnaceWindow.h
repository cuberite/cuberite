
// FurnaceWindow.h

// Representing the UI window for the furnace block





#pragma once

#include "Window.h"





class cFurnaceWindow :
	public cWindow
{
	using Super = cWindow;

public:
	cFurnaceWindow(cFurnaceEntity * a_Furnace);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

};




