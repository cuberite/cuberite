
#pragma once

#include "BlockHandler.h"





class cBlockGlowstoneHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Drop self only when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(Item::Glowstone);
		}

		// Number of dust to drop, capped at the max amount of 4.
		const auto DropNum = FortuneDiscreteRandom(2, 4, ToolFortuneLevel(a_Tool), 4);
		return cItem(Item::Glowstone, DropNum);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 2;
	}
} ;




