
#pragma once

#include "BlockHandler.h"





class cBlockSeaLanternHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Drop self only when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(E_BLOCK_SEA_LANTERN);
		}

		// Number of crystals to drop, capped at the max amount of 5.
		const auto DropNum = FortuneDiscreteRandom(2, 3, ToolFortuneLevel(a_Tool), 5);
		return cItem(E_ITEM_PRISMARINE_CRYSTALS, DropNum);
	}
} ;
