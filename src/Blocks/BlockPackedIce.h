
#pragma once

#include "BlockHandler.h"





class cBlockPackedIceHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Only drop self when using silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(BlockItemConverter::FromBlock(m_BlockType));
		}

		return {};
	}

	virtual ColourID GetMapBaseColourID() const override
	{
		return 5;
	}
} ;
