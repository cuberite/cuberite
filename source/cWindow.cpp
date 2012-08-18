
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWindow.h"
#include "cItem.h"
#include "cClientHandle.h"
#include "cPlayer.h"
#include "cPickup.h"
#include "cInventory.h"
#include "cWindowOwner.h"
#include "items/Item.h"

#include "packets/cPacket_WholeInventory.h"
#include "packets/cPacket_WindowOpen.h"
#include "packets/cPacket_WindowClose.h"





cWindow::cWindow( cWindowOwner* a_Owner, bool a_bInventoryVisible, cWindow::WindowType a_WindowType, int a_WindowID)
	: m_WindowID( a_WindowID )
	, m_WindowType( a_WindowType )
	, m_Owner( a_Owner )
	, m_bInventoryVisible( a_bInventoryVisible )
	, m_NumSlots( 0 )
	, m_Slots( 0 )
	, m_DraggingItem( 0 )
	, m_IsDestroyed(false)
{
	LOGD("Created a window at %p, type = %d, ID = %i", this, a_WindowType, a_WindowID);
	if (!m_bInventoryVisible)
	{
		m_DraggingItem = new cItem();
	}
}





cWindow::~cWindow()
{
	LOGD("Deleting a window at %p", this);
	if( !m_bInventoryVisible && m_DraggingItem )
	{
		delete m_DraggingItem;
		m_DraggingItem = 0;
	}
	LOGD("Deleted a window at %p", this);
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





void cWindow::Clicked(
	cPlayer & a_Player, 
	int a_WindowID, short a_SlotNum, bool a_IsRightClick, bool a_IsShiftPressed, 
	const cItem & a_HeldItem
)
{
	if (a_WindowID != m_WindowID)
	{
		LOG("WRONG WINDOW ID! (exp %d, got %d) received from \"%s\"", m_WindowID, a_WindowID, a_Player.GetName().c_str());
		return;
	}

	if (m_bInventoryVisible)
	{
		cWindow * Window = a_Player.GetInventory().GetWindow();
		if (Window != NULL)
		{
			m_DraggingItem = Window->GetDraggingItem();
		}
	}
	bool bAsync = false;
	if (a_SlotNum == -999) // Outside window click
	{
		if (a_IsRightClick)
		{
			a_Player.TossItem(true);
		}
		else
		{
			a_Player.TossItem(true, m_DraggingItem->m_ItemCount);
		}
	}
	else if (GetSlot(a_SlotNum) != NULL)
	{
		cItem * Item = GetSlot(a_SlotNum);
		if (!Item->IsEqual(a_HeldItem))
		{
			LOGD("*** Window lost sync ***");
			LOGD("My Type:  %i Their Type:  %i", Item->m_ItemID,     a_HeldItem.m_ItemID);
			LOGD("My Count: %i Their Count: %i", Item->m_ItemCount,  a_HeldItem.m_ItemCount);
			LOGD("My Dmg:   %i Their Dmg:   %i", Item->m_ItemHealth, a_HeldItem.m_ItemHealth);
			bAsync = true;
		}
	}
	if (m_DraggingItem && (a_SlotNum > -1) && (a_SlotNum < m_NumSlots))
	{
		if (!a_IsRightClick)
		{
			// Left-clicked
			if (!m_DraggingItem->IsEqual(m_Slots[a_SlotNum]))
			{
				// Switch contents
				cItem tmp(*m_DraggingItem);
				*m_DraggingItem = m_Slots[a_SlotNum];
				m_Slots[a_SlotNum] = tmp;
			}
			else
			{
				// Same type, add items:
				cItemHandler * Handler = ItemHandler(m_DraggingItem->m_ItemID);
				int FreeSlots = Handler->GetMaxStackSize() - m_Slots[a_SlotNum].m_ItemCount;
				if (FreeSlots < 0)
				{
					ASSERT(!"Bad item stack size - where did we get more items in a slot than allowed?");
					FreeSlots = 0;
				}
				int Filling = (FreeSlots > m_DraggingItem->m_ItemCount) ? m_DraggingItem->m_ItemCount : FreeSlots;
				m_Slots[a_SlotNum].m_ItemCount += (char)Filling;
				m_DraggingItem->m_ItemCount -= (char)Filling;
				if (m_DraggingItem->m_ItemCount <= 0)
				{
					m_DraggingItem->Empty();
				}
			}
		}
		else
		{
			// Right clicked
			if (m_DraggingItem->m_ItemID <= 0) // Empty-handed?
			{
				m_DraggingItem->m_ItemCount = (char)(((float)m_Slots[a_SlotNum].m_ItemCount) / 2.f + 0.5f);
				m_Slots[a_SlotNum].m_ItemCount -= m_DraggingItem->m_ItemCount;
				m_DraggingItem->m_ItemID = m_Slots[a_SlotNum].m_ItemID;
				m_DraggingItem->m_ItemHealth = m_Slots[a_SlotNum].m_ItemHealth;

				if (m_Slots[a_SlotNum].m_ItemCount <= 0)
				{
					m_Slots[a_SlotNum].Empty();
				}
			}
			else if ((m_Slots[a_SlotNum].m_ItemID <= 0) || m_DraggingItem->IsEqual(m_Slots[a_SlotNum]))
			{
				// Drop one item in slot
				cItemHandler * Handler = ItemHandler(m_Slots[a_SlotNum].m_ItemID);
				if ((m_DraggingItem->m_ItemCount > 0) && (m_Slots[a_SlotNum].m_ItemCount < Handler->GetMaxStackSize()))
				{
					m_Slots[a_SlotNum].m_ItemID = m_DraggingItem->m_ItemID;
					m_Slots[a_SlotNum].m_ItemCount++;
					m_Slots[a_SlotNum].m_ItemHealth = m_DraggingItem->m_ItemHealth;
					m_DraggingItem->m_ItemCount--;
				}
				if (m_DraggingItem->m_ItemCount <= 0)
				{
					m_DraggingItem->Empty();
				}
			}
			else if (!m_DraggingItem->IsEqual(m_Slots[a_SlotNum]))
			{
				// Swap contents
				cItem tmp( *m_DraggingItem );
				*m_DraggingItem = m_Slots[a_SlotNum];
				m_Slots[a_SlotNum] = tmp;
			}
		}
		if (bAsync)
		{
			// TODO: Handle this thread-safely (m_OpenedBy may change by another cSocketThread
			for (cPlayerList::iterator itr = m_OpenedBy.begin(); itr != m_OpenedBy.end(); ++itr)
			{
				SendWholeWindow((*itr)->GetClientHandle());
			}
			if (m_bInventoryVisible || m_OpenedBy.empty())
			{
				a_Player.GetInventory().SendWholeInventory( a_Player.GetClientHandle() );
			}
		}
	}
	else if (m_bInventoryVisible) // Click in player inventory
	{
		cWindow * Window = a_Player.GetInventory().GetWindow();
		if (Window)
		{
			Window->Clicked(a_Player, a_WindowID, a_SlotNum - 9, a_IsRightClick, a_IsShiftPressed, a_HeldItem);
		}
	}
	if (m_DraggingItem != NULL)
	{
		LOGD("Dragging: %i", m_DraggingItem->m_ItemCount );
	}
}





void cWindow::Open( cPlayer & a_Player )
{
	{
		cCSLock Lock(m_CS);
		// If player is already in OpenedBy remove player first
		m_OpenedBy.remove( &a_Player );
		// Then add player
		m_OpenedBy.push_back( &a_Player );
	}

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
	if (m_DraggingItem && m_DraggingItem->m_ItemCount > 0)
	{
		LOGD("Player holds item! Dropping it...");
		a_Player.TossItem(true, m_DraggingItem->m_ItemCount);
	}

	cPacket_WindowClose WindowClose;
	WindowClose.m_Close = (char)m_WindowID;
	cClientHandle * ClientHandle = a_Player.GetClientHandle();
	if (ClientHandle != NULL)
	{
		ClientHandle->Send(WindowClose);
	}

	{
		cCSLock Lock(m_CS);
		m_OpenedBy.remove( &a_Player );
		if( m_OpenedBy.size() == 0 )
		{
			Destroy();
		}
	}
	if (m_IsDestroyed)
	{
		delete this;
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





bool cWindow::ForEachPlayer(cItemCallback<cPlayer> & a_Callback)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(), end = m_OpenedBy.end(); itr != end; ++itr)
	{
		if (a_Callback.Item(*itr))
		{
			return false;
		}
	}  // for itr - m_OpenedBy[]
	return true;
}





bool cWindow::ForEachClient(cItemCallback<cClientHandle> & a_Callback)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(), end = m_OpenedBy.end(); itr != end; ++itr)
	{
		if (a_Callback.Item((*itr)->GetClientHandle()))
		{
			return false;
		}
	}  // for itr - m_OpenedBy[]
	return true;
}





void cWindow::Destroy()
{
	LOGD("Destroying window %p (type %d)", this, m_WindowType);
	if (m_Owner != NULL)
	{
		m_Owner->CloseWindow();
		m_Owner = NULL;
	}
	m_IsDestroyed = true;
}





void cWindow::SendWholeWindow( cClientHandle* a_Client )
{
	cPacket_WholeInventory Inventory( this );
	a_Client->Send( Inventory );
}





void cWindow::BroadcastWholeWindow(void)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(); itr != m_OpenedBy.end(); ++itr)
	{
		SendWholeWindow((*itr)->GetClientHandle());
	}  // for itr - m_OpenedBy[]
}





void cWindow::Broadcast(const cPacket & a_Packet)
{
	cCSLock Lock(m_CS);
	for (cPlayerList::iterator itr = m_OpenedBy.begin(); itr != m_OpenedBy.end(); ++itr)
	{
		(*itr)->GetClientHandle()->Send(a_Packet);
	}  // for itr - m_OpenedBy[]
}




