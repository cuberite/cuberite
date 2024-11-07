
#pragma once

#include "BlockHandler.h"
#include "Mixins/SolidSurfaceUnderneath.h"




class cBlockRedstoneWireHandler final :
	public cSolidSurfaceUnderneath<cBlockHandler>
{
	using Super = cSolidSurfaceUnderneath<cBlockHandler>;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(E_ITEM_REDSTONE_DUST, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




