
#pragma once





namespace RedstoneBlockHandler
{
	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, Vector3i a_QueryPosition, BlockState a_QueryBlock, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_Block);
		UNUSED(a_QueryPosition);
		UNUSED(IsLinked);
		return IsLinked ? 0 : 15;
	}

	static void Update(cChunk & a_Chunk, cChunk & CurrentlyTicking, Vector3i a_Position, BlockState a_Block, const PowerLevel Power)
	{
		LOGREDSTONE("Evaluating crimson the redstone block (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_Block);
		UNUSED(Callback);
	}
};
