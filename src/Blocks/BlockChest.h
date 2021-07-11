
#pragma once

#include "../BlockArea.h"
#include "../Entities/Player.h"
#include "Mixins.h"





class cBlockChestHandler final :
	public cYawRotator<cClearMetaOnDrop<cBlockEntityHandler>, 0x07, 0x03, 0x04, 0x02, 0x05>
{
	using Super = cYawRotator<cClearMetaOnDrop<cBlockEntityHandler>, 0x07, 0x03, 0x04, 0x02, 0x05>;

public:

	using Super::Super;


	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 13;
	}
} ;
