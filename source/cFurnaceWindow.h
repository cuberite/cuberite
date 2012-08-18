
#pragma once

#include "cWindow.h"





class cFurnaceEntity;
class cWindowOwner;





class cFurnaceWindow :
	public cWindow
{
public:
	cFurnaceWindow( cFurnaceEntity* a_Owner );

	virtual void Clicked(
		cPlayer & a_Player, 
		int a_WindowID, short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, 
		const cItem & a_HeldItem
	) override;
	
	virtual void Close( cPlayer & a_Player ) override;
	
private:
	cFurnaceEntity * m_Furnace;
};




