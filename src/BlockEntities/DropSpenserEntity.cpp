
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





cDropSpenserEntity::cDropSpenserEntity(BlockState a_Block, Vector3i a_Pos, cWorld * a_World):
	Super(a_Block, a_Pos, ContentsWidth, ContentsHeight, a_World),
	m_ShouldDropSpense(false)
{
}





void cDropSpenserEntity::DropSpense(cChunk & a_Chunk)
{
	// Pick one of the occupied slots:
	std::array<int, 9> OccupiedSlots;
	size_t SlotsCnt = 0;
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

	const size_t RandomSlot = GetRandomProvider().RandInt(SlotsCnt - 1);
	const int SpenseSlot = OccupiedSlots[RandomSlot];

	if (cPluginManager::Get()->CallHookDropSpense(*m_World, *this, SpenseSlot))
	{
		// Plugin disagrees with the move
		return;
	}

	// DropSpense the item, using the specialized behavior in the subclasses:
	DropSpenseFromSlot(a_Chunk, SpenseSlot);

	// Broadcast a smoke and click effects:
	auto Self = a_Chunk.GetBlock(GetRelPos());

	eBlockFace Facing = BLOCK_FACE_NONE;
	switch (Self.Type())
	{
		case BlockType::Dropper:   Facing = Block::Dropper::Facing(Self); break;
		case BlockType::Dispenser: Facing = Block::Dispenser::Facing(Self); break;
		default: return;
	}

	int SmokeDir = 0;
	switch (Facing)
	{
		case BLOCK_FACE_YP: SmokeDir = static_cast<int>(SmokeDirection::CENTRE); break;  // YP & YM don't have associated smoke dirs, just do 4 (centre of block)
		case BLOCK_FACE_YM: SmokeDir = static_cast<int>(SmokeDirection::CENTRE); break;
		case BLOCK_FACE_XM: SmokeDir = static_cast<int>(SmokeDirection::EAST); break;
		case BLOCK_FACE_XP: SmokeDir = static_cast<int>(SmokeDirection::WEST); break;
		case BLOCK_FACE_ZM: SmokeDir = static_cast<int>(SmokeDirection::SOUTH); break;
		case BLOCK_FACE_ZP: SmokeDir = static_cast<int>(SmokeDirection::NORTH); break;
		default: return;
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





void cDropSpenserEntity::OnRemoveFromWorld()
{
	const auto Window = GetWindow();
	if (Window != nullptr)
	{
		// Tell window its owner is destroyed:
		Window->OwnerDestroyed();
	}
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
	if (m_Block.Type() == BlockType::Dispenser)
	{
		a_Player->GetStatistics().Custom[CustomStatistic::InspectDispenser]++;
	}
	else  // BlockType::Dropper
	{
		a_Player->GetStatistics().Custom[CustomStatistic::InspectDropper]++;
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
	Vector3i DispCoord(m_Pos);
	auto Self = a_Chunk.GetBlock(GetRelPos());
	eBlockFace Facing = BLOCK_FACE_NONE;
	switch (Self.Type())
	{
		case BlockType::Dropper:   Facing = Block::Dropper::Facing(Self); break;
		case BlockType::Dispenser: Facing = Block::Dispenser::Facing(Self); break;
		default: return;
	}

	DispCoord = AddFaceDirection(DispCoord, Facing);

	cItems Pickups;
	Pickups.push_back(m_Contents.RemoveOneItem(a_SlotNum));

	const int PickupSpeed = GetRandomProvider().RandInt(2, 6);  // At least 2, at most 6
	int PickupSpeedX = 0, PickupSpeedY = 0, PickupSpeedZ = 0;
	switch (Facing)
	{
		case BLOCK_FACE_YP: PickupSpeedY =  PickupSpeed; break;
		case BLOCK_FACE_YM: PickupSpeedY = -PickupSpeed; break;
		case BLOCK_FACE_XM: PickupSpeedX = -PickupSpeed; break;
		case BLOCK_FACE_XP: PickupSpeedX =  PickupSpeed; break;
		case BLOCK_FACE_ZM: PickupSpeedZ = -PickupSpeed; break;
		case BLOCK_FACE_ZP: PickupSpeedZ =  PickupSpeed; break;
		default: UNREACHABLE("Got unknown Block face");
	}

	double MicroX, MicroY, MicroZ;
	MicroX = DispCoord.x + 0.5;
	MicroY = DispCoord.y + 0.4;  // Slightly less than half, to accomodate actual texture hole on DropSpenser
	MicroZ = DispCoord.z + 0.5;


	m_World->SpawnItemPickups(Pickups, MicroX, MicroY, MicroZ, PickupSpeedX, PickupSpeedY, PickupSpeedZ);
}




