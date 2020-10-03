
#pragma once

#include "BlockHandler.h"





#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cBlockMelonHandler :
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

#ifdef __clang__
#pragma clang diagnostic pop
#endif




