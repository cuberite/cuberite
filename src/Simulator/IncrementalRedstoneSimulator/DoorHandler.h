
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

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}

	inline void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PowerLevel Power)
	{
		// LOGD("Evaluating dori the door (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		NIBBLETYPE TopMeta;
		const bool IsTop = (a_Meta & 0x8) == 0x8;
		const auto TopPosition = IsTop ? a_Position : a_Position.addedY(1);

		// Figure out the metadata of the top half, which stores the previous redstone power state:
		if (IsTop)
		{
			TopMeta = a_Meta;
		}
		else
		{
			if (TopPosition.y == cChunkDef::Height)
			{
				return;
			}

			BLOCKTYPE AboveType;
			a_Chunk.GetBlockTypeMeta(TopPosition, AboveType, TopMeta);
			if (!cBlockDoorHandler::IsDoorBlockType(AboveType))
			{
				return;
			}
		}

		const auto OppositeHalfPosition = a_Position + (IsTop ? OffsetYM : OffsetYP);
		ForEachSourceCallback Callback(a_Chunk, OppositeHalfPosition, a_BlockType);
		ForValidSourcePositions(a_Chunk, OppositeHalfPosition, a_BlockType, a_Meta, Callback);

		// Factor in what the other half is getting:
		Power = std::max(Power, Callback.Power);

		const bool ShouldBeOpen = Power != 0;
		const bool PreviouslyPowered = (TopMeta & 0x2) == 0x2;

		// Allow players to override redstone control
		// don't update if redstone power hasn't changed since we last saw it:
		if (ShouldBeOpen == PreviouslyPowered)
		{
			return;
		}

		// Update the previous redstone power:
		if (ShouldBeOpen)
		{
			a_Chunk.SetMeta(TopPosition, TopMeta | 0x2);
		}
		else
		{
			a_Chunk.SetMeta(TopPosition, TopMeta & ~0x2);
		}

		cChunkInterface ChunkInterface(a_Chunk.GetWorld()->GetChunkMap());
		const auto AbsolutePosition = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());

		// Toggle the door, if it needs to be changed:
		if (ShouldBeOpen != cBlockDoorHandler::IsOpen(ChunkInterface, AbsolutePosition))
		{
			cBlockDoorHandler::SetOpen(ChunkInterface, AbsolutePosition, ShouldBeOpen);
			a_Chunk.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_DOOR_OPEN, AbsolutePosition, 0);
		}
	}
};
