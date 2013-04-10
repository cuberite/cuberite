
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





cDispenserEntity::cDispenserEntity(int a_X, int a_Y, int a_Z, cWorld * a_World) :
	cBlockEntity(E_BLOCK_DISPENSER, a_X, a_Y, a_Z, a_World),
	m_Contents(3, 3),
	m_ShouldDispense(false)
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





cDispenserEntity::~cDispenserEntity()
{
	// Tell window its owner is destroyed
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->OwnerDestroyed();
	}
}





void cDispenserEntity::Destroy(void)
{
	// Drop items
	cItems Pickups;
	m_Contents.CopyToItems(Pickups);
	m_Contents.Clear();
	m_World->SpawnItemPickups(Pickups, m_PosX, m_PosY, m_PosZ);
}





void cDispenserEntity::Dispense(void)
{
	int Disp_X = m_PosX;
	int Disp_Y = m_PosY;
	int Disp_Z = m_PosZ;
	NIBBLETYPE Meta = m_World->GetBlockMeta(m_PosX, m_PosY, m_PosZ);
	AddDispenserDir(Disp_X, Disp_Y, Disp_Z, Meta);
	int OccupiedSlots[9];
	int SlotsCnt = 0;
	for (int i = m_Contents.GetNumSlots() - 1; i >= 0; i--)
	{
		if (!m_Contents.GetSlot(i).IsEmpty())
		{
			OccupiedSlots[SlotsCnt] = i;
			SlotsCnt++;
		}
	}  // for i - m_Contents[]
	
	if (SlotsCnt == 0)
	{
		// Nothing in the dispenser, play the click sound
		m_World->BroadcastSoundEffect("random.click", m_PosX * 8, m_PosY * 8, m_PosZ * 8, 1.0f, 1.2f);
		return;
	}
	
	// Pick an item to dispense:
	MTRand r1;
	int RandomSlot = r1.randInt(SlotsCnt);
	cItem & Drop = m_Contents.GetSlot(OccupiedSlots[RandomSlot]);
	
	// Dispense the item:
	switch (Drop.m_ItemType)
	{
		case E_ITEM_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock(Disp_X, Disp_Y, Disp_Z);
			if (DispBlock == E_BLOCK_STATIONARY_WATER)
			{
				m_World->SetBlock(Disp_X, Disp_Y, Disp_Z, E_BLOCK_AIR, 0);
				Drop.m_ItemType = E_ITEM_WATER_BUCKET;  // TODO: Duplication glitch - bucket stacking allows you to duplicate water
			}
			else if (DispBlock == E_BLOCK_STATIONARY_LAVA)
			{
				m_World->SetBlock(Disp_X, Disp_Y, Disp_Z, E_BLOCK_AIR, 0);
				Drop.m_ItemType = E_ITEM_LAVA_BUCKET;  // TODO: Duplication glitch - bucket stacking allows you to duplicate lava
			}
			else
			{
				cItems Pickups;
				Pickups.push_back(Drop.CopyOne());
				m_World->SpawnItemPickups(Pickups, Disp_X, Disp_Y, Disp_Z);
				Drop.m_ItemCount--;
			}
			break;
		}
		
		case E_ITEM_WATER_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock(Disp_X, Disp_Y, Disp_Z);
			if ((DispBlock == E_BLOCK_AIR) || IsBlockLiquid(DispBlock) || cFluidSimulator::CanWashAway(DispBlock))
			{
				m_World->SetBlock(Disp_X, Disp_Y, Disp_Z, E_BLOCK_STATIONARY_WATER, 0);
				Drop.m_ItemType = E_ITEM_BUCKET;
			}
			else
			{
				cItems Pickups;
				Pickups.push_back(Drop.CopyOne());
				m_World->SpawnItemPickups(Pickups, Disp_X, Disp_Y, Disp_Z);
				Drop.m_ItemCount--;
			}
			break;
		}
		
		case E_ITEM_LAVA_BUCKET:
		{
			BLOCKTYPE DispBlock = m_World->GetBlock( Disp_X, Disp_Y, Disp_Z );
			if ((DispBlock == E_BLOCK_AIR) || IsBlockLiquid(DispBlock) || cFluidSimulator::CanWashAway(DispBlock))
			{
				m_World->SetBlock(Disp_X, Disp_Y, Disp_Z, E_BLOCK_STATIONARY_LAVA, 0);
				Drop.m_ItemType = E_ITEM_BUCKET;
			}
			else
			{
				cItems Pickups;
				Pickups.push_back(Drop.CopyOne());
				m_World->SpawnItemPickups(Pickups, Disp_X, Disp_Y, Disp_Z);
				Drop.m_ItemCount--;
			}
			break;
		}
		
		case E_ITEM_SPAWN_EGG:
		{
			if (m_World->SpawnMob(Disp_X + 0.5, Disp_Y, Disp_Z + 0.5, Drop.m_ItemDamage) >= 0)
			{
				Drop.m_ItemCount--;
			}
			break;
		}
		
		default:
		{
			cItems Pickups;
			Pickups.push_back(Drop.CopyOne());
			m_World->SpawnItemPickups(Pickups, Disp_X, Disp_Y, Disp_Z);
			Drop.m_ItemCount--;
			break;
		}
	}  // switch (ItemType)
	
	char SmokeDir = 0;
	switch (Meta)
	{
		case 2: SmokeDir = 1; break;
		case 3: SmokeDir = 7; break;
		case 4: SmokeDir = 3; break;
		case 5: SmokeDir = 5; break;
	}
	m_World->BroadcastSoundParticleEffect(2000, m_PosX * 8, m_PosY * 8, m_PosZ * 8, SmokeDir);
	m_World->BroadcastSoundEffect("random.click", m_PosX * 8, m_PosY * 8, m_PosZ * 8, 1.0f, 1.0f);
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->BroadcastWholeWindow();
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





void cDispenserEntity::Activate(void)
{
	m_ShouldDispense = true;
}





bool cDispenserEntity::Tick(float a_Dt)
{
	if (m_ShouldDispense)
	{
		m_ShouldDispense = false;
		Dispense();
	}
	return false;
}





bool cDispenserEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for (Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr)
	{
		m_Contents.GetSlot(SlotIdx).FromJson(*itr);
		SlotIdx++;
	}

	return true;
}





void cDispenserEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;

	Json::Value AllSlots;
	int NumSlots = m_Contents.GetNumSlots();
	for (int i = 0; i < NumSlots; i++)
	{
		Json::Value Slot;
		m_Contents.GetSlot(i).GetJson(Slot);
		AllSlots.append(Slot);
	}
	a_Value["Slots"] = AllSlots;
}





void cDispenserEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
}




