
// BrewingstandWindow.h

// Representing the UI window for the brewing stand block





#pragma once

#include "Window.h"





class cBrewingstandWindow :
	public cWindow
{
	using Super = cWindow;

public:

	cBrewingstandWindow(cBrewingstandEntity * a_Brewingstand);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;
};




