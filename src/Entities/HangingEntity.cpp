
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "HangingEntity.h"
#include "BlockInfo.h"
#include "Player.h"
#include "Chunk.h"
#include "../ClientHandle.h"





cHangingEntity::cHangingEntity(eEntityType a_EntityType, eBlockFace a_Facing, Vector3d a_Pos) :
	Super(a_EntityType, a_Pos, 0.5f, 0.5f),
	m_Facing(cHangingEntity::BlockFaceToProtocolFace(a_Facing))
{
	SetMaxHealth(1);
	SetHealth(1);
}





bool cHangingEntity::IsValidSupportBlock(const BlockState a_Block)
{
	return cBlockInfo::IsSolid(a_Block.Type()) && (a_Block.Type() != BlockType::Repeater);
}





void cHangingEntity::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);

	Destroy();
}





void cHangingEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	SetYaw(GetProtocolFacing() * 90);
}





void cHangingEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);

	// Check for a valid support block once every 64 ticks (3.2 seconds):
	if ((m_World->GetWorldTickAge() % 64_tick) != 0_tick)
	{
		return;
	}

	BlockState Block;
	const auto SupportPosition = AddFaceDirection(cChunkDef::AbsoluteToRelative(GetPosition()), ProtocolFaceToBlockFace(m_Facing), true);
	if (!a_Chunk.UnboundedRelGetBlock(SupportPosition, Block) || IsValidSupportBlock(Block))
	{
		return;
	}

	// Take environmental damage, intending to self-destruct, with "take damage" handling done by child classes:
	TakeDamage(dtEnvironment, nullptr, static_cast<int>(GetMaxHealth()), 0);
}
