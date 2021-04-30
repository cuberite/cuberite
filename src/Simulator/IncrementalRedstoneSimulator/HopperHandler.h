
#pragma once

#include "../../BlockEntities/HopperEntity.h"





namespace HopperHandler
{
	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);
		return 0;
	}

	static void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating holey the hopper (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool ShouldBeLocked = Power != 0;
		const bool PreviouslyLocked = (a_Meta & 0x8) == 0x8;

		if (ShouldBeLocked == PreviouslyLocked)
		{
			return;
		}

		if (ShouldBeLocked)
		{
			a_Chunk.SetMeta(a_Position, a_Meta | 0x8);
		}
		else
		{
			a_Chunk.SetMeta(a_Position, a_Meta & ~0x8);
		}

		a_Chunk.DoWithBlockEntityAt(a_Position, [ShouldBeLocked](cBlockEntity & a_BlockEntity)
		{
			ASSERT(a_BlockEntity.GetBlockType() == E_BLOCK_HOPPER);

			static_cast<cHopperEntity &>(a_BlockEntity).SetLocked(ShouldBeLocked);
			return false;
		});
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
