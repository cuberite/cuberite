
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "HangingEntity.h"
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





void cHangingEntity::SpawnOn(cClientHandle & a_ClientHandle)
{
	SetYaw(GetProtocolFacing() * 90);
}





void cHangingEntity::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (m_TickCounter++ >= 100)
	{
		m_TickCounter = 0;

		const auto & Pos = GetPosition();
		const auto SupportBlockPos =
			AddFaceDirection(Pos, ProtocolFaceToBlockFace(m_Facing), true);

		BLOCKTYPE Block;

		const auto ChunkPos = a_Chunk.PositionToWorldPosition(Vector3i());
		const auto RelPos = SupportBlockPos - ChunkPos;

		if (!a_Chunk.UnboundedRelGetBlockType(RelPos, Block))
		{
			return;
		}

		if (!ValidSupportBlock(Block))
		{
			cItems Item;
			GetDrops(Item);

			GetWorld()->SpawnItemPickups(Item, GetPosition(), 5);

			Destroy();
			return;
		}
	}

	UNUSED(a_Dt);
}
