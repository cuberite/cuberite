
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FurnaceEntity.h"
#include "BlockID.h"
#include "Item.h"
#include "UI/Window.h"
#include "Player.h"
#include "World.h"
#include "ClientHandle.h"
#include "FurnaceRecipe.h"
#include "Server.h"
#include "Pickup.h"
#include "Root.h"
#include <json/json.h>






enum
{
	PROGRESSBAR_SMELTING = 0,
	PROGRESSBAR_FUEL = 1,
} ;





cFurnaceEntity::cFurnaceEntity(int a_X, int a_Y, int a_Z, cWorld * a_World)
	: cBlockEntity( E_BLOCK_FURNACE, a_X, a_Y, a_Z, a_World )
	, m_Items( new cItem[3] )
	, m_CookingItem( 0 )
	, m_CookTime( 0 )
	, m_TimeCooked( 0 )
	, m_BurnTime( 0 )
	, m_TimeBurned( 0 )
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
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
	cItems Pickups;
	for( int i = 0; i < 3; i++)
	{
		if( !m_Items[i].IsEmpty() )
		{
			Pickups.push_back(m_Items[i]);
			m_Items[i].Empty();
		}
	}
	m_World->SpawnItemPickups(Pickups, m_PosX, m_PosY, m_PosZ);
}





void cFurnaceEntity::UsedBy(cPlayer * a_Player)
{
	if (GetWindow() == NULL)
	{
		OpenWindow(new cFurnaceWindow(m_PosX, m_PosY, m_PosZ, this));
	}
	if (GetWindow() != NULL)
	{
		if (a_Player->GetWindow() != GetWindow())
		{
			a_Player->OpenWindow(GetWindow());
			GetWindow()->SendWholeWindow(*a_Player->GetClientHandle());
		}
	}
}





bool cFurnaceEntity::Tick(float a_Dt, cChunk & a_Chunk)
{
	/*
	// DEBUG:
	Int16 BurnTime = (Int16)(GetTimeToBurn() / 50.0);
	Int16 CookTime = (Int16)(GetTimeCooked() / 50.0);
	LOGD("Furnace: BurnTime %d, CookTime %d", BurnTime, CookTime);
	*/

	if (m_BurnTime <= 0)
	{
		if (m_TimeCooked > 0)
		{
			// We have just finished smelting, reset the progress bar:
			BroadcastProgress(PROGRESSBAR_SMELTING, 0);
			m_TimeCooked = 0;
			m_World->FastSetBlock(m_PosX, m_PosY, m_PosZ, E_BLOCK_FURNACE, m_World->GetBlockMeta(m_PosX, m_PosY, m_PosZ));
		}
		// There is no fuel and no flame, no need to tick at all
		return false;
	}
	
	// DEBUG: LOGD("Furnace: Left: %0.1f Burned: %0.1f Burn time: %0.1f", m_CookTime - m_TimeCooked, m_TimeBurned, m_BurnTime );
	
	short SmeltingProgress = 0;
	if ((m_CookingItem != NULL) && ((m_TimeBurned < m_BurnTime) || (m_TimeCooked + a_Dt >= m_CookTime)))
	{
		if (m_CookingItem->IsEqual(m_Items[2]) || m_Items[2].IsEmpty())
		{
			m_TimeCooked += a_Dt;
			if ( m_TimeCooked >= m_CookTime )
			{
				m_Items[0].m_ItemCount--;
				if( m_Items[0].IsEmpty() ) m_Items[0].Empty();

				m_Items[2].m_ItemDamage = m_CookingItem->m_ItemDamage;
				m_Items[2].m_ItemType = m_CookingItem->m_ItemType;
				m_Items[2].m_ItemCount += m_CookingItem->m_ItemCount;
				delete m_CookingItem;
				m_CookingItem = NULL;

				cWindow * Window = GetWindow();
				if (Window != NULL)
				{
					Window->BroadcastWholeWindow();
				}

				m_TimeCooked -= m_CookTime;
				StartCooking();
			}
			SmeltingProgress = (short)( m_TimeCooked * (180.f / m_CookTime));
			if (SmeltingProgress > 180) SmeltingProgress = 180;
			if (SmeltingProgress < 0)   SmeltingProgress = 0;
		}
	}
	BroadcastProgress(PROGRESSBAR_SMELTING, SmeltingProgress);

	m_TimeBurned += a_Dt;

	cWindow * Window = GetWindow();
	if (m_TimeBurned >= m_BurnTime)
	{
		m_TimeBurned -= m_BurnTime;
		m_BurnTime = 0;
		if (StartCooking() && (Window != NULL))
		{
			Window->BroadcastWholeWindow();
		}
	}
	short Value = 0;
	if (m_BurnTime > 0.f)
	{
		Value = 250 - (short)( m_TimeBurned * (250.f / m_BurnTime));
		if (Value > 250) Value = 250;
		if (Value < 0)   Value = 0;
	}
	BroadcastProgress(PROGRESSBAR_FUEL, Value);
	
	return ((m_CookingItem != NULL) || (m_TimeBurned < m_BurnTime)) && (m_BurnTime > 0.0); // Keep on ticking, if there's more to cook, or if it's cooking
}





bool cFurnaceEntity::StartCooking(void)
{
	cFurnaceRecipe* FR = cRoot::Get()->GetFurnaceRecipe();
	float BurnTime = FR->GetBurnTime( m_Items[1] );
	if( (m_TimeBurned < m_BurnTime) || BurnTime > 0.f ) // burnable material
	{
		const cFurnaceRecipe::Recipe* R = FR->GetRecipeFrom( m_Items[0] );
		if (R != NULL) // cook able ingredient
		{
			if (m_Items[2].IsEqual(*R->Out) || m_Items[2].IsEmpty())
			{
				// good to go
				m_World->FastSetBlock(m_PosX, m_PosY, m_PosZ, E_BLOCK_LIT_FURNACE, m_World->GetBlockMeta(m_PosX, m_PosY, m_PosZ));

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
				return true;
			}
		}
	}
	return false;
}





bool cFurnaceEntity::ContinueCooking(void)
{
	cFurnaceRecipe * FR = cRoot::Get()->GetFurnaceRecipe();
	float BurnTime = FR->GetBurnTime( m_Items[1] );
	if( (m_TimeBurned < m_BurnTime) || BurnTime > 0.f ) // burnable material
	{
		const cFurnaceRecipe::Recipe * R = FR->GetRecipeFrom( m_Items[0] );
		if (R != NULL) // cook able ingredient
		{
			if (m_Items[2].IsEqual(*R->Out) || m_Items[2].IsEmpty())
			{
				// good to go
				if (m_CookingItem == NULL) // Only cook new item if not already cooking
				{
					m_CookingItem = new cItem( *R->Out ); // Resulting item
				}
				return true;
			}
		}
	}
	return false;
}





void cFurnaceEntity::ResetCookTimer()
{
	delete m_CookingItem;
	m_CookingItem = NULL;
	m_TimeCooked = 0.f;
	m_CookTime = 0.f;
}





void cFurnaceEntity::SetSlot(int a_Slot, const cItem & a_Item)
{
	if ((a_Slot < 0) || (a_Slot >= 3))
	{
		ASSERT(!"Furnace: slot number out of range");
		return;
	}
	m_Items[a_Slot] = a_Item;
}





bool cFurnaceEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for( Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr )
	{
		m_Items[ SlotIdx ].FromJson( *itr );
		SlotIdx++;
	}

	// Get currently cooking item
	Json::Value JsonItem = a_Value.get("Cooking", Json::nullValue );
	if( !JsonItem.empty() )
	{
		cItem Item;
		Item.FromJson( JsonItem );
		if( !Item.IsEmpty() )
		{
			m_CookingItem = new cItem( Item );
		}
	}

	m_CookTime   = (float)a_Value.get("CookTime",   0).asDouble();
	m_TimeCooked = (float)a_Value.get("TimeCooked", 0).asDouble();
	m_BurnTime   = (float)a_Value.get("BurnTime",   0).asDouble();
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
		m_Items[ i ].GetJson( Slot );
		AllSlots.append( Slot );
	}
	a_Value["Slots"] = AllSlots;

	// Currently cooking item
	if( m_CookingItem )
	{
		Json::Value JsonItem;
		m_CookingItem->GetJson( JsonItem );
		a_Value["Cooking"] = JsonItem;
	}

	a_Value["CookTime"] = m_CookTime;
	a_Value["TimeCooked"] = m_TimeCooked;
	a_Value["BurnTime"] = m_BurnTime;
	a_Value["TimeBurned"] = m_TimeBurned;
}





void cFurnaceEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
}





void cFurnaceEntity::BroadcastProgress(int a_ProgressbarID, short a_Value)
{
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->BroadcastInventoryProgress(a_ProgressbarID, a_Value);
	}
}




