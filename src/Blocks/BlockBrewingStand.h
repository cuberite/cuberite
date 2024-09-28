
#pragma once

#include "Mixins.h"





class cBlockBrewingStandHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::BrewingStand);  // We have to drop the item form of a brewing stand
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 6;
	}
} ;




