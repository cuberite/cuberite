
#pragma once

#include "BlockHandler.h"





class cBlockGlowstoneHandler :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Drop self only when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(E_BLOCK_GLOWSTONE, 1, 0);
		}

		// Number of dust to drop, capped at the max amount of 4.
		const auto Drops = std::min(
			static_cast<char>(4),
			GetRandomProvider().RandInt<char>(2, 4 + ToolFortuneLevel(a_Tool))
		);

		return cItem(E_ITEM_GLOWSTONE_DUST, Drops);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 2;
	}
} ;




