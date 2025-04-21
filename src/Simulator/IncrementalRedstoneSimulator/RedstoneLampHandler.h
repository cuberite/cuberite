
#pragma once





namespace RedstoneLampHandler
{
	static inline bool IsOn(BlockState a_Block)
	{
		return Block::RedstoneLamp::Lit(a_Block);
	}

	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		return 0;
	}

	static void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating lamp (%i %i %i)", a_Position.x, a_Position.y, a_Position.z);

		if (Power > 0)
		{
			if (!IsOn(a_Block))
			{
				a_Chunk.FastSetBlock(a_Position, Block::RedstoneLamp::RedstoneLamp(true));
			}
		}
		else
		{
			if (IsOn(a_Block))
			{
				a_Chunk.FastSetBlock(a_Position, Block::RedstoneLamp::RedstoneLamp(false));
			}
		}
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
