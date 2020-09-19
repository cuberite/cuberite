
#pragma once

#include "BlockHandler.h"




class cBlockGlowstoneHandler :
	public cBlockHandler
{
public:
	cBlockGlowstoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Drop self only when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(E_BLOCK_GLOWSTONE, 1, 0);
		}
		else
		{
			unsigned int dropNum = GetRandomProvider().RandInt<char>(2, 4 + ToolFortuneLevel(a_Tool));
			// cap the dropnum to the max amount of 4
			dropNum = std::min<unsigned int>(dropNum, 4);
			return cItem(E_ITEM_GLOWSTONE_DUST, dropNum, 0);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 2;
	}
} ;




