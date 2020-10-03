#pragma once

#include "BlockHandler.h"
#include "Mixins.h"



#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cBlockPumpkinHandler :
	public cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>
{
	using Super = cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 15;
	}
} ;

#ifdef __clang__
#pragma clang diagnostic pop
#endif




