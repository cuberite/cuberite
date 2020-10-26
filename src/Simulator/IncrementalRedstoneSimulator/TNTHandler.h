
#pragma once





namespace TNTHandler
{
	inline PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);
		return 0;
	}

	inline void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating explodinator the trinitrotoluene (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);
		if (Power != 0)
		{
			a_Chunk.SetBlock(a_Position, E_BLOCK_AIR, 0);
			a_Chunk.GetWorld()->SpawnPrimedTNT(Vector3d(0.5, 0.5, 0.5) + cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos()));  // 80 ticks to boom
		}
	}

	inline void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);
		UNUSED(a_Meta);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
