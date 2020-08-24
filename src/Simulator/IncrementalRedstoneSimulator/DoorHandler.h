
#pragma once

#include "../../Blocks/BlockDoor.h"





namespace DoorHandler
{
	// "Doormammu, I've come to bargain"

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);
		return 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PowerLevel Power)
	{
		// LOGD("Evaluating dori the door (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		if ((a_Meta & 0x8) == 0x8)
		{
			// We're treating the bottom half as the source of truth, so ignore updates to the top:
			return;
		}

		const auto TopPosition = a_Position + OffsetYP;
		ForEachSourceCallback Callback(a_Chunk, TopPosition, a_BlockType);
		RedstoneHandler::ForValidSourcePositions(a_Chunk, TopPosition, a_BlockType, a_Meta, Callback);

		// Factor in what the upper half is getting:
		Power = std::max(Power, Callback.Power);

		cChunkInterface ChunkInterface(a_Chunk.GetWorld()->GetChunkMap());
		const bool ShouldBeOpen = Power != 0;
		const auto AbsolutePosition = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());
		const bool IsOpen = cBlockDoorHandler::IsOpen(ChunkInterface, AbsolutePosition);

		if (ShouldBeOpen != IsOpen)
		{
			cBlockDoorHandler::SetOpen(ChunkInterface, AbsolutePosition, ShouldBeOpen);
			a_Chunk.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_DOOR_OPEN, AbsolutePosition, 0);
		}
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
