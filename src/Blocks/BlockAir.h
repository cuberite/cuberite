
#pragma once

#include "BlockHandler.h"



class cBlockAirHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

	static inline bool IsBlockAir(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Air:
			case BlockType::CaveAir:
			case BlockType::VoidAir:
				return true;
			default: return false;
		}
	}

private:

	virtual cItems ConvertToPickups(const BlockState a_Block, const cItem * a_Tool) const override
	{
		// Don't drop anything:
		return {};
	}

	virtual bool FullyOccupiesVoxel(const BlockState a_Block) const override
	{
		return false;
	}
};
