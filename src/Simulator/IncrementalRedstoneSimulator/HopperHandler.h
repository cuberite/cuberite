
#pragma once

#include "RedstoneHandler.h"
#include "../../BlockEntities/HopperEntity.h"





class cHopperHandler final : public cRedstoneHandler
{
public:

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		return 0;
	}

	virtual void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
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

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
