
#pragma once

#include "BlockHandler.h"





class cBlockMelonHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		const auto DropNum = FortuneDiscreteRandom(3, 7, ToolFortuneLevel(a_Tool), 9);
		return cItem(E_ITEM_MELON_SLICE, DropNum);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 19;
	}
} ;




