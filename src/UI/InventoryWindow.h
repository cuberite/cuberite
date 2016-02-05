
// InventoryWindow.h

// Representing the UI window for the player inventory





#pragma once

#include "Window.h"





class cInventoryWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cInventoryWindow(cPlayer & a_Player);

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

protected:
	cPlayer & m_Player;
};




