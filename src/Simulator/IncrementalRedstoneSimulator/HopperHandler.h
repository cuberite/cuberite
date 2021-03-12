
#pragma once

#include "../../BlockEntities/HopperEntity.h"





namespace HopperHandler
{
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

	static void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating holey the hopper (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

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

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
