#pragma once

#include "Registries/BlockTypes.h"

struct BlockState
{
	constexpr BlockState(unsigned short ID) :
		ID(ID)
	{
	}

	Block::Type Type() const;
	bool operator == (Block::Type Type) const;
	bool operator == (BlockState Block) const;
	bool operator != (BlockState Block) const;

	unsigned short ID;
};
