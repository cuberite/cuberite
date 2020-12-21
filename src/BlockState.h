#pragma once

#include "Registries/BlockTypes.h"

struct BlockState
{
	constexpr BlockState(uint_least16_t StateID) :
		ID(StateID)
	{
	}

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
};
