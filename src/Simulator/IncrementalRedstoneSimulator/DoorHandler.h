
#pragma once

#include "../../Blocks/BlockDoor.h"





namespace DoorHandler
{
	// "Doormammu, I've come to bargain"

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_Block);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlock);
		UNUSED(IsLinked);
		return 0;
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}

	static void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BlockState a_Block, PowerLevel Power)
	{
		LOGREDSTONE("Evaluating dori the door (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool IsTop = cBlockDoorHandler::IsTop(a_Block);
		const auto TopPosition = IsTop ? a_Position : a_Position.addedY(1);
		cChunkInterface ChunkInterface(a_Chunk.GetWorld()->GetChunkMap());

		bool IsOpen;
		// Figure out the metadata of the top half, which stores the previous redstone power state:
		if (IsTop)
		{
			IsOpen = cBlockDoorHandler::IsOpen(ChunkInterface, TopPosition);
		}
		else
		{
			if (TopPosition.y == cChunkDef::Height)
			{
				return;
			}

			auto AboveBlock = a_Chunk.GetBlock(TopPosition);
			if (!cBlockDoorHandler::IsBlockDoor(AboveBlock))
			{
				return;
			}
			IsOpen = cBlockDoorHandler::IsOpen(ChunkInterface, TopPosition);
		}

		const auto OppositeHalfPosition = a_Position + (IsTop ? OffsetYM : OffsetYP);
		ForEachSourceCallback Callback(a_Chunk, OppositeHalfPosition, a_Block);
		ForValidSourcePositions(a_Chunk, OppositeHalfPosition, a_Block, Callback);

		// Factor in what the other half is getting:
		Power = std::max(Power, Callback.Power);

		const bool ShouldBeOpen = Power != 0;
		const auto AbsolutePosition = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());

		// Allow players to override redstone control
		// don't update if redstone power hasn't changed since we last saw it:
		if (ShouldBeOpen == IsOpen)
		{
			return;
		}

		// Update the previous redstone power:
		if (ShouldBeOpen)
		{
			cBlockDoorHandler::SetPowered(ChunkInterface, AbsolutePosition, ShouldBeOpen);
		}

		// Toggle the door, if it needs to be changed:
		if (ShouldBeOpen != cBlockDoorHandler::IsOpen(ChunkInterface, AbsolutePosition))
		{
			cBlockDoorHandler::SetOpen(ChunkInterface, AbsolutePosition, ShouldBeOpen);
			a_Chunk.GetWorld()->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_WOODEN_DOOR_OPEN, AbsolutePosition, 0);
		}
	}
};
