
#pragma once

#include "RedstoneHandler.h"





class cRedstoneBlockHandler final : public cRedstoneHandler
{
public:

	virtual unsigned char GetPowerDeliveredToPosition(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(a_QueryPosition);
		return cIncrementalRedstoneSimulator::IsMechanism(a_QueryBlockType) ? 15 : 0;
	}

	virtual void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating crimson the redstone block (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
	}

	virtual void ForValidSourcePositions(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		UNUSED(Callback);
	}
};
