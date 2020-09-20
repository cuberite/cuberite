
#pragma once

#include "BlockHandler.h"





class cBlockMelonHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(E_ITEM_MELON_SLICE, GetRandomProvider().RandInt<char>(3, 7), 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 19;
	}
} ;




