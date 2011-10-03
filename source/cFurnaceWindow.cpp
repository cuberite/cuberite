#include "cFurnaceWindow.h"
#include "cItem.h"
#include "cFurnaceEntity.h"
#include "cPlayer.h"

#include "packets/cPacket_WindowClick.h"

#include "cMCLogger.h"

cFurnaceWindow::cFurnaceWindow( cFurnaceEntity* a_Owner )
	: cWindow( a_Owner, true )
	, m_Furnace( a_Owner )
{
	SetWindowID( 1 );
	SetWindowType( 2 ); // Furnace
}

void cFurnaceWindow::Clicked( cPacket_WindowClick* a_ClickPacket, cPlayer & a_Player )
{
	cItem Fuel = *GetSlot( 0 );

	cWindow::Clicked( a_ClickPacket, a_Player );
	if( m_Furnace )
	{
		if( a_ClickPacket->m_SlotNum >= 0 && a_ClickPacket->m_SlotNum <= 2 ) // them important slots
		{
			if( Fuel.m_ItemID != GetSlot( 0 )->m_ItemID )
				m_Furnace->ResetCookTimer();

			if( m_Furnace->StartCooking() )
			{
				SendWholeWindow( a_Player.GetClientHandle() );
			}
		}
	}
}

void cFurnaceWindow::Close( cPlayer & a_Player )
{
	m_Furnace = 0;
	cWindow::Close( a_Player );
}