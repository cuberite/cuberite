
#pragma once

#include "Mixins.h"




class cBlockFurnaceHandler final :
	public cYawRotator<cBlockEntityHandler, 0x07, 0x03, 0x04, 0x02, 0x05>
{
	using Super = cYawRotator<cBlockEntityHandler, 0x07, 0x03, 0x04, 0x02, 0x05>;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(E_BLOCK_FURNACE);  // We can't drop a lit furnace
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




