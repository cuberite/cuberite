
#pragma once

#include "BlockHandler.h"





class cBlockSeaLanternHandler :
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
			return cItem(E_BLOCK_SEA_LANTERN, 1, 0);
		}

		// Number of crystals to drop, capped at the max amount of 5.
		const auto Drops = std::min(
			static_cast<char>(5),
			GetRandomProvider().RandInt<char>(2, 3 + ToolFortuneLevel(a_Tool))
		);

		return cItem(E_ITEM_PRISMARINE_CRYSTALS, Drops);
	}
} ;
