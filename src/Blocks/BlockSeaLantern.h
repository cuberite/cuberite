
#pragma once

#include "BlockHandler.h"





class cBlockSeaLanternHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Drop self only when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(Item::SeaLantern);
		}

		// Number of crystals to drop, capped at the max amount of 5.
		const auto DropNum = FortuneDiscreteRandom(2, 3, ToolFortuneLevel(a_Tool), 5);
		return cItem(Item::PrismarineCrystals, DropNum);
	}
} ;
