#include "cFurnaceEntity.h"
#include "BlockID.h"
#include "cItem.h"
#include "cFurnaceWindow.h"
#include "cPlayer.h"
#include "cWorld.h"
#include "cChunk.h"
#include "cClientHandle.h"
#include "cFurnaceRecipe.h"
#include "cServer.h"
#include "cPickup.h"
#include "cRoot.h"

#include "packets/cPacket_InventoryProgressBar.h"

#include "cMCLogger.h"

#include <json/json.h>

cFurnaceEntity::cFurnaceEntity(int a_X, int a_Y, int a_Z, cChunk* a_Chunk)
	: cBlockEntity( E_BLOCK_FURNACE, a_X, a_Y, a_Z, a_Chunk ) 
	, m_Items( new cItem[3] )
	, m_CookingItem( 0 )
	, m_CookTime( 0 )
	, m_TimeCooked( 0 )
	, m_BurnTime( 0 )
	, m_TimeBurned( 0 )
{
}

cFurnaceEntity::~cFurnaceEntity()
{
	// Tell window its owner is destroyed
	if( GetWindow() )
	{
		GetWindow()->OwnerDestroyed();
	}

	// Clean up items
	if( m_Items )
	{
		delete [] m_Items;
	}
}

void cFurnaceEntity::Destroy()
{
	// Drop items
	for( int i = 0; i < 3; i++)
	{
		if( !m_Items[i].IsEmpty() )
		{
			cPickup* Pickup = new cPickup( m_PosX*32 + 16, m_PosY*32 + 16, m_PosZ*32 + 16, m_Items[i], 0, 1.f, 0 );
			Pickup->Initialize( m_Chunk->GetWorld() );
			m_Items[i].Empty();
		}
	}

	// Remove from tick list
	GetChunk()->RemoveTickBlockEntity( this );
}

void cFurnaceEntity::UsedBy( cPlayer & a_Player )
{
	LOG("Used a furnace");

	if( !GetWindow() )
	{
		cWindow* Window = new cFurnaceWindow( this );
		Window->SetSlots( m_Items, 3 );
		Window->SetWindowTitle("UberFurnace");
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

bool cFurnaceEntity::Tick( float a_Dt )
{
	//LOG("Time left: %0.1f Time burned: %0.1f Burn time: %0.1f", m_CookTime - m_TimeCooked, m_TimeBurned, m_BurnTime );
	if( m_CookingItem && ( (m_TimeBurned < m_BurnTime) || (m_TimeCooked + a_Dt >= m_CookTime) ) )
	{
		if( m_CookingItem->Equals( m_Items[2] ) || m_Items[2].IsEmpty() )
		{
			m_TimeCooked += a_Dt;
			if( m_TimeCooked >= m_CookTime )
			{
				m_Items[0].m_ItemCount--;
				if( m_Items[0].IsEmpty() ) m_Items[0].Empty();

				m_Items[2].m_ItemHealth = m_CookingItem->m_ItemHealth;
				m_Items[2].m_ItemID = m_CookingItem->m_ItemID;
				m_Items[2].m_ItemCount += m_CookingItem->m_ItemCount;
				delete m_CookingItem;
				m_CookingItem = 0;

				cWindow* Window = GetWindow();
				if( Window )
				{
					const std::list< cPlayer* > & OpenedBy = Window->GetOpenedBy();
					for( std::list< cPlayer* >::const_iterator itr = OpenedBy.begin(); itr != OpenedBy.end(); ++itr )
					{
						Window->SendWholeWindow( (*itr)->GetClientHandle() );
					}
				}

				m_TimeCooked = 0.f;
				StartCooking();
			}
			cWindow* Window = GetWindow();
			if( Window )
			{
				const std::list< cPlayer* > & OpenedBy = Window->GetOpenedBy();
				for( std::list< cPlayer* >::const_iterator itr = OpenedBy.begin(); itr != OpenedBy.end(); ++itr )
				{
					cClientHandle* Client = (*itr)->GetClientHandle();

					cPacket_InventoryProgressBar Progress;
					Progress.m_ProgressBar = 0;
					Progress.m_WindowID = (char)Window->GetWindowID();
					Progress.m_Value = (short)( m_TimeCooked * (180.f/m_CookTime) );
					if( Progress.m_Value > 180 ) Progress.m_Value = 180;
					if( Progress.m_Value < 0 ) Progress.m_Value = 0;
					Client->Send( Progress );
				}
			}
		}
	}

	m_TimeBurned += a_Dt;

	cWindow* Window = GetWindow();
	if( m_TimeBurned >= m_BurnTime )
	{
		m_TimeBurned -= m_BurnTime;
		m_BurnTime = 0;
		if( StartCooking() && Window )
		{
			const std::list< cPlayer* > & OpenedBy = Window->GetOpenedBy();
			for( std::list< cPlayer* >::const_iterator itr = OpenedBy.begin(); itr != OpenedBy.end(); ++itr )
			{
				Window->SendWholeWindow( (*itr)->GetClientHandle() );
			}
		}
	}
	if( Window )
	{
		const std::list< cPlayer* > & OpenedBy = Window->GetOpenedBy();
		for( std::list< cPlayer* >::const_iterator itr = OpenedBy.begin(); itr != OpenedBy.end(); ++itr )
		{
			cClientHandle* Client = (*itr)->GetClientHandle();

			cPacket_InventoryProgressBar Progress;
			Progress.m_WindowID = (char)Window->GetWindowID();
			Progress.m_ProgressBar = 1;

			if( m_BurnTime > 0.f )	Progress.m_Value = (short)( m_TimeBurned * (150.f/m_BurnTime) );
			else					Progress.m_Value = 0;
			if( Progress.m_Value > 150 ) Progress.m_Value = 150;
			if( Progress.m_Value < 0 ) Progress.m_Value = 0;
			Client->Send( Progress );
		}
	}
	return ((m_CookingItem != 0) || (m_TimeBurned < m_BurnTime)) && m_BurnTime > 0.f; // Keep on ticking, if there's more to cook, or if it's cooking
}

bool cFurnaceEntity::StartCooking()
{
	cFurnaceRecipe* FR = cRoot::Get()->GetFurnaceRecipe();
	float BurnTime = FR->GetBurnTime( m_Items[1] );
	if( (m_TimeBurned < m_BurnTime) || BurnTime > 0.f ) // burnable material
	{
		const cFurnaceRecipe::Recipe* R = FR->GetRecipeFrom( m_Items[0] );
		if( R ) // cook able ingredient
		{
			if( m_Items[2].Equals( *R->Out ) || m_Items[2].IsEmpty() )
			{
				// good to go

				if( m_TimeBurned >= m_BurnTime ) // burn new material
				{
					m_Items[1].m_ItemCount--;
					if( m_Items[1].m_ItemCount <= 0 ) m_Items[1].Empty();
					m_TimeBurned = 0;
					m_BurnTime = BurnTime;
				}

				if( !m_CookingItem ) // Only cook new item if not already cooking
				{
					m_CookingItem = new cItem( *R->Out ); // Resulting item
					m_TimeCooked = 0.f;
					m_CookTime = R->CookTime;
				}
				GetChunk()->AddTickBlockEntity( this );
				return true;
			}
		}
	}
	return false;
}

void cFurnaceEntity::ResetCookTimer()
{
	if( m_CookingItem )
	{
		delete m_CookingItem;
		m_CookingItem = 0;
	}
	m_TimeCooked = 0.f;
	m_CookTime = 0.f;
}

void cFurnaceEntity::WriteToFile(FILE* a_File)
{
	fwrite( &m_BlockType, sizeof( ENUM_BLOCK_ID ), 1, a_File );
	fwrite( &m_PosX, sizeof( int ), 1, a_File );
	fwrite( &m_PosY, sizeof( int ), 1, a_File );
	fwrite( &m_PosZ, sizeof( int ), 1, a_File );

	unsigned int NumSlots = 3;
	fwrite( &NumSlots, sizeof(unsigned int), 1, a_File );
	for(unsigned int i = 0; i < NumSlots; i++)
	{
		cItem* Item = &m_Items[i];
		if( Item )
		{
			fwrite( &Item->m_ItemID, sizeof(Item->m_ItemID), 1, a_File );
			fwrite( &Item->m_ItemCount, sizeof(Item->m_ItemCount), 1, a_File );
			fwrite( &Item->m_ItemHealth, sizeof(Item->m_ItemHealth), 1, a_File );
		}
	}
	cItem Item;
	if( m_CookingItem ) Item = *m_CookingItem;
	fwrite( &Item.m_ItemID,		sizeof(Item.m_ItemID),		1, a_File );
	fwrite( &Item.m_ItemCount,	sizeof(Item.m_ItemCount),	1, a_File );
	fwrite( &Item.m_ItemHealth, sizeof(Item.m_ItemHealth),	1, a_File );

	fwrite( &m_CookTime,	sizeof(float), 1, a_File );
	fwrite( &m_TimeCooked,	sizeof(float), 1, a_File );
	fwrite( &m_BurnTime,	sizeof(float), 1, a_File );
	fwrite( &m_TimeBurned,	sizeof(float), 1, a_File );
}

bool cFurnaceEntity::LoadFromFile(FILE* a_File)
{
	if( fread( &m_PosX, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	if( fread( &m_PosY, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	if( fread( &m_PosZ, sizeof(int), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }

	unsigned int NumSlots = 0;
	if( fread( &NumSlots, sizeof(unsigned int), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	m_Items = new cItem[ NumSlots ];
	for(unsigned int i = 0; i < NumSlots; i++)
	{
		cItem & Item = m_Items[ i ];
		if( fread( &Item.m_ItemID,		sizeof(Item.m_ItemID), 1, a_File)	 != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
		if( fread( &Item.m_ItemCount,	sizeof(Item.m_ItemCount), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
		if( fread( &Item.m_ItemHealth,	sizeof(Item.m_ItemHealth), 1, a_File)!= 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	}
	cItem Item;
	if( fread( &Item.m_ItemID,		sizeof(Item.m_ItemID), 1, a_File)	 != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	if( fread( &Item.m_ItemCount,	sizeof(Item.m_ItemCount), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	if( fread( &Item.m_ItemHealth,	sizeof(Item.m_ItemHealth), 1, a_File)!= 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	if( !Item.IsEmpty() ) m_CookingItem = new cItem( Item );

	if( fread( &m_CookTime,		sizeof(float), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	if( fread( &m_TimeCooked,	sizeof(float), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	if( fread( &m_BurnTime,		sizeof(float), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }
	if( fread( &m_TimeBurned,	sizeof(float), 1, a_File) != 1 ) { LOGERROR("ERROR READING FURNACE FROM FILE"); return false; }

	return true;
}

bool cFurnaceEntity::LoadFromJson( const Json::Value& a_Value )
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for( Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr )
	{
		Json::Value & Slot = *itr;
		cItem & Item = m_Items[ SlotIdx ];
		Item.m_ItemID = (ENUM_ITEM_ID)Slot.get("ID", -1 ).asInt();
		if( Item.m_ItemID > 0 )
		{
			Item.m_ItemCount = (char)Slot.get("Count", -1 ).asInt();
			Item.m_ItemHealth = (short)Slot.get("Health", -1 ).asInt();
		}
		SlotIdx++;
	}

	// Get currently cooking item
	Json::Value JsonItem = a_Value.get("Cooking", Json::nullValue );
	if( !JsonItem.empty() )
	{
		cItem Item;
		Item.m_ItemID = (ENUM_ITEM_ID)JsonItem.get("ID", -1).asInt();
		if( Item.m_ItemID > 0 )
		{
			Item.m_ItemCount = (char)JsonItem.get("Count", -1).asInt();
			Item.m_ItemHealth = (short)JsonItem.get("Health", -1).asInt();
		}
		if( !Item.IsEmpty() )
		{
			m_CookingItem = new cItem( Item );
			GetChunk()->AddTickBlockEntity( this );
		}
	}

	m_CookTime = (float)a_Value.get("CookTime", 0).asDouble();
	m_TimeCooked = (float)a_Value.get("TimeCooked", 0).asDouble();
	m_BurnTime = (float)a_Value.get("BurnTime", 0).asDouble();
	m_TimeBurned = (float)a_Value.get("TimeBurned", 0).asDouble();

	return true;
}

void cFurnaceEntity::SaveToJson( Json::Value& a_Value )
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	Json::Value AllSlots;
	for(unsigned int i = 0; i < 3; i++)
	{
		Json::Value Slot;
		cItem & Item = m_Items[ i ];
		Slot["ID"] = Item.m_ItemID;
		if( Item.m_ItemID > 0 )
		{
			Slot["Count"] = Item.m_ItemCount;
			Slot["Health"] = Item.m_ItemHealth;
		}
		AllSlots.append( Slot );
	}
	a_Value["Slots"] = AllSlots;

	// Currently cooking item
	if( m_CookingItem )
	{
		Json::Value JsonItem;
		JsonItem["ID"] = m_CookingItem->m_ItemID;
		if( m_CookingItem->m_ItemID > 0 )
		{
			JsonItem["Count"] = m_CookingItem->m_ItemCount;
			JsonItem["Health"] = m_CookingItem->m_ItemHealth;
		}
		a_Value["Cooking"] = JsonItem;
	}

	a_Value["CookTime"] = m_CookTime;
	a_Value["TimeCooked"] = m_TimeCooked;
	a_Value["BurnTime"] = m_BurnTime;
	a_Value["TimeBurned"] = m_TimeBurned;
}