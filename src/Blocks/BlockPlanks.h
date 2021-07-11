
#pragma once

#include "BlockHandler.h"





class cBlockPlanksHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		switch (a_Meta)
		{
			case E_META_PLANKS_BIRCH: return 2;
			case E_META_PLANKS_JUNGLE: return 10;
			case E_META_PLANKS_OAK: return 13;
			case E_META_PLANKS_ACACIA: return 15;
			case E_META_PLANKS_DARK_OAK: return 26;
			case E_META_PLANKS_SPRUCE: return 34;
			default:
			{
				ASSERT(!"Unhandled meta in planks handler!");
				return 0;
			}
		}
	}
} ;




