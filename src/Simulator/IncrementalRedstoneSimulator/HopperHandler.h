
#pragma once

#include "../../BlockEntities/HopperEntity.h"





namespace HopperHandler
{
	inline unsigned char GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);
		return 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData)
	{
		// LOGD("Evaluating holey the hopper (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		auto Previous = DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, a_PoweringData);
		if (Previous.PowerLevel == a_PoweringData.PowerLevel)
		{
			return;
		}

		a_Chunk.DoWithHopperAt(a_Position, [a_PoweringData](cHopperEntity & a_Hopper)
		{
			a_Hopper.SetLocked(a_PoweringData.PowerLevel != 0);
			return false;
		});
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
