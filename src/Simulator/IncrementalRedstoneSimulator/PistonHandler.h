
#pragma once

#include "../../Blocks/BlockPiston.h"





namespace PistonHandler
{
	static PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, Vector3i a_QueryPosition, BLOCKTYPE a_QueryBlockType, bool IsLinked)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Position);
		UNUSED(a_BlockType);
		UNUSED(a_QueryPosition);
		UNUSED(a_QueryBlockType);
		UNUSED(IsLinked);
		return 0;
	}

	static void Update(cChunk & a_Chunk, cChunk &, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, const PowerLevel Power)
	{
		// LOGD("Evaluating pisty the piston (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool ShouldBeExtended = Power != 0;
		auto & ChunkData = *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());
		if (ShouldBeExtended == cBlockPistonHandler::IsExtended(a_Meta))
		{
			return;
		}

		a_Position = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());

		if (ShouldBeExtended)
		{
			cBlockPistonHandler::ExtendPiston(a_Position, *a_Chunk.GetWorld());
			ChunkData.PistonExtensionWorldTickTime.insert({a_Position, a_Chunk.GetWorld()->GetWorldTickAge()});
		}
		else
		{
			auto Result = ChunkData.PistonExtensionWorldTickTime.find(a_Position);
			bool ShouldPullBlock = true;
			if (Result == ChunkData.PistonExtensionWorldTickTime.end() || Result->second - a_Chunk.GetWorld()->GetWorldTickAge() > 2_tick)
			{
				ShouldPullBlock = false;
			}
			cBlockPistonHandler::RetractPiston(a_Position, *a_Chunk.GetWorld(), ShouldPullBlock);
			ChunkData.PistonExtensionWorldTickTime.erase(a_Position);
		}

		// It is necessary to delay after a signal to prevent an infinite loop (#3168)
		// However, this delay is already present: as a side effect of the implementation of piston animation in Blocks\BlockPiston.cpp
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_BlockType);

		const auto Face = cBlockPistonHandler::MetaDataToDirection(a_Meta);
		const auto FrontOffset = AddFaceDirection(Vector3i(), Face);

		for (const auto & Offset : RelativeAdjacents)
		{
			if (Offset != FrontOffset)
			{
				Callback(a_Position + Offset);
			}
		}

		// Consider indirect power:
		Callback.CheckIndirectPower();
	}
};
