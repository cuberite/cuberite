#include "cInventory.h"
#include <string> //memset
#include "cPlayer.h"
#include "cClientHandle.h"
#include "cMCLogger.h"
#include "cWindow.h"
#include "cItem.h"
#include "cRecipeChecker.h"
#include "cRoot.h"

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

void cInventory::Clear()
{
	for(unsigned int i = 0; i < c_NumSlots; i++)
		m_Slots[i].Empty();
}

cItem* cInventory::GetSlotsForType( int a_Type )
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
	switch( a_Type )
	{
	case -1:
		return 36;
	case -2:
	case -3:
		return 4;
	}
	return 0;
}

void cInventory::Clicked( cPacket_WindowClick* a_ClickPacket )
{
	bool bDontCook = false;
	if( GetWindow() )
	{
		// Override for craft result slot
		if( a_ClickPacket->m_SlotNum == (short)c_CraftOffset )
		{
			LOG("In craft slot: %i x %i !!", m_Slots[c_CraftOffset].m_ItemID, m_Slots[c_CraftOffset].m_ItemCount );
			cItem* DraggingItem = GetWindow()->GetDraggingItem();
			if( DraggingItem->IsEmpty() )
			{
				*DraggingItem = m_Slots[c_CraftOffset];
				m_Slots[c_CraftOffset].Empty();
			}
			else if( DraggingItem->Equals( m_Slots[c_CraftOffset] ) )
			{
				if( DraggingItem->m_ItemCount + m_Slots[c_CraftOffset].m_ItemCount <= 64 )
				{
					DraggingItem->m_ItemCount += m_Slots[c_CraftOffset].m_ItemCount;
					m_Slots[0].Empty();
				}
				else
				{
					bDontCook = true;
				}
			}
			else
			{
				bDontCook = true;
			}
			LOG("Dragging Dish %i", DraggingItem->m_ItemCount );
		}
		else
		{
			GetWindow()->Clicked( a_ClickPacket, *m_Owner );
		}
	}
	else
	{
		LOG("No Inventory window! WTF");
	}

	if( a_ClickPacket->m_SlotNum >= (short)c_CraftOffset && a_ClickPacket->m_SlotNum < (short)(c_CraftOffset+c_CraftSlots+1) )
	{
		cItem CookedItem;
		if( a_ClickPacket->m_SlotNum == 0 && !bDontCook )
		{
			CookedItem = cRoot::Get()->GetRecipeChecker()->CookIngredients( m_Slots+c_CraftOffset+1, 2, 2, true );
		}
		else
		{
			CookedItem = cRoot::Get()->GetRecipeChecker()->CookIngredients( m_Slots+c_CraftOffset+1, 2, 2 );
		}
		m_Slots[c_CraftOffset] = CookedItem;
		LOG("You cooked: %i x %i !!", m_Slots[c_CraftOffset].m_ItemID, m_Slots[c_CraftOffset].m_ItemCount );
		SendWholeInventory( m_Owner->GetClientHandle() );
	}
	SendSlot( 0 );
}

bool cInventory::AddToBar( cItem & a_Item, const int a_Offset, const int a_Size, bool* a_bChangedSlots, int a_Mode /* = 0 */ )
{
	// Fill already present stacks
	if( a_Mode < 2 )
	{
		for(int i = 0; i < a_Size; i++)
		{
			if( m_Slots[i + a_Offset].m_ItemID == a_Item.m_ItemID && m_Slots[i + a_Offset].m_ItemCount < 64 )
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

cItem* cInventory::GetSlot( int a_SlotNum )
{
	if( a_SlotNum < 0 || a_SlotNum >= (short)c_NumSlots ) return 0;
	return &m_Slots[a_SlotNum];
}

cItem* cInventory::GetFromHotBar( int a_SlotNum )
{
	if( a_SlotNum < 0 || a_SlotNum >= 9 ) return 0;
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
		InventorySlot.m_ItemID = (short)Item->m_ItemID;
		InventorySlot.m_ItemUses = (char)Item->m_ItemHealth;
		InventorySlot.m_SlotNum = (short)a_SlotNum;
		InventorySlot.m_WindowID = 0; // Inventory window ID
		m_Owner->GetClientHandle()->Send( InventorySlot );
	}
}

void cInventory::DrawInventory()
{
	printf("%i %i           %i %i\n", m_ArmorSlots[0].m_ItemCount, m_ArmorSlots[1].m_ItemCount, m_CraftSlots[0].m_ItemCount, m_CraftSlots[1].m_ItemCount );
	printf("%i %i           %i %i\n", m_ArmorSlots[2].m_ItemCount, m_ArmorSlots[3].m_ItemCount, m_CraftSlots[2].m_ItemCount, m_CraftSlots[3].m_ItemCount );
	for(int y = 0; y < 4; y++)
	{
		for(int x = 0; x < 9; x++)
		{
			printf("%i ", m_MainSlots[x + y*9].m_ItemCount );
		}
		printf("\n");
	}
}

void cInventory::WriteToFile(FILE* a_File)
{
	for(unsigned int i = 0; i < c_NumSlots; i++)
	{
		cItem & Item = m_Slots[i];
		fwrite( &Item.m_ItemID, sizeof(Item.m_ItemID), 1, a_File );
		fwrite( &Item.m_ItemCount, sizeof(Item.m_ItemCount), 1, a_File );
		fwrite( &Item.m_ItemHealth, sizeof(Item.m_ItemHealth), 1, a_File );
	}
}

bool cInventory::LoadFromFile(FILE* a_File)
{
	for(unsigned int i = 0; i < c_NumSlots; i++)
	{
		cItem & Item = m_Slots[i];
		if( fread( &Item.m_ItemID,		sizeof(Item.m_ItemID), 1, a_File)	 != 1 ) { LOGERROR("ERROR READING INVENTORY FROM FILE"); return false; }
		if( fread( &Item.m_ItemCount,	sizeof(Item.m_ItemCount), 1, a_File) != 1 ) { LOGERROR("ERROR READING INVENTORY FROM FILE"); return false; }
		if( fread( &Item.m_ItemHealth,	sizeof(Item.m_ItemHealth), 1, a_File)!= 1 ) { LOGERROR("ERROR READING INVENTORY FROM FILE"); return false; }
	}
	return true;
}
