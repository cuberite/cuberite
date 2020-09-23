
#pragma once

#include "BlockHandler.h"





class cBlockGravelHandler :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(E_BLOCK_GRAVEL);
		}

		// Denominator of probability from wiki, don't let it go below 1
		double Denominator = std::max(10 - (3 * ToolFortuneLevel(a_Tool)), 1);
		auto & Random = GetRandomProvider();
		if (Random.RandBool(1 / Denominator))
		{
			return cItem(E_ITEM_FLINT);
		}

		return cItem(E_BLOCK_GRAVEL);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




