
// BlockStandingBanner.h

#pragma once

#include "../BlockInfo.h"
#include "BlockEntity.h"
#include "Mixins/SolidSurfaceUnderneath.h"





class cBlockStandingBannerHandler final :
	public cSolidSurfaceUnderneath<cBlockEntityHandler>
{
	using Super = cSolidSurfaceUnderneath<cBlockEntityHandler>;

public:

	using Super::Super;

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;
