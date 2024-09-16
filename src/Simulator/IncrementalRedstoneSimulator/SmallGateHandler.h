
#pragma once
#include "Blocks/BlockFenceGate.h"





namespace SmallGateHandler
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
		LOGREDSTONE("Evaluating gateydory the fence gate/trapdoor (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		// Use redstone data rather than block state so players can override redstone control
		const auto Previous = DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, Power);
		const bool IsOpen = (Previous != 0);
		const bool ShouldBeOpen = Power != 0;

		if (ShouldBeOpen != IsOpen)
		{
			cChunkInterface ChunkInterface(a_Chunk.GetWorld()->GetChunkMap());
			auto AbsolutePos = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());
			cBlockFenceGateHandler::Toggle(ChunkInterface, AbsolutePos);
		}
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
