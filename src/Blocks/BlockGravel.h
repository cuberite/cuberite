
#pragma once

#include "BlockHandler.h"





class cBlockGravelHandler :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// TODO: Handle the Fortune and Silk touch enchantments here
		if (GetRandomProvider().RandBool(0.10))
		{
			return cItem(E_ITEM_FLINT, 1, 0);
		}
		else
		{
			return cItem(E_BLOCK_GRAVEL, 1, 0);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




