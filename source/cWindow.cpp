
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWindow.h"
#include "cItem.h"
#include "cClientHandle.h"
#include "cPlayer.h"
#include "cPickup.h"
#include "cInventory.h"
#include "cWindowOwner.h"

#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_WholeInventory.h"
#include "packets/cPacket_WindowOpen.h"
#include "packets/cPacket_WindowClose.h"





cWindow::cWindow( cWindowOwner* a_Owner, bool a_bInventoryVisible )
	: m_WindowID( 0 )
	, m_WindowType( 0 )
	, m_Owner( a_Owner )
	, m_bInventoryVisible( a_bInventoryVisible )
	, m_NumSlots( 0 )
	, m_Slots( 0 )
	, m_DraggingItem( 0 )
{
	if( !m_bInventoryVisible ) m_DraggingItem = new cItem();
}





cWindow::~cWindow()
{
	if( !m_bInventoryVisible && m_DraggingItem )
	{
		delete m_DraggingItem;
		m_DraggingItem = 0;
	}
}





cItem* cWindow::GetSlot( int a_Slot )
{
	if(a_Slot > -1 && a_Slot < m_NumSlots)
	{
		return (m_Slots + a_Slot);
	}
	return 0;
}





cItem* cWindow::GetDraggingItem( cPlayer * a_Player /* = 0 */ )
{
	if( m_bInventoryVisible && a_Player )
	{
		cWindow* Window = a_Player->GetInventory().GetWindow();
		if( Window )
		{
			return Window->GetDraggingItem();
		}
	}
	return m_DraggingItem;
}





void cWindow::Clicked( cPacket_WindowClick* a_ClickPacket, cPlayer & a_Player )
{
	if( a_ClickPacket->m_WindowID != m_WindowID )
	{
		LOG("WRONG WINDOW ID!");
		return;
	}

	if( m_bInventoryVisible )
	{
		cWindow* Window = a_Player.GetInventory().GetWindow();
		if( Window )
		{
			m_DraggingItem = Window->GetDraggingItem();
		}
	}
	bool bAsync = false;
	if( a_ClickPacket->m_SlotNum == -999 ) // Outside window click
	{
		if( a_ClickPacket->m_RightMouse )
			a_Player.TossItem( true );
		else
			a_Player.TossItem( true, m_DraggingItem->m_ItemCount );
	}
	else if( GetSlot( a_ClickPacket->m_SlotNum ) != 0 )
	{
		cItem* Item = GetSlot( a_ClickPacket->m_SlotNum );
		if( a_ClickPacket->m_ItemID != Item->m_ItemID
			|| a_ClickPacket->m_ItemCount != Item->m_ItemCount
			|| a_ClickPacket->m_ItemUses != Item->m_ItemHealth )
		{
			if( !((a_ClickPacket->m_ItemID == -1 || a_ClickPacket->m_ItemID == 0) && (Item->m_ItemID == -1 || Item->m_ItemID == 0 )) )
			{
				LOG("My ID: %i Their ID: %i", Item->m_ItemID, a_ClickPacket->m_ItemID );
				LOG("My Count: %i Their Count: %i", Item->m_ItemCount, a_ClickPacket->m_ItemCount );
				LOG("My Uses: %i Their Uses: %i", Item->m_ItemHealth, a_ClickPacket->m_ItemUses );
				bAsync = true;
			}
		}
	}
	if( m_DraggingItem && a_ClickPacket->m_SlotNum > -1 && a_ClickPacket->m_SlotNum < m_NumSlots )
	{
		if( a_ClickPacket->m_RightMouse == 0 )
		{
			if( !m_DraggingItem->Equals( m_Slots[a_ClickPacket->m_SlotNum] ) )
			{
				cItem tmp( *m_DraggingItem );
				*m_DraggingItem = m_Slots[a_ClickPacket->m_SlotNum];
				m_Slots[a_ClickPacket->m_SlotNum] = tmp; // Switch contents
			}
			else
			{
				int FreeSlots = 64 - m_Slots[a_ClickPacket->m_SlotNum].m_ItemCount;
				int Filling = (FreeSlots > m_DraggingItem->m_ItemCount) ? m_DraggingItem->m_ItemCount : FreeSlots;
				m_Slots[a_ClickPacket->m_SlotNum].m_ItemCount += (char)Filling;
				m_DraggingItem->m_ItemCount -= (char)Filling;
				if( m_DraggingItem->m_ItemCount <= 0 )
					m_DraggingItem->Empty();
			}
		}
		else // Right clicked
		{
			if( m_DraggingItem->m_ItemID <= 0 ) // Empty?
			{
				m_DraggingItem->m_ItemCount = (char)(((float)m_Slots[a_ClickPacket->m_SlotNum].m_ItemCount)/2.f + 0.5f);
				m_Slots[a_ClickPacket->m_SlotNum].m_ItemCount -= m_DraggingItem->m_ItemCount;
				m_DraggingItem->m_ItemID = m_Slots[a_ClickPacket->m_SlotNum].m_ItemID;
				m_DraggingItem->m_ItemHealth = m_Slots[a_ClickPacket->m_SlotNum].m_ItemHealth;

				if( m_Slots[a_ClickPacket->m_SlotNum].m_ItemCount <= 0 )
				{
					m_Slots[a_ClickPacket->m_SlotNum].Empty();
				}
			}
			else if( m_Slots[a_ClickPacket->m_SlotNum].m_ItemID <= 0 || m_DraggingItem->Equals( m_Slots[a_ClickPacket->m_SlotNum] ) )
			{	// Drop one item in slot
				if( m_DraggingItem->m_ItemCount > 0 && m_Slots[a_ClickPacket->m_SlotNum].m_ItemCount < 64 )
				{
					m_Slots[a_ClickPacket->m_SlotNum].m_ItemID = m_DraggingItem->m_ItemID;
					m_Slots[a_ClickPacket->m_SlotNum].m_ItemCount++;
					m_Slots[a_ClickPacket->m_SlotNum].m_ItemHealth = m_DraggingItem->m_ItemHealth;
					m_DraggingItem->m_ItemCount--;
				}
				if( m_DraggingItem->m_ItemCount <= 0 )
				{
					m_DraggingItem->Empty();
				}
			}
			else if( !m_DraggingItem->Equals( m_Slots[a_ClickPacket->m_SlotNum]) ) // Swap contents
			{
				cItem tmp( *m_DraggingItem );
				*m_DraggingItem = m_Slots[a_ClickPacket->m_SlotNum];
				m_Slots[a_ClickPacket->m_SlotNum] = tmp; // Switch contents
			}
		}
		if( bAsync )
		{
			LOG("Window is not synchonous with client. Sending whole window. ID: %i", m_WindowID);
			for( std::list< cPlayer* >::iterator itr = m_OpenedBy.begin(); itr != m_OpenedBy.end(); ++itr )
			{
				SendWholeWindow( (*itr)->GetClientHandle() );
			}
			if( m_bInventoryVisible || m_OpenedBy.size() == 0 )
			{
				a_Player.GetInventory().SendWholeInventory( a_Player.GetClientHandle() );
			}
		}
	}
	else if( m_bInventoryVisible ) // Click in player inventory
	{
		a_ClickPacket->m_WindowID = 0;
		a_ClickPacket->m_SlotNum -= (short)(m_NumSlots - 9);
		cWindow* Window = a_Player.GetInventory().GetWindow();
		if( Window )
		{
			Window->Clicked( a_ClickPacket, a_Player );
		}
	}
	if( m_DraggingItem ) LOG("Dragging: %i", m_DraggingItem->m_ItemCount );
}





void cWindow::Open( cPlayer & a_Player )
{
	// If player is already in OpenedBy remove player first
	m_OpenedBy.remove( &a_Player );
	// Then add player
	m_OpenedBy.push_back( &a_Player );

	cPacket_WindowOpen WindowOpen;
	WindowOpen.m_WindowID = (char)m_WindowID;
	WindowOpen.m_InventoryType = (char)m_WindowType;
	WindowOpen.m_WindowTitle = m_WindowTitle;
	WindowOpen.m_NumSlots = (char)m_NumSlots;
	a_Player.GetClientHandle()->Send( WindowOpen );
}





void cWindow::Close( cPlayer & a_Player )
{
	//Checks wheather the player is still holding an item
	if(m_DraggingItem && m_DraggingItem->m_ItemCount > 0)
	{
		LOG("Player holds item! Dropping it...");
		a_Player.TossItem( true, m_DraggingItem->m_ItemCount );
	}

	cPacket_WindowClose WindowClose;
	WindowClose.m_Close = (char)m_WindowID;
	cClientHandle* ClientHandle = a_Player.GetClientHandle();
	if( ClientHandle ) ClientHandle->Send( WindowClose );

	m_OpenedBy.remove( &a_Player );
	if( m_OpenedBy.size() == 0 )
	{
		Destroy();
	}
}





void cWindow::OwnerDestroyed()
{
	m_Owner = 0;
	while( m_OpenedBy.size() > 1 )
	{
		(*m_OpenedBy.begin() )->CloseWindow((char)GetWindowType());
	}
	(*m_OpenedBy.begin() )->CloseWindow((char)GetWindowType());
}





void cWindow::Destroy()
{
	LOG("DESTROY WINDOW");
	if( m_Owner )
	{
		m_Owner->CloseWindow();
		m_Owner = 0;
	}
	delete this;
}





void cWindow::SendWholeWindow( cClientHandle* a_Client )
{
	cPacket_WholeInventory Inventory( this );
	a_Client->Send( Inventory );
}




