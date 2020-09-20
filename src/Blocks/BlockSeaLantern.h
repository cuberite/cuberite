
#pragma once

#include "BlockHandler.h"




class cBlockSeaLanternHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Drop self only when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(E_BLOCK_SEA_LANTERN, 1, 0);
		}
		else
		{
			unsigned int DropNum = GetRandomProvider().RandInt<char>(2, 3 + ToolFortuneLevel(a_Tool));
			// cap the dropnum to the max amount of 5
			DropNum = std::min<unsigned int>(DropNum, 5);
			// Reset meta to 0
			return cItem(E_ITEM_PRISMARINE_CRYSTALS, DropNum, 0);
		}
	}
} ;
