
#pragma once

#include "BlockEntity.h"
#include "Mixins.h"





class cBlockJukeboxHandler final :
	public cClearMetaOnDrop<cBlockEntityHandler>
{
public:

	using cClearMetaOnDrop<cBlockEntityHandler>::cClearMetaOnDrop;

private:

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 10;
	}
} ;
