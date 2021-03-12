
#pragma once





namespace TNTHandler
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
		LOGREDSTONE("Evaluating explodinator the trinitrotoluene (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		if (Power != 0)
		{
			a_Chunk.SetBlock(a_Position, Block::Air::Air());
			a_Chunk.GetWorld()->SpawnPrimedTNT(Vector3d(0.5, 0.5, 0.5) + cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos()));  // 80 ticks to boom
		}
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
