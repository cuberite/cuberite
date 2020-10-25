
#pragma once

#include "BlockHandler.h"





class cBlockPackedIceHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Only drop self when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType);
		}

		return {};
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 5;
	}
} ;
