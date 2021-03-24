#pragma once

#include <stdint.h>
#include "Registries/BlockTypes.h"
#include "ChunkDef.h"


/** The UINT_LEAST16_MAX value is used to mark the value invalid if needed. */

struct BlockState
{
	constexpr BlockState() : ID(0) {}

	constexpr BlockState(uint_least16_t StateID) :
		ID(StateID),
		ID2(0)
	{
	}

	constexpr BlockState(BlockType a_Type);

	/** Gets the block type of this block state. */
	BlockType Type() const;

	bool operator == (BlockState Block) const
	{
		return ID == Block.ID;
	}

	bool operator != (BlockState Block) const
	{
		return ID != Block.ID;
	}

	/** The state ID of the block state. */
	uint_least16_t ID;

	NEWBLOCKTYPE ID2;
};
