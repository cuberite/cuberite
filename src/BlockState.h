#pragma once

#include "Registries/BlockTypes.h"

struct BlockState
{
	constexpr BlockState(uint_least16_t ID) :
		ID(ID)
	{
	}

	BlockType Type() const;
	bool operator == (BlockState Block) const;
	bool operator != (BlockState Block) const;

	uint_least16_t ID;
};
