
#pragma once

#include "Mixins.h"





class cBlockBrewingStandHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(E_ITEM_BREWING_STAND);  // We have to drop the item form of a brewing stand
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 6;
	}
} ;




