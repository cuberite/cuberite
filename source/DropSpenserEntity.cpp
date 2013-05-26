
// DropSpenserEntity.cpp

// Declares the cDropSpenserEntity class representing a common ancestor to the cDispenserEntity and cDropperEntity
// The dropper and dispenser only needs to override the DropSpenseFromSlot() function to provide the specific item behavior

#include "Globals.h"
#include "DropSpenserEntity.h"
#include "Player.h"





cDropSpenserEntity::cDropSpenserEntity(BLOCKTYPE a_BlockType, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(a_BlockType, a_BlockX, a_BlockY, a_BlockZ, ContentsWidth, ContentsHeight, a_World),
	m_ShouldDropSpense(false)
{
	SetBlockEntity(this);  // cBlockEntityWindowOwner
}





cDropSpenserEntity::~cDropSpenserEntity()
{
	// Tell window its owner is destroyed
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->OwnerDestroyed();
	}
}





void cDropSpenserEntity::AddDropSpenserDir(int & a_BlockX, int & a_BlockY, int & a_BlockZ, NIBBLETYPE a_Direction)
{
	switch (a_Direction)
	{
		case E_META_DISPENSER_FACING_YM: a_BlockY--; return;
		case E_META_DISPENSER_FACING_YP: a_BlockY++; return;
		case E_META_DISPENSER_FACING_ZM: a_BlockZ--; return;
		case E_META_DISPENSER_FACING_ZP: a_BlockZ++; return;
		case E_META_DISPENSER_FACING_XM: a_BlockX--; return;
		case E_META_DISPENSER_FACING_XP: a_BlockX++; return;
	}
	LOGWARNING("%s: Unhandled direction: %d", __FUNCTION__, a_Direction);
	return;
}





void cDropSpenserEntity::DropSpense(void)
{
	int Disp_X = m_PosX;
	int Disp_Y = m_PosY;
	int Disp_Z = m_PosZ;
	NIBBLETYPE Meta = m_World->GetBlockMeta(m_PosX, m_PosY, m_PosZ);
	
	// Pick one of the occupied slots:
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
		// Nothing in the dropspenser, play the click sound
		m_World->BroadcastSoundEffect("random.click", m_PosX * 8, m_PosY * 8, m_PosZ * 8, 1.0f, 1.2f);
		return;
	}
	
	int RandomSlot = 	m_World->GetTickRandomNumber(SlotsCnt - 1);
	
	// DropSpense the item, using the specialized behavior in the subclasses:
	DropSpenseFromSlot(OccupiedSlots[RandomSlot]);
	
	// Broadcast a smoke and click effects:
	NIBBLETYPE SmokeDir = 0;
	switch (Meta)
	{
		case 2: SmokeDir = 1; break;
		case 3: SmokeDir = 7; break;
		case 4: SmokeDir = 3; break;
		case 5: SmokeDir = 5; break;
	}
	m_World->BroadcastSoundParticleEffect(2000, m_PosX * 8, m_PosY * 8, m_PosZ * 8, SmokeDir);
	m_World->BroadcastSoundEffect("random.click", m_PosX * 8, m_PosY * 8, m_PosZ * 8, 1.0f, 1.0f);
	
	// Update the UI window, if open:
	cWindow * Window = GetWindow();
	if (Window != NULL)
	{
		Window->BroadcastWholeWindow();
	}
}	





void cDropSpenserEntity::Activate(void)
{
	m_ShouldDropSpense = true;
}





bool cDropSpenserEntity::Tick(float a_Dt)
{
	if (!m_ShouldDropSpense)
	{
		return false;
	}
	
	m_ShouldDropSpense = false;
	DropSpense();
	return true;
}





bool cDropSpenserEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	Json::Value AllSlots = a_Value.get("Slots", 0);
	int SlotIdx = 0;
	for (Json::Value::iterator itr = AllSlots.begin(); itr != AllSlots.end(); ++itr)
	{
		cItem Contents;
		Contents.FromJson(*itr);
		m_Contents.SetSlot(SlotIdx, Contents);
		SlotIdx++;
		if (SlotIdx >= m_Contents.GetNumSlots())
		{
			return true;
		}
	}

	return true;
}





void cDropSpenserEntity::SaveToJson(Json::Value & a_Value)
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





void cDropSpenserEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
}





void cDropSpenserEntity::UsedBy(cPlayer * a_Player)
{
	cWindow * Window = GetWindow();
	if (Window == NULL)
	{
		OpenWindow(new cDropSpenserWindow(m_PosX, m_PosY, m_PosZ, this));
		Window = GetWindow();
	}
	
	if (Window != NULL)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(Window);
			Window->SendWholeWindow(*a_Player->GetClientHandle());
		}
	}
}





void cDropSpenserEntity::DropFromSlot(int a_SlotNum)
{
	int DispX = m_PosX;
	int DispY = m_PosY;
	int DispZ = m_PosZ;
	NIBBLETYPE Meta = m_World->GetBlockMeta(m_PosX, m_PosY, m_PosZ);
	AddDropSpenserDir(DispX, DispY, DispZ, Meta);

	cItems Pickups;
	Pickups.push_back(m_Contents.RemoveOneItem(a_SlotNum));
	m_World->SpawnItemPickups(Pickups, DispX, DispY, DispZ);
}




