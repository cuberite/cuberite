
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cFurnaceWindow.h"
#include "cItem.h"
#include "cFurnaceEntity.h"
#include "cPlayer.h"





cFurnaceWindow::cFurnaceWindow( cFurnaceEntity* a_Owner )
	: cWindow(a_Owner, true, cWindow::Furnace, 1)
	, m_Furnace( a_Owner )
{
}





void cFurnaceWindow::Clicked(
	cPlayer & a_Player, 
	int a_WindowID, short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, 
	const cItem & a_HeldItem
)
{
	cItem Fuel = *GetSlot( 0 );

	cWindow::Clicked(a_Player, a_WindowID, a_SlotNum, a_IsRightClick, a_IsShiftPressed, a_HeldItem);
	if (m_Furnace != NULL)
	{
		if ((a_SlotNum >= 0) && (a_SlotNum <= 2)) // them important slots
		{
			if (Fuel.m_ItemID != GetSlot( 0 )->m_ItemID)
			{
				m_Furnace->ResetCookTimer();
			}

			if (m_Furnace->StartCooking())
			{
				SendWholeWindow(a_Player.GetClientHandle());
			}
		}
	}
}





void cFurnaceWindow::Close( cPlayer & a_Player )
{
	m_Furnace = NULL;
	cWindow::Close( a_Player );
}




