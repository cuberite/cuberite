
#pragma once

#include "RedstoneHandler.h"





class cSmallGateHandler final : public cRedstoneHandler
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
		// LOGD("Evaluating gateydory the fence gate/trapdoor (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool ShouldBeOpen = a_PoweringData.PowerLevel != 0;
		const bool IsOpen = (a_Meta & 0x4) == 0x4;

		if (ShouldBeOpen != IsOpen)
		{
			a_Chunk.SetMeta(a_Position, ShouldBeOpen ? (a_Meta | 0x4) : (a_Meta & ~0x04));
		}
	}

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
