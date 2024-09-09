
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

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Don't allow as a pickup:
		return {};
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 10;
	}
} ;




