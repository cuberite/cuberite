
#pragma once

#include "BlockHandler.h"





class cBlockGravelHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(Item::Gravel);
		}

		// Denominator of probability from wiki, don't let it go below 1.
		const auto Denominator = std::max(10 - 3 * ToolFortuneLevel(a_Tool), 1);
		if (GetRandomProvider().RandBool(1.0 / Denominator))
		{
			return cItem(Item::Flint);
		}

		return cItem(Item::Gravel);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
} ;




