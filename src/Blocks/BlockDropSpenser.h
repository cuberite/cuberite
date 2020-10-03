
// BlockDropSpenser.h

// Declares the cBlockDropSpenserHandler class representing the BlockHandler for Dropper and Dispenser blocks

#pragma once

#include "Mixins.h"





class cBlockDropSpenserHandler final :
	public cPitchYawRotator<cClearMetaOnDrop<cBlockEntityHandler>, 0x07, 0x03, 0x04, 0x02, 0x05, 0x01, 0x00>
{
	using Super = cPitchYawRotator<cClearMetaOnDrop<cBlockEntityHandler>, 0x07, 0x03, 0x04, 0x02, 0x05, 0x01, 0x00>;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;
