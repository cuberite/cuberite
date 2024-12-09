
#pragma once

#include "BlockEntity.h"





class cBlockCommandBlockHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

	static inline bool IsBlockCommandBlock(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::CommandBlock:
			case BlockType::ChainCommandBlock:
			case BlockType::RepeatingCommandBlock:
				return true;
			default: return false;
		}
	}

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Don't allow as a pickup:
		return {};
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 10;
	}
} ;




