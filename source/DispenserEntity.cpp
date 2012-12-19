
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "DispenserEntity.h"
#include "BlockID.h"
#include "Item.h"
#include "UI/Window.h"
#include "Player.h"
#include "World.h"
#include "ClientHandle.h"
#include "Server.h"
#include "Pickup.h"
#include "Root.h"
#include <json/json.h>





cDispenserEntity::cDispenserEntity(int a_X, int a_Y, int a_Z, cWorld * a_World)
	: cBlockEntity( E_BLOCK_DISPENSER, a_X, a_Y, a_Z, a_World )
	, m_Items( new cItem[9] )
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





cDispenserEntity::~cDispenserEntity()
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





void cDispenserEntity::Destroy()
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





void cDispenserEntity::UsedBy(cPlayer * a_Player)
{
	if (GetWindow() == NULL)
	{
		OpenWindow(new cDispenserWindow(m_PosX, m_PosY, m_PosZ, this));
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





bool cDispenserEntity::Tick( float a_Dt )
{
	return true;
}





void cDispenserEntity::SetSlot(int a_Slot, const cItem & a_Item)
{
	if ((a_Slot < 0) || (a_Slot >= 9))
	{
		ASSERT(!"Dispenser: slot number out of range");
		return;
	}
	m_Items[a_Slot] = a_Item;
}





#define READ(File, Var) \
	if (File.Read(&Var, sizeof(Var)) != sizeof(Var)) \
	{ \
		LOGERROR("ERROR READING cDispenserEntity %s FROM FILE (line %d)", #Var, __LINE__); \
		return false; \
	}

bool cDispenserEntity::LoadFromFile(cFile & f)
{
	READ(f, m_PosX);
	READ(f, m_PosY);
	READ(f, m_PosZ);

	unsigned int NumSlots = 0;
	READ(f, NumSlots);
	m_Items = new cItem[ NumSlots ];
	for(unsigned int i = 0; i < NumSlots; i++)
	{
		cItem & Item = m_Items[i];
		READ(f, Item.m_ItemID);
		READ(f, Item.m_ItemCount);
		READ(f, Item.m_ItemHealth);
	}

	return true;
}





bool cDispenserEntity::LoadFromJson( const Json::Value& a_Value )
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

	return true;
}





void cDispenserEntity::SaveToJson( Json::Value& a_Value )
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
}





void cDispenserEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
}




