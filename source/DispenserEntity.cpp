
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
#include "Simulator/FluidSimulator.h"
#include <json/json.h>





#define AddDispenserDir(x, y, z, dir) \
	switch (dir) \
	{ \
		case 2: (z) --; break; \
		case 3: (z) ++; break; \
		case 4: (x) --; break; \
		case 5: (x) ++; break; \
	}





cDispenserEntity::cDispenserEntity(int a_X, int a_Y, int a_Z, cWorld * a_World)
	: cBlockEntity( E_BLOCK_DISPENSER, a_X, a_Y, a_Z, a_World )
	, m_Items( new cItem[9] )
	, m_CanDispense( 0 )
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
	for( int i = 0; i < 9; i++)
	{
		if( !m_Items[i].IsEmpty() )
		{
			Pickups.push_back(m_Items[i]);
			m_Items[i].Empty();
		}
	}
	m_World->SpawnItemPickups(Pickups, m_PosX, m_PosY, m_PosZ);
}





void cDispenserEntity::Dispense()
{
	int Disp_X = m_PosX;
	int Disp_Y = m_PosY;
	int Disp_Z = m_PosZ;
	NIBBLETYPE Meta = m_World->GetBlockMeta( m_PosX, m_PosY, m_PosZ );
	AddDispenserDir( Disp_X, Disp_Y, Disp_Z, Meta );
	char OccupiedSlots[9];
	char SlotsCnt = 0;
	for( int i = 0; i < 9; i++)
	{
		if( !m_Items[i].IsEmpty() )
		{
			OccupiedSlots[SlotsCnt] = i;
			SlotsCnt++;
		}
	}
	if(SlotsCnt > 0)
	{
		MTRand r1;
		char RandomSlot = r1.randInt() % SlotsCnt;
		cItem Drop = m_Items[OccupiedSlots[RandomSlot]];
		switch( m_Items[OccupiedSlots[RandomSlot]].m_ItemType )
		{
			case E_ITEM_BUCKET:
			{
				BLOCKTYPE DispBlock = m_World->GetBlock( Disp_X, Disp_Y, Disp_Z );
				if( DispBlock == E_BLOCK_STATIONARY_WATER )
				{
					m_World->SetBlock( Disp_X, Disp_Y, Disp_Z, E_BLOCK_AIR, 0 );
					m_Items[OccupiedSlots[RandomSlot]].m_ItemType = E_ITEM_WATER_BUCKET;
				}
				else if( DispBlock == E_BLOCK_STATIONARY_LAVA )
				{
					m_World->SetBlock( Disp_X, Disp_Y, Disp_Z, E_BLOCK_AIR, 0 );
					m_Items[OccupiedSlots[RandomSlot]].m_ItemType = E_ITEM_LAVA_BUCKET;
				}
				else
				{
					cItems Pickups;
					Pickups.push_back(cItem(Drop.m_ItemType, 1, Drop.m_ItemDamage));	
					m_World->SpawnItemPickups(Pickups, Disp_X, Disp_Y, Disp_Z);
					m_Items[OccupiedSlots[RandomSlot]].m_ItemCount--;
				}
				break;
			}
			case E_ITEM_WATER_BUCKET:
			{
				BLOCKTYPE DispBlock = m_World->GetBlock( Disp_X, Disp_Y, Disp_Z );
				if( DispBlock == E_BLOCK_AIR || IsBlockLiquid(DispBlock) || cFluidSimulator::CanWashAway(DispBlock) )
				{
					m_World->SetBlock( Disp_X, Disp_Y, Disp_Z, E_BLOCK_STATIONARY_WATER, 0 );
					m_Items[OccupiedSlots[RandomSlot]].m_ItemType = E_ITEM_BUCKET;
				}
				else
				{
					cItems Pickups;
					Pickups.push_back(cItem(Drop.m_ItemType, 1, Drop.m_ItemDamage));	
					m_World->SpawnItemPickups(Pickups, Disp_X, Disp_Y, Disp_Z);
					m_Items[OccupiedSlots[RandomSlot]].m_ItemCount--;
				}
				break;
			}
			case E_ITEM_LAVA_BUCKET:
			{
				BLOCKTYPE DispBlock = m_World->GetBlock( Disp_X, Disp_Y, Disp_Z );
				if( DispBlock == E_BLOCK_AIR || IsBlockLiquid(DispBlock) || cFluidSimulator::CanWashAway(DispBlock) )
				{
					m_World->SetBlock( Disp_X, Disp_Y, Disp_Z, E_BLOCK_STATIONARY_LAVA, 0 );
					m_Items[OccupiedSlots[RandomSlot]].m_ItemType = E_ITEM_BUCKET;
				}
				else
				{
					cItems Pickups;
					Pickups.push_back(cItem(Drop.m_ItemType, 1, Drop.m_ItemDamage));	
					m_World->SpawnItemPickups(Pickups, Disp_X, Disp_Y, Disp_Z);
					m_Items[OccupiedSlots[RandomSlot]].m_ItemCount--;
				}
				break;
			}
			case E_ITEM_SPAWN_EGG:
			{
				if (m_World->SpawnMob(Disp_X + 0.5, Disp_Y, Disp_Z + 0.5, m_Items[OccupiedSlots[RandomSlot]].m_ItemDamage) >= 0)
				{
					m_Items[OccupiedSlots[RandomSlot]].m_ItemCount--;
				}
				break;
			}
			default:
			{
				cItems Pickups;
				Pickups.push_back(cItem(Drop.m_ItemType, 1, Drop.m_ItemDamage));	
				m_World->SpawnItemPickups(Pickups, Disp_X, Disp_Y, Disp_Z);
				m_Items[OccupiedSlots[RandomSlot]].m_ItemCount--;
				break;
			}
		}
		char SmokeDir;
		switch( Meta )
		{
			case 2: SmokeDir = 1; break;
			case 3: SmokeDir = 7; break;
			case 4: SmokeDir = 3; break;
			case 5: SmokeDir = 5; break;
		}
		m_World->BroadcastSoundParticleEffect(2000, m_PosX * 8, m_PosY * 8, m_PosZ * 8, SmokeDir);
		m_World->BroadcastSoundEffect("random.click", m_PosX * 8, m_PosY * 8, m_PosZ * 8, 1.0f, 1.0f);
		cWindow * Window = GetWindow();
		if ( Window != NULL )
		{
			Window->BroadcastWholeWindow();
		}
	}
	else
	{
		m_World->BroadcastSoundEffect("random.click", m_PosX * 8, m_PosY * 8, m_PosZ * 8, 1.0f, 1.2f);
	}
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





void cDispenserEntity::Activate()
{
	m_CanDispense = 1;
}





bool cDispenserEntity::Tick( float a_Dt )
{
	if(m_CanDispense)
	{
		m_CanDispense = 0;
		Dispense();
	}
	return false;
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




