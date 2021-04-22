
#pragma once





namespace RedstoneLampHandler
{
	static bool IsOn(BLOCKTYPE a_BlockType)
	{
		return (a_BlockType == E_BLOCK_REDSTONE_LAMP_ON);
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		return 0;
	}

	static void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating lamp (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		if (Power > 0)
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

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Meta);
		UNUSED(a_BlockType);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
