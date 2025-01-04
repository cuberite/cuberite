// ShulkerBoxWindow.h

// Representing the UI window for the shulker box block

#pragma once

#include "Window.h"

#include "../BlockEntities/ShulkerBoxEntity.h"





class cShulkerBoxWindow :
	public cWindow
{
	using Super = cWindow;

public:

	cShulkerBoxWindow(cShulkerBoxEntity * a_ShulkerBox, const AString & CustomName);

	virtual ~cShulkerBoxWindow() override;

	virtual bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse) override;

	virtual void OpenedByPlayer(cPlayer & a_Player) override;

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

	virtual void Clicked(cPlayer & a_Player, int a_WindowID, short a_SlotNum, eClickAction a_ClickAction, const cItem & a_ClickedItem) override;

protected:
	cWorld * m_World;
	Vector3i m_BlockPos;  // Position of the shulker, for the window-close packet
	cShulkerBoxEntity * m_ShulkerBox;
};
