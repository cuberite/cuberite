
#pragma once

#include "BlockHandler.h"





class cBlockSeaLanternHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Reset meta to 0
		// TODO: Handle the Fortune enchantment
		return cItem(E_ITEM_PRISMARINE_CRYSTALS, GetRandomProvider().RandInt<char>(2, 3), 0);
	}
} ;
