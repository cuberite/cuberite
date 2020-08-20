
#pragma once

#include "../../Blocks/BlockTripwireHook.h"





namespace TripwireHookHandler
{
	inline unsigned char GetPowerLevel(const cChunk & a_Chunk, Vector3i a_Position, NIBBLETYPE a_Meta)
	{
		bool FoundActivated = false;
		const auto FaceToGoTowards = cBlockTripwireHookHandler::MetadataToDirection(a_Meta);

		for (int i = 0; i < 40; ++i)  // Tripwires can be connected up to 40 blocks
		{
			BLOCKTYPE Type;
			NIBBLETYPE Meta;

			a_Position = AddFaceDirection(a_Position, FaceToGoTowards);
			if (!a_Chunk.UnboundedRelGetBlock(a_Position, Type, Meta))
			{
				return 0;
			}

			if (Type == E_BLOCK_TRIPWIRE)
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
			else if (Type == E_BLOCK_TRIPWIRE_HOOK)
			{
				if (ReverseBlockFace(cBlockTripwireHookHandler::MetadataToDirection(Meta)) == FaceToGoTowards)
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

	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_BlockType);
		UNUSED(a_QueryBlockType);
		UNUSED(a_QueryPosition);

		return (GetPowerLevel(a_Chunk, a_Position, a_Chunk.GetMeta(a_Position)) == 15) ? 15 : 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating hooky the tripwire hook (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const auto PowerLevel = GetPowerLevel(a_Chunk, a_Position, a_Meta);
		NIBBLETYPE Meta;
		if (PowerLevel == 0)
		{
			Meta = (a_Meta & 0x3);
		}
		else if (PowerLevel == 1)
		{
			// Connected but not activated, AND away the highest bit
			Meta = (a_Meta & 0x7) | 0x4;
		}
		else if (PowerLevel == 15)
		{
			// Connected and activated, set the 3rd and 4th highest bits
			Meta = (a_Meta | 0xC);
		}
		else
		{
			UNREACHABLE("Unexpected tripwire hook power level!");
		}

		if (Meta != a_Meta)
		{
			a_Chunk.SetMeta(a_Position, Meta);
			UpdateAdjustedRelatives(a_Chunk, CurrentlyTicking, a_Position, RelativeAdjacents);
		}
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_Position);
		UNUSED(Callback);
	}
};
