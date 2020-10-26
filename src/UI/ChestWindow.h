
// ChestWindow.h

// Representing the UI window for the chest block





#pragma once

#include "Window.h"





class cChestWindow:
	public cWindow
{
	using Super = cWindow;

public:

	cChestWindow(cChestEntity * a_Chest);

	cChestWindow(cChestEntity * a_PrimaryChest, cChestEntity * a_SecondaryChest);

	virtual ~cChestWindow() override;

	virtual bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse) override;

	virtual void OpenedByPlayer(cPlayer & a_Player) override;

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

protected:
	cWorld * m_World;
	Vector3i m_BlockPos;  // Position of the chest, for the window-close packet
	cChestEntity * m_PrimaryChest;
	cChestEntity * m_SecondaryChest;
};




