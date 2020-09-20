
#pragma once

#include "BlockHandler.h"





class cBlockGlowstoneHandler :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Drop self only when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(E_BLOCK_GLOWSTONE, 1, 0);
		}
		else
		{
			// TODO: Handle the Fortune enchantment here
			return cItem(E_ITEM_GLOWSTONE_DUST, GetRandomProvider().RandInt<char>(2, 4), 0);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 2;
	}
} ;




