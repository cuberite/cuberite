
#pragma once

#include "BlockEntity.h"
#include "Mixins.h"





#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cBlockJukeboxHandler :
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

#ifdef __clang__
#pragma clang diagnostic pop
#endif
