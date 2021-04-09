
#pragma once

#include "BlockHandler.h"





class cBlockTripwireHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(Item::String);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
};




