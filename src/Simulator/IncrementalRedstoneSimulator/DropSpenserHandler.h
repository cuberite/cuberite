
#pragma once

#include "../../BlockEntities/DropSpenserEntity.h"





namespace DropSpenserHandler
{
	static bool IsActivated(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Dropper:   return Dropper::Triggered(a_Block);
			case BlockType::Dispenser: return Dispenser::Triggered(a_Block);
			default: return false;
		}
	}

	static BlockState SetActivationState(BlockState a_Block, bool a_IsOn)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::Dropper:   return Dropper::Dropper(Dropper::Facing(a_Block), a_IsOn);
			case BlockType::Dispenser: return Dispenser::Dispenser(Dispenser::Facing(a_Block), a_IsOn);
			default: return a_Block;
		}
	}

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
		LOGREDSTONE("Evaluating spencer the dropspenser (%d %d %d)", a_Position.x, a_Position.y, a_Position.z);

		const bool IsPoweredNow = (Power > 0);
		const bool WasPoweredPreviously = IsActivated(a_Block);

		if (IsPoweredNow && !WasPoweredPreviously)
		{
			a_Chunk.DoWithBlockEntityAt(a_Position, [](cBlockEntity & a_BlockEntity)
			{
				ASSERT((a_BlockEntity.GetBlockType() == BlockType::Dispenser) || (a_BlockEntity.GetBlockType() == BlockType::Dropper));

				static_cast<cDropSpenserEntity &>(a_BlockEntity).Activate();
				return false;
			});
		}

		// Update the internal dropspenser state if necessary
		if (IsPoweredNow != WasPoweredPreviously)
		{
			a_Chunk.FastSetBlock(a_Position, SetActivationState(a_Block, IsPoweredNow));
		}
	}

	static void ForValidSourcePositions(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block, ForEachSourceCallback & Callback)
	{
		UNUSED(a_Chunk);
		UNUSED(a_Block);

		// Consider indirect power:
		Callback.CheckIndirectPower();

		// Consider normal adjacents:
		InvokeForAdjustedRelatives(Callback, a_Position, RelativeAdjacents);
	}
};
