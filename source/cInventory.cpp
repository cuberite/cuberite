
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cInventory.h"
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cWindow.h"
#include "cItem.h"
#include "cRoot.h"

#include <json/json.h>

#include "packets/cPacket_WindowClick.h"
#include "packets/cPacket_WholeInventory.h"
#include "packets/cPacket_InventorySlot.h"





cInventory::~cInventory()
{
	delete [] m_Slots;
	delete m_EquippedItem;
	if( GetWindow() ) GetWindow()->Close( *m_Owner );
	CloseWindow();
}





cInventory::cInventory(cPlayer* a_Owner)
{
	m_Owner = a_Owner;

	m_Slots = new cItem[c_NumSlots];
	for(unsigned int i = 0; i < c_NumSlots; i++)
		m_Slots[i].Empty();

	m_CraftSlots = m_Slots + c_CraftOffset;
	m_ArmorSlots = m_Slots + c_ArmorOffset;
	m_MainSlots  = m_Slots + c_MainOffset;
	m_HotSlots   = m_Slots + c_HotOffset;

	m_EquippedItem = new cItem();
	m_EquippedSlot = 0;

	if( !GetWindow() )
	{
		cWindow* Window = new cWindow( this, false );
		Window->SetSlots( m_Slots, c_NumSlots );
		Window->SetWindowID( 0 );
		OpenWindow( Window );
	}
}





bool cInventory::AddItem( cItem & a_Item )
{
	cItem BackupSlots[c_NumSlots];
	memcpy( BackupSlots, m_Slots, c_NumSlots * sizeof( cItem ) );

	bool ChangedSlots[c_NumSlots];
	memset( ChangedSlots, false, c_NumSlots * sizeof( bool ) );

	if( a_Item.m_ItemCount > 0 ) AddToBar( a_Item, c_HotOffset,  c_HotSlots,  ChangedSlots, 0 );
	if( a_Item.m_ItemCount > 0 ) AddToBar( a_Item, c_MainOffset, c_MainSlots, ChangedSlots, 0 );
	if( a_Item.m_ItemCount > 0 ) AddToBar( a_Item, c_HotOffset,  c_HotSlots,  ChangedSlots, 2 );
	if( a_Item.m_ItemCount > 0 ) AddToBar( a_Item, c_MainOffset, c_MainSlots, ChangedSlots, 2 );

	if( a_Item.m_ItemCount > 0 ) // Could not add all items
	{
		// retore backup
		memcpy( m_Slots, BackupSlots, c_NumSlots * sizeof( cItem ) );
		return false;
	}

	for(unsigned int i = 0; i < c_NumSlots; i++)
	{
		if( ChangedSlots[i] )
		{
			LOG("Item was added to %i ID:%i Count:%i", i, m_Slots[i].m_ItemID, m_Slots[i].m_ItemCount );
			SendSlot(i);
		}
	}

	return (a_Item.m_ItemCount == 0);
}





// TODO: Right now if you dont have enough items, the items you did have are removed, and the function returns false anyway
bool cInventory::RemoveItem( cItem & a_Item )
{
	// First check equipped slot
	if( m_EquippedSlot >= 0 && m_EquippedSlot < 9 )
	{
		if( m_HotSlots[m_EquippedSlot].m_ItemID == a_Item.m_ItemID )
		{
			cItem & Item = m_HotSlots[m_EquippedSlot];
			if(Item.m_ItemCount > a_Item.m_ItemCount)
			{
				Item.m_ItemCount -= a_Item.m_ItemCount;
				SendSlot( m_EquippedSlot + c_HotOffset );
				return true;
			}
			else if(Item.m_ItemCount > 0 )
			{
				a_Item.m_ItemCount -= Item.m_ItemCount;
				Item.Empty();
				SendSlot( m_EquippedSlot + c_HotOffset );
			}
		}
	}

	// Then check other slotz
	if( a_Item.m_ItemCount > 0 )
	{
		for(int i = 0; i < 36; i++)
		{
			cItem & Item = m_MainSlots[i];
			if( Item.m_ItemID == a_Item.m_ItemID )
			{
				if(Item.m_ItemCount > a_Item.m_ItemCount)
				{
					Item.m_ItemCount -= a_Item.m_ItemCount;
					SendSlot( i + c_MainOffset );
					return true;
				}
				else if(Item.m_ItemCount > 0 )
				{
					a_Item.m_ItemCount -= Item.m_ItemCount;
					Item.Empty();
					SendSlot( i + c_MainOffset );
				}
			}
		}
	}

	if( a_Item.m_ItemCount == 0 )
		return true;
	else
		return false;
}





void cInventory::Clear()
{
	for(unsigned int i = 0; i < c_NumSlots; i++)
		m_Slots[i].Empty();
}





cItem * cInventory::GetSlotsForType( int a_Type )
{
	switch( a_Type )
	{
	case -1:
		return m_MainSlots;
	case -2:
		return m_CraftSlots;
	case -3:
		return m_ArmorSlots;
	}
	return 0;
}





int cInventory::GetSlotCountForType( int a_Type )
{
	switch (a_Type)
	{
		case -1:
			return 36;
		case -2:
		case -3:
			return 4;
	}
	return 0;
}





cItem* cInventory::GetSlot( int a_SlotNum )
{
	if( a_SlotNum < 0 || a_SlotNum >= (short)c_NumSlots ) return 0;
	return &m_Slots[a_SlotNum];
}





cItem* cInventory::GetFromHotBar( int a_SlotNum )
{
	if ((a_SlotNum < 0) || (a_SlotNum >= 9))
	{
		return NULL;
	}
	return &m_HotSlots[a_SlotNum];
}





void cInventory::SetEquippedSlot( int a_SlotNum )
{
	if( a_SlotNum < 0 || a_SlotNum >= 9 ) m_EquippedSlot = 0;
	else m_EquippedSlot = (short)a_SlotNum;
}





cItem & cInventory::GetEquippedItem()
{
	cItem* Item = GetFromHotBar( m_EquippedSlot );
	if( Item )
	{
		*m_EquippedItem = *Item;
		return *Item;
	}
	else
	{
		m_EquippedItem->Empty();
	}
	return *m_EquippedItem;
}





void cInventory::SendWholeInventory( cClientHandle* a_Client )
{
	cPacket_WholeInventory Inventory( this );
	a_Client->Send( Inventory );
}





void cInventory::SendSlot( int a_SlotNum )
{
	cItem* Item = GetSlot( a_SlotNum );
	if( Item )
	{
		cPacket_InventorySlot InventorySlot;
		InventorySlot.m_ItemCount = Item->m_ItemCount;
		InventorySlot.m_ItemID = (short) Item->m_ItemID;
		InventorySlot.m_ItemUses = (char) Item->m_ItemHealth;
		InventorySlot.m_SlotNum = (short) a_SlotNum;
		InventorySlot.m_WindowID = 0; // Inventory window ID
		m_Owner->GetClientHandle()->Send( InventorySlot );
	}
}





bool cInventory::AddToBar( cItem & a_Item, const int a_Offset, const int a_Size, bool* a_bChangedSlots, int a_Mode /* = 0 */ )
{
	// Fill already present stacks
	if( a_Mode < 2 )
	{
		for(int i = 0; i < a_Size; i++)
		{
			if( m_Slots[i + a_Offset].m_ItemID == a_Item.m_ItemID && m_Slots[i + a_Offset].m_ItemCount < 64 && m_Slots[i + a_Offset].m_ItemHealth == a_Item.m_ItemHealth )
			{
				int NumFree = 64 - m_Slots[i + a_Offset].m_ItemCount;
				if( NumFree >= a_Item.m_ItemCount )
				{

					//printf("1. Adding %i items ( free: %i )\n", a_Item.m_ItemCount, NumFree );
					m_Slots[i + a_Offset].m_ItemCount += a_Item.m_ItemCount;
					a_Item.m_ItemCount = 0;
					a_bChangedSlots[i + a_Offset] = true;
					break;
				}
				else
				{
					//printf("2. Adding %i items\n", NumFree );
					m_Slots[i + a_Offset].m_ItemCount += (char)NumFree;
					a_Item.m_ItemCount -= (char)NumFree;
					a_bChangedSlots[i + a_Offset] = true;
				}
			}
		}
	}

	if( a_Mode > 0 )
	{
		// If we got more left, find first empty slot
		for(int i = 0; i < a_Size && a_Item.m_ItemCount > 0; i++)
		{
			if( m_Slots[i + a_Offset].m_ItemID == -1 )
			{
				m_Slots[i + a_Offset] = a_Item;
				a_Item.m_ItemCount = 0;
				a_bChangedSlots[i + a_Offset] = true;
			}
		}
	}

	return true;
}





void cInventory::SaveToJson(Json::Value & a_Value)
{
	for(unsigned int i = 0; i < c_NumSlots; i++)
	{
		Json::Value JSON_Item;
		m_Slots[i].GetJson( JSON_Item );
		a_Value.append( JSON_Item );
	}
}





bool cInventory::LoadFromJson(Json::Value & a_Value)
{
	int SlotIdx = 0;
	for( Json::Value::iterator itr = a_Value.begin(); itr != a_Value.end(); ++itr )
	{
		m_Slots[SlotIdx].FromJson( *itr );
		SlotIdx++;
	}
	return true;
}




