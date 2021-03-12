
#pragma once

#include "../../Blocks/BlockPiston.h"





namespace PistonHandler
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
		LOGREDSTONE("Evaluating pisty the piston (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool ShouldBeExtended = Power != 0;
		if (ShouldBeExtended == cBlockPistonHandler::IsExtended(a_Block))
		{
			return;
		}

		a_Position = cChunkDef::RelativeToAbsolute(a_Position, a_Chunk.GetPos());

		if (ShouldBeExtended)
		{
			cBlockPistonHandler::ExtendPiston(a_Position, *a_Chunk.GetWorld());
		}
		else
		{
			cBlockPistonHandler::RetractPiston(a_Position, *a_Chunk.GetWorld());
		}

		// It is necessary to delay after a signal to prevent an infinite loop (#3168)
		// However, this delay is already present: as a side effect of the implementation of piston animation in Blocks\BlockPiston.cpp
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);

		eBlockFace Face;
		switch (a_Block.Type())
		{
			case BlockType::Piston:       Face = Block::Piston::Facing(a_Block); break;
			case BlockType::StickyPiston: Face = Block::StickyPiston::Facing(a_Block); break;
			case BlockType::MovingPiston: Face = Block::MovingPiston::Facing(a_Block); break;
			default: return;
		}
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
