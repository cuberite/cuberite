
// DropSpenserWindow.h

// Representing the UI window for the dropper / dispenser block





#pragma once

#include "Window.h"
#include "../BlockEntities/DropSpenserEntity.h"





class cDropSpenserWindow :
	public cWindow
{
	using Super = cWindow;

public:
	cDropSpenserWindow(cDropSpenserEntity * a_DropSpenser);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;
};




