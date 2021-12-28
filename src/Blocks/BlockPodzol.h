
#pragma once

#include "BlockHandler.h"

class cBlockPodzolHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

	using Super::Super;

	cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		if (a_Tool != nullptr)
		{
			if (ToolHasSilkTouch(a_Tool))
			{
				return cItem(Item::Podzol);
			}
			return cItem(Item::Dirt);
		}
		return cItem(Item::Dirt);
	}
};
