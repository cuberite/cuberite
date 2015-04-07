
// EnderChestWindow.h

// Representing the UI window for the enderchest block





#pragma once

#include "Window.h"
#include "../BlockEntities/EnderChestEntity.h"





class cEnderChestWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cEnderChestWindow(cEnderChestEntity * a_EnderChest);

	~cEnderChestWindow();

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

protected:
	cWorld * m_World;
	int m_BlockX, m_BlockY, m_BlockZ;  // Position of the enderchest, for the window-close packet
};




