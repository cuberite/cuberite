#include "cChestEntity.h"
#include "cItem.h"
#include <string>
#include "cClientHandle.h"
#include "cMCLogger.h"
#include "cPlayer.h"
#include "cWindow.h"
#include "cPickup.h"
#include "cMCLogger.h"

#include <json/json.h>

cChestEntity::cChestEntity(int a_X, int a_Y, int a_Z)
	: cBlockEntity( E_BLOCK_CHEST, a_X, a_Y, a_Z ) 
{
	m_Content = new cItem[ c_ChestHeight*c_ChestWidth ];
}

cChestEntity::~cChestEntity()
{
	if( GetWindow() )
	{
		GetWindow()->OwnerDestroyed();
	}

	if( m_Content )
	{
		delete [] m_Content;
	}
}

void cChestEntity::Destroy()
{
	// Drop items
	for( int i = 0; i < c_ChestHeight*c_ChestWidth; ++i )
	{
		if( !m_Content[i].IsEmpty() )
		{
			cPickup* Pickup = new cPickup( m_PosX*32 + 16, m_PosY*32 + 16, m_PosZ*32 + 16, m_Content[i], 0, 1.f, 0 );
			Pickup->Initialize();
			m_Content[i].Empty();
		}
	}
}

cItem * cChestEntity::GetSlot( int a_Slot )
{
	if( a_Slot > -1 && a_Slot < c_ChestHeight*c_ChestWidth )
	{
		return &m_Content[ a_Slot ];
	}
	return 0;
}

void cChestEntity::SetSlot( int a_Slot, cItem & a_Item )
{
	if( a_Slot > -1 && a_Slot < c_ChestHeight*c_ChestWidth )
	{
		m_Content[a_Slot] = a_Item;
	}
}

void cChestEntity::WriteToFile(FILE* a_File)
{
	fwrite( &m_BlockType, sizeof( ENUM_BLOCK_ID ), 1, a_File );
	fwrite( &m_PosX, sizeof( int ), 1, a_File );
	fwrite( &m_PosY, sizeof( int ), 1, a_File );
	fwrite( &m_PosZ, sizeof( int ), 1, a_File );

	unsigned int NumSlots = c_ChestHeight*c_ChestWidth;
	fwrite( &NumSlots, sizeof(unsigned int), 1, a_File );
	for(unsigned int i = 0; i < NumSlots; i++)
	{
		cItem* Item = GetSlot( i );
		if( Item )
		{
			fwrite( &Item->m_ItemID, sizeof(Item->m_ItemID), 1, a_File );
			fwrite( &Item->m_ItemCount, sizeof(Item->m_ItemCount), 1, a_File );
			fwrite( &Item->m_ItemHealth, sizeof(Item->m_ItemHealth), 1, a_File );
		}
	}
}

bool cChestEntity::LoadFromFile(FILE* a_File)
{
	if( fread( &m_PosX, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING CHEST FROM FILE"); return false; }
	if( fread( &m_PosY, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING CHEST FROM FILE"); return false; }
	if( fread( &m_PosZ, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING CHEST FROM FILE"); return false; }

	unsigned int NumSlots = 0;
	if( fread( &NumSlots, sizeof(unsigned int), 1, a_File) != 1 ) { LOGERROR("ERROR READING CHEST FROM FILE"); return false; }
	for(unsigned int i = 0; i < NumSlots; i++)
	{
		cItem Item;
		if( fread( &Item.m_ItemID,		sizeof(Item.m_ItemID), 1, a_File)	 != 1 ) { LOGERROR("ERROR READING CHEST FROM FILE"); return false; }
		if( fread( &Item.m_ItemCount,	sizeof(Item.m_ItemCount), 1, a_File) != 1 ) { LOGERROR("ERROR READING CHEST FROM FILE"); return false; }
		if( fread( &Item.m_ItemHealth,	sizeof(Item.m_ItemHealth), 1, a_File)!= 1 ) { LOGERROR("ERROR READING CHEST FROM FILE"); return false; }
		SetSlot( i, Item );
	}
	return true;
}

bool cChestEntity::LoadFromJson( const Json::Value& a_Value )
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for( Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr )
	{
		Json::Value & Slot = *itr;
		cItem Item;
		Item.m_ItemID = (ENUM_ITEM_ID)Slot.get("ID", -1 ).asInt();
		if( Item.m_ItemID > 0 )
		{
			Item.m_ItemCount = (char)Slot.get("Count", -1 ).asInt();
			Item.m_ItemHealth = (short)Slot.get("Health", -1 ).asInt();
		}
		SetSlot( SlotIdx, Item );
		SlotIdx++;
	}
	return true;
}

void cChestEntity::SaveToJson( Json::Value& a_Value )
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	unsigned int NumSlots = c_ChestHeight*c_ChestWidth;
	Json::Value AllSlots;
	for(unsigned int i = 0; i < NumSlots; i++)
	{
		Json::Value Slot;
		cItem* Item = GetSlot( i );
		if( Item )
		{
			Slot["ID"] = Item->m_ItemID;
			if( Item->m_ItemID > 0 )
			{
				Slot["Count"] = Item->m_ItemCount;
				Slot["Health"] = Item->m_ItemHealth;
			}
		}
		AllSlots.append( Slot );
	}
	a_Value["Slots"] = AllSlots;
}

void cChestEntity::SendTo( cClientHandle* a_Client, cServer* a_Server )
{
	(void)a_Client;
	(void)a_Server;
	return;
}

void cChestEntity::UsedBy( cPlayer & a_Player )
{
	LOG("Used a chest");
// 	m_Content[0].m_ItemCount	= 1;
// 	m_Content[0].m_ItemID		= E_ITEM_STONE;
// 	m_Content[0].m_ItemHealth	= 0;

	if( !GetWindow() )
	{
		cWindow* Window = new cWindow( this, true );
		Window->SetSlots( m_Content, c_ChestHeight*c_ChestWidth );
		Window->SetWindowID( 1 );
		Window->SetWindowType( 0 );
		Window->SetWindowTitle("UberChest");
		OpenWindow( Window );
	}
	if( GetWindow() )
	{
		if( a_Player.GetWindow() != GetWindow() )
		{
			a_Player.OpenWindow( GetWindow() );

			GetWindow()->SendWholeWindow( a_Player.GetClientHandle() );
		}
	}
}
