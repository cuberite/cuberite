
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

	virtual ~cEnderChestWindow() override;

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

protected:
	cWorld * m_World;
	Vector3i m_Block;  // Position of the enderchest, for the window-close packet
};




