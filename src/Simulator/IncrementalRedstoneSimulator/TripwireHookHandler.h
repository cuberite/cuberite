
#pragma once

#include "../../Blocks/BlockTripwireHook.h"





namespace TripwireHookHandler
{
	const int MaxTripwireLength = 40;
	static unsigned char GetPowerLevel(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
	{
		bool FoundActivated = false;
		const auto FaceToGoTowards = Block::TripwireHook::Facing(a_Block);

		for (int i = 0; i < MaxTripwireLength; ++i)  // Tripwires can be connected up to 40 blocks
		{
			BlockState Other = 0;

			a_Position = AddFaceDirection(a_Position, FaceToGoTowards);
			if (!a_Chunk.UnboundedRelGetBlock(a_Position, Other))
			{
				return 0;
			}

			if (Other.Type() == BlockType::Tripwire)
			{
				if (FoundActivated)
				{
					continue;
				}

				if (
					!a_Chunk.ForEachEntityInBox(
						cBoundingBox(Vector3d(0.5, 0, 0.5) + cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos()), 0.5, 0.5),
						[](cEntity &) { return true; }
					)
				)
				{
					FoundActivated = true;
				}

				continue;
			}
			else if (Other.Type() == BlockType::TripwireHook)
			{
				if (ReverseBlockFace(Block::TripwireHook::Facing(Other)) == FaceToGoTowards)
				{
					// Other hook facing in opposite direction - circuit completed!
					return FoundActivated ? 15 : 1;
				}
			}

			// Tripwire hook not connected at all
			return 0;
		}

		return 0;
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		UNUSED(a_Block);
		UNUSED(a_QueryBlock);
		UNUSED(a_QueryPosition);

		return (GetPowerLevel(a_Chunk, a_Position, a_Block) == 15 ? 15 : 0);
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating hooky the tripwire hook (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const auto PowerLevel = GetPowerLevel(a_Chunk, a_Position, a_Block);
		bool IsAttached = false;
		bool IsPowered = false;
		if (PowerLevel == 1)
		{
			IsAttached = true;
		}
		else if (PowerLevel == 15)
		{
			IsAttached = true;
			IsPowered = true;
		}
		else if (PowerLevel != 0)
		{
			UNREACHABLE("Unexpected tripwire hook power level!");
		}

		a_Chunk.FastSetBlock(a_Position, Block::TripwireHook::TripwireHook(IsAttached, Block::TripwireHook::Facing(a_Block), IsPowered));
		UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);
		UNUSED(a_Position);
		UNUSED(Callback);
	}
};
