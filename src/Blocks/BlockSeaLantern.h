
#pragma once

#include "BlockHandler.h"




class cBlockSeaLanternHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	cBlockSeaLanternHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Drop self only when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(E_BLOCK_SEA_LANTERN, 1, 0);
		}
		else
		{
			unsigned int dropNum = GetRandomProvider().RandInt<char>(2, 3 + ToolFortuneLevel(a_Tool));
			// cap the dropnum to the max amount of 5
			dropNum = std::min<unsigned int>(dropNum, 5);
			// Reset meta to 0
			return cItem(E_ITEM_PRISMARINE_CRYSTALS, dropNum, 0);
		}
	}
} ;




