
// CraftingWindow.h

// Representing the UI window for the crafting block





#pragma once

#include "Window.h"





class cCraftingWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cCraftingWindow(int a_BlockX, int a_BlockY, int a_BlockZ);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;
};




