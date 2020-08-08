
#pragma once





class cRedstoneLampHandler final : public cRedstoneHandler
{
	inline static bool IsOn(BLOCKTYPE a_BlockType)
	{
		return (a_BlockType == E_BLOCK_REDSTONE_LAMP_ON);
	}

	virtual unsigned char GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked) const override
	{
		return 0;
	}

	virtual void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, PoweringData a_PoweringData) const override
	{
		// LOGD("Evaluating lamp (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		if (a_PoweringData.PowerLevel > 0)
		{
			if (!IsOn(a_BlockType))
			{
				a_Chunk.FastSetBlock(a_Position, E_BLOCK_REDSTONE_LAMP_ON, 0);
			}
		}
		else
		{
			if (IsOn(a_BlockType))
			{
				a_Chunk.FastSetBlock(a_Position, E_BLOCK_REDSTONE_LAMP_OFF, 0);
			}
		}
	}

	virtual void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, SourceCallback Callback) const override
	{
		UNUSED(a_Chunk);
		UNUSED(a_Meta);
		UNUSED(a_BlockType);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
