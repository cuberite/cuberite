
// DropSpenserWindow.h

// Representing the UI window for the dropper / dispenser block





#pragma once

#include "Window.h"
#include "../BlockEntities/DropSpenserEntity.h"





class cDropSpenserWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cDropSpenserWindow(int a_BlockX, int a_BlockY, int a_BlockZ, cDropSpenserEntity * a_DropSpenser);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;
};




