
// ShulkerBoxWindow.h

// Representing the UI window for the Shulker Box block





#pragma once

#include "Window.h"





class cShulkerBoxWindow :
	public cWindow
{
	typedef cWindow super;

public:
	cShulkerBoxWindow(cShulkerBoxEntity * a_ShulkerBox);

	virtual ~cShulkerBoxWindow() override;

	virtual bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse) override;

	virtual void OpenedByPlayer(cPlayer & a_Player) override;

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

protected:
	cWorld * m_World;
	Vector3i m_BlockPos;  // Position of the ShulkerBox, for the window-close packet
	cShulkerBoxEntity * m_ShulkerBox;
};




