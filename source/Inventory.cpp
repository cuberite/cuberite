
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Inventory.h"
#include "Player.h"
#include "ClientHandle.h"
#include "UI/Window.h"
#include "Item.h"
#include "Root.h"
#include "World.h"

#include <json/json.h>

#include "Items/ItemHandler.h"





cInventory::cInventory(cPlayer & a_Owner) :
	m_Owner(a_Owner)
{
	m_CraftSlots = m_Slots + c_CraftOffset;
	m_ArmorSlots = m_Slots + c_ArmorOffset;
	m_MainSlots  = m_Slots + c_MainOffset;
	m_HotSlots   = m_Slots + c_HotOffset;

	SetEquippedSlotNum(0);
}





cInventory::~cInventory()
{
	/*
	// TODO
	cWindow wnd = GetWindow();
	if (wnd != NULL)
	{
		wnd->Close(*m_Owner);
	}
	CloseWindow();
	*/
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

	for (unsigned int i = 0; i < c_NumSlots; i++)
	{
		if (ChangedSlots[i])
		{
			LOGD("cInventory::AddItem(): Item was added to %i ID:%i Count:%i", i, m_Slots[i].m_ItemType, m_Slots[i].m_ItemCount);
			SendSlot(i);
		}
	}

	return (a_Item.m_ItemCount == 0);
}





bool cInventory::AddItemAnyAmount( cItem & a_Item )
{
	bool ChangedSlots[c_NumSlots];
	memset( ChangedSlots, false, c_NumSlots * sizeof( bool ) );

	char StartCount = a_Item.m_ItemCount;
	if( a_Item.m_ItemCount > 0 ) AddToBar( a_Item, c_HotOffset,  c_HotSlots,  ChangedSlots, 0 );
	if( a_Item.m_ItemCount > 0 ) AddToBar( a_Item, c_MainOffset, c_MainSlots, ChangedSlots, 0 );
	if( a_Item.m_ItemCount > 0 ) AddToBar( a_Item, c_HotOffset,  c_HotSlots,  ChangedSlots, 2 );
	if( a_Item.m_ItemCount > 0 ) AddToBar( a_Item, c_MainOffset, c_MainSlots, ChangedSlots, 2 );
	
	if (a_Item.m_ItemCount == StartCount)
		return false;

	for (unsigned int i = 0; i < c_NumSlots; i++)
	{
		if (ChangedSlots[i])
		{
			LOGD("cInventory::AddItemAnyAmount(): Item was added to %i ID:%i Count:%i", i, m_Slots[i].m_ItemType, m_Slots[i].m_ItemCount);
			SendSlot(i);
		}
	}

	return true;
}





// TODO: Right now if you dont have enough items, the items you did have are removed, and the function returns false anyway
bool cInventory::RemoveItem(cItem & a_Item)
{
	// First check equipped slot
	if ((m_EquippedSlotNum >= 0) && (m_EquippedSlotNum < 9))
	{
		if (m_HotSlots[m_EquippedSlotNum].m_ItemType == a_Item.m_ItemType)
		{
			cItem & Item = m_HotSlots[m_EquippedSlotNum];
			if (Item.m_ItemCount > a_Item.m_ItemCount)
			{
				Item.m_ItemCount -= a_Item.m_ItemCount;
				SendSlot(m_EquippedSlotNum + c_HotOffset);
				return true;
			}
			else if (Item.m_ItemCount > 0)
			{
				a_Item.m_ItemCount -= Item.m_ItemCount;
				Item.Empty();
				SendSlot(m_EquippedSlotNum + c_HotOffset);
			}
		}
	}

	// Then check other slotz
	if (a_Item.m_ItemCount > 0)
	{
		for (int i = 0; i < c_MainSlots; i++)
		{
			cItem & Item = m_MainSlots[i];
			if (Item.m_ItemType == a_Item.m_ItemType)
			{
				if (Item.m_ItemCount > a_Item.m_ItemCount)
				{
					Item.m_ItemCount -= a_Item.m_ItemCount;
					SendSlot(i + c_MainOffset);
					return true;
				}
				else if (Item.m_ItemCount > 0)
				{
					a_Item.m_ItemCount -= Item.m_ItemCount;
					Item.Empty();
					SendSlot(i + c_MainOffset);
				}
			}
		}
	}

	return (a_Item.m_ItemCount == 0);
}





void cInventory::Clear()
{
	for (unsigned int i = 0; i < ARRAYCOUNT(m_Slots); i++)
	{
		m_Slots[i].Empty();
	}
	// TODO: Broadcast / send the changes to wherever needed
}





void cInventory::SetSlot(int a_SlotNum, const cItem & a_Item)
{
	if ((a_SlotNum < 0) || (a_SlotNum >= ARRAYCOUNT(m_Slots)))
	{
		LOGWARNING("%s requesting an invalid slot index: %d out of %d. Ignoring.", __FUNCTION__, a_SlotNum, ARRAYCOUNT(m_Slots));
		return;
	}
	m_Slots[a_SlotNum] = a_Item;
	
	// If an armor slot was touched, broadcast an EntityEquipment packet
	if ((a_SlotNum >= c_ArmorOffset) && (a_SlotNum < c_MainOffset))
	{
		m_Owner.GetWorld()->BroadcastEntityEquipment(m_Owner, SlotNumToEntityEquipmentID(a_SlotNum), a_Item, m_Owner.GetClientHandle());
	}
}





void cInventory::SetHotBarSlot(int a_HotBarSlotNum, const cItem & a_Item)
{
	SetSlot(a_HotBarSlotNum + c_HotSlots, a_Item);
}





const cItem & cInventory::GetSlot(int a_SlotNum) const
{
	if ((a_SlotNum < 0) || (a_SlotNum >= ARRAYCOUNT(m_Slots)))
	{
		LOGWARNING("%s requesting an invalid slot index: %d out of %d. Returning the first one instead.", __FUNCTION__, a_SlotNum, ARRAYCOUNT(m_Slots));
		return m_Slots[0];
	}
	
	return m_Slots[a_SlotNum];
}





const cItem & cInventory::GetHotBarSlot(int a_SlotNum) const
{
	if ((a_SlotNum < 0) || (a_SlotNum >= 9))
	{
		LOGWARNING("%s requesting an invalid slot index: %d out of 9. Returning the first one instead", __FUNCTION__, a_SlotNum);
		return m_HotSlots[0];
	}
	return m_HotSlots[a_SlotNum];
}





const cItem & cInventory::GetEquippedItem(void) const
{
	return GetHotBarSlot(m_EquippedSlotNum);
}





void cInventory::SetEquippedSlotNum(int a_SlotNum)
{
	if ((a_SlotNum < 0) || (a_SlotNum >= 9))
	{
		LOGWARNING("%s requesting invalid slot index: %d out of 9. Setting 0 instead.", __FUNCTION__, a_SlotNum);
		m_EquippedSlotNum = 0;
	}
	else
	{
		m_EquippedSlotNum = (short)a_SlotNum;
	}
}





bool cInventory::DamageEquippedItem(short a_Amount)
{
	return DamageItem(c_HotOffset + m_EquippedSlotNum, a_Amount);
}





bool cInventory::DamageItem(int a_SlotNum, short a_Amount)
{
	if ((a_SlotNum < 0) || (a_SlotNum >= ARRAYCOUNT(m_Slots)))
	{
		LOGWARNING("%s requesting an invalid slot index: %d out of %d", __FUNCTION__, a_SlotNum, ARRAYCOUNT(m_Slots));
		return false;
	}
	
	if (!m_Slots[a_SlotNum].DamageItem(a_Amount))
	{
		return false;
	}
	
	// The item has broken, remove it:
	m_Slots[a_SlotNum].Empty();
	SendSlot(a_SlotNum);
	
	// TODO: If it was a special slot (armor / equipped), broadcast the change
	return true;
}





void cInventory::SendWholeInventory(cClientHandle & a_Client)
{
	a_Client.SendWholeInventory(*this);
}





void cInventory::SendSlot(int a_SlotNum)
{
	cItem Item(GetSlot(a_SlotNum));
	if (Item.IsEmpty())
	{
		// Sanitize items that are not completely empty (ie. count == 0, but type != empty)
		Item.Empty();
	}
	m_Owner.GetClientHandle()->SendInventorySlot(0, a_SlotNum, Item);
}





int cInventory::HowManyCanFit(short a_ItemType, short a_ItemDamage, int a_BeginSlot, int a_EndSlot)
{
	int res = 0;
	for (int i = a_BeginSlot; i <= a_EndSlot; i++)
	{
		if (
			m_Slots[i].IsEmpty() ||
			((m_Slots[i].m_ItemType == a_ItemType) && (m_Slots[i].m_ItemDamage == a_ItemDamage))
		)
		{
			int MaxCount = ItemHandler(a_ItemType)->GetMaxStackSize();
			ASSERT(m_Slots[i].m_ItemCount <= MaxCount);
			res += MaxCount - m_Slots[i].m_ItemCount;
		}
	}  // for i - m_Slots[]
	return res;
}





int cInventory::MoveItem(short a_ItemType, short a_ItemDamage, int a_Count, int a_BeginSlot, int a_EndSlot)
{
	int res = 0;
	for (int i = a_BeginSlot; i <= a_EndSlot; i++)
	{
		if (
			m_Slots[i].IsEmpty() ||
			((m_Slots[i].m_ItemType == a_ItemType) && (m_Slots[i].m_ItemDamage == a_ItemDamage))
		)
		{
			int MaxCount = ItemHandler(a_ItemType)->GetMaxStackSize();
			ASSERT(m_Slots[i].m_ItemCount <= MaxCount);
			int NumToMove = std::min(a_Count, MaxCount - m_Slots[i].m_ItemCount);
			m_Slots[i].m_ItemCount += NumToMove;
			m_Slots[i].m_ItemDamage = a_ItemDamage;
			m_Slots[i].m_ItemType = a_ItemType;
			SendSlot(i);
			res += NumToMove;
			a_Count -= NumToMove;
			if (a_Count <= 0)
			{
				// No more items to distribute
				return res;
			}
		}
	}  // for i - m_Slots[]
	// No more space to distribute to
	return res;
}





int cInventory::SlotNumToEntityEquipmentID(short a_SlotNum)
{
	switch (a_SlotNum)
	{
		case 5: return 4;  // Helmet
		case 6: return 3;  // Chestplate
		case 7: return 2;  // Leggings
		case 8: return 1;  // Boots
	}
	LOGWARN("%s: invalid slot number: %d", __FUNCTION__, a_SlotNum);
	return 0;
}





bool cInventory::AddToBar( cItem & a_Item, const int a_Offset, const int a_Size, bool* a_bChangedSlots, int a_Mode /* = 0 */ )
{
	// Fill already present stacks
	if( a_Mode < 2 )
	{
		int MaxStackSize = cItemHandler::GetItemHandler(a_Item.m_ItemType)->GetMaxStackSize();
		for(int i = 0; i < a_Size; i++)
		{
			if( m_Slots[i + a_Offset].m_ItemType == a_Item.m_ItemType && m_Slots[i + a_Offset].m_ItemCount < MaxStackSize && m_Slots[i + a_Offset].m_ItemDamage == a_Item.m_ItemDamage )
			{
				int NumFree = MaxStackSize - m_Slots[i + a_Offset].m_ItemCount;
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
			if( m_Slots[i + a_Offset].m_ItemType == -1 )
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
		if (SlotIdx >= ARRAYCOUNT(m_Slots))
		{
			break;
		}
	}
	return true;
}




