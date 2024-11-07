
#pragma once

#include "BlockHandler.h"
#include "Mixins/DirtLikeUnderneath.h"





class cBlockFlowerHandler final :
	public cDirtLikeUnderneath<cBlockHandler>
{
	using Super = cDirtLikeUnderneath<cBlockHandler>;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		NIBBLETYPE meta = a_BlockMeta & 0x7;
		return cItem(m_BlockType, 1, meta);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




