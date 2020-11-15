
// DropSpenserEntity.cpp

// Declares the cDropSpenserEntity class representing a common ancestor to the cDispenserEntity and cDropperEntity
// The dropper and dispenser only needs to override the DropSpenseFromSlot() function to provide the specific item behavior

#include "Globals.h"
#include "DropSpenserEntity.h"
#include "../Bindings/PluginManager.h"
#include "../EffectID.h"
#include "../Entities/Player.h"
#include "../Chunk.h"
#include "../UI/DropSpenserWindow.h"





cDropSpenserEntity::cDropSpenserEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, ContentsWidth, ContentsHeight, a_World),
	m_ShouldDropSpense(false)
{
}





cDropSpenserEntity::~cDropSpenserEntity()
{
	// Tell window its owner is destroyed
	cWindow * Window = GetWindow();
	if (Window != nullptr)
	{
		Window->OwnerDestroyed();
	}
}





void cDropSpenserEntity::AddDropSpenserDir(Vector3i & a_RelCoord, NIBBLETYPE a_Direction)
{
	switch (a_Direction & E_META_DROPSPENSER_FACING_MASK)
	{
		case E_META_DROPSPENSER_FACING_YM: a_RelCoord.y--; return;
		case E_META_DROPSPENSER_FACING_YP: a_RelCoord.y++; return;
		case E_META_DROPSPENSER_FACING_ZM: a_RelCoord.z--; return;
		case E_META_DROPSPENSER_FACING_ZP: a_RelCoord.z++; return;
		case E_META_DROPSPENSER_FACING_XM: a_RelCoord.x--; return;
		case E_META_DROPSPENSER_FACING_XP: a_RelCoord.x++; return;
	}
	LOGWARNING("%s: Unhandled direction: %d", __FUNCTION__, a_Direction);
}





void cDropSpenserEntity::DropSpense(cChunk & a_Chunk)
{
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
		m_World->BroadcastSoundEffect("block.dispenser.fail", m_Pos, 1.0f, 1.2f);
		return;
	}

	const int RandomSlot = m_World->GetTickRandomNumber(SlotsCnt - 1);
	const int SpenseSlot = OccupiedSlots[RandomSlot];

	if (cPluginManager::Get()->CallHookDropSpense(*m_World, *this, SpenseSlot))
	{
		// Plugin disagrees with the move
		return;
	}

	// DropSpense the item, using the specialized behavior in the subclasses:
	DropSpenseFromSlot(a_Chunk, SpenseSlot);

	// Broadcast a smoke and click effects:
	NIBBLETYPE Meta = a_Chunk.GetMeta(GetRelPos());
	int SmokeDir = 0;
	switch (Meta & E_META_DROPSPENSER_FACING_MASK)
	{
		case E_META_DROPSPENSER_FACING_YP: SmokeDir = static_cast<int>(SmokeDirection::CENTRE); break;  // YP & YM don't have associated smoke dirs, just do 4 (centre of block)
		case E_META_DROPSPENSER_FACING_YM: SmokeDir = static_cast<int>(SmokeDirection::CENTRE); break;
		case E_META_DROPSPENSER_FACING_XM: SmokeDir = static_cast<int>(SmokeDirection::EAST); break;
		case E_META_DROPSPENSER_FACING_XP: SmokeDir = static_cast<int>(SmokeDirection::WEST); break;
		case E_META_DROPSPENSER_FACING_ZM: SmokeDir = static_cast<int>(SmokeDirection::SOUTH); break;
		case E_META_DROPSPENSER_FACING_ZP: SmokeDir = static_cast<int>(SmokeDirection::NORTH); break;
	}
	m_World->BroadcastSoundParticleEffect(EffectID::PARTICLE_SMOKE, GetPos(), SmokeDir);
	m_World->BroadcastSoundEffect("block.dispenser.dispense", m_Pos, 1.0f, 1.0f);
}





void cDropSpenserEntity::Activate(void)
{
	m_ShouldDropSpense = true;
}





void cDropSpenserEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cDropSpenserEntity &>(a_Src);
	m_Contents.CopyFrom(src.m_Contents);
	m_ShouldDropSpense = src.m_ShouldDropSpense;
}





bool cDropSpenserEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	if (!m_ShouldDropSpense)
	{
		return false;
	}

	m_ShouldDropSpense = false;
	DropSpense(a_Chunk);
	return true;
}





void cDropSpenserEntity::SendTo(cClientHandle & a_Client)
{
	// Nothing needs to be sent
	UNUSED(a_Client);
}





bool cDropSpenserEntity::UsedBy(cPlayer * a_Player)
{
	if (m_BlockType == E_BLOCK_DISPENSER)
	{
		a_Player->GetStatManager().AddValue(Statistic::InspectDispenser);
	}
	else  // E_BLOCK_DROPPER
	{
		a_Player->GetStatManager().AddValue(Statistic::InspectDropper);
	}

	cWindow * Window = GetWindow();
	if (Window == nullptr)
	{
		OpenWindow(new cDropSpenserWindow(this));
		Window = GetWindow();
	}

	if (Window != nullptr)
	{
		if (a_Player->GetWindow() != Window)
		{
			a_Player->OpenWindow(*Window);
		}
	}
	return true;
}





void cDropSpenserEntity::DropFromSlot(cChunk & a_Chunk, int a_SlotNum)
{
	Vector3i dispCoord(m_Pos);
	auto Meta = a_Chunk.GetMeta(GetRelPos());
	AddDropSpenserDir(dispCoord, Meta);

	cItems Pickups;
	Pickups.push_back(m_Contents.RemoveOneItem(a_SlotNum));

	const int PickupSpeed = GetRandomProvider().RandInt(2, 6);  // At least 2, at most 6
	int PickupSpeedX = 0, PickupSpeedY = 0, PickupSpeedZ = 0;
	switch (Meta & E_META_DROPSPENSER_FACING_MASK)
	{
		case E_META_DROPSPENSER_FACING_YP: PickupSpeedY =  PickupSpeed; break;
		case E_META_DROPSPENSER_FACING_YM: PickupSpeedY = -PickupSpeed; break;
		case E_META_DROPSPENSER_FACING_XM: PickupSpeedX = -PickupSpeed; break;
		case E_META_DROPSPENSER_FACING_XP: PickupSpeedX =  PickupSpeed; break;
		case E_META_DROPSPENSER_FACING_ZM: PickupSpeedZ = -PickupSpeed; break;
		case E_META_DROPSPENSER_FACING_ZP: PickupSpeedZ =  PickupSpeed; break;
	}

	double MicroX, MicroY, MicroZ;
	MicroX = dispCoord.x + 0.5;
	MicroY = dispCoord.y + 0.4;  // Slightly less than half, to accomodate actual texture hole on DropSpenser
	MicroZ = dispCoord.z + 0.5;


	m_World->SpawnItemPickups(Pickups, MicroX, MicroY, MicroZ, PickupSpeedX, PickupSpeedY, PickupSpeedZ);
}




