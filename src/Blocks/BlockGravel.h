
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
			return cItem(E_BLOCK_GRAVEL, 1, 0);
		}
		else
		{
			// Denominator of probability from wiki
			double Denominator = 10 - (3 * ToolFortuneLevel(a_Tool));
			// if it went below 0, reset to 1 (only for Fortune > IV)
			Denominator = std::max<double>(Denominator, 1);
			auto & random = GetRandomProvider();
			if (random.RandBool(1 / Denominator))
			{
				return cItem(E_ITEM_FLINT, 1, 0);
			}
			else
			{
				return cItem(E_BLOCK_GRAVEL, 1, 0);
			}
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




