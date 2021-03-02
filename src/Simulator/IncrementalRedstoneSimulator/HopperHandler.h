
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

		const auto Previous = DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, Power);
		if (Previous == Power)
		{
			return;
		}

		a_Chunk.DoWithHopperAt(a_Position, [Power](cHopperEntity & a_Hopper)
		{
			a_Hopper.SetLocked(Power != 0);
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
