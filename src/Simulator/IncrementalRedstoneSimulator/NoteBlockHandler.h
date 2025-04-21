
#pragma once

#include "../../BlockEntities/NoteEntity.h"





namespace NoteBlockHandler
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
		LOGREDSTONE("Evaluating sparky the magical note block (%d %d %d) %i", a_Position.x, a_Position.y, a_Position.z, Power);

		const auto Previous = DataForChunk(a_Chunk).ExchangeUpdateOncePowerData(a_Position, Power);
		if ((Previous != 0) || (Power == 0))
		{
			// If we're already powered or received an update of no power, don't make a sound
			return;
		}

		a_Chunk.DoWithBlockEntityAt(a_Position, [](cBlockEntity & a_BlockEntity)
		{
			ASSERT(a_BlockEntity.GetBlockType() == BlockType::NoteBlock);

			static_cast<cNoteEntity &>(a_BlockEntity).MakeSound();
			return false;
		});
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
