
// BlockHopper.h

// Declares the cBlockHopperHandler class representing the handler for the Hopper block

#include "Mixins.h"



class cBlockHopperHandler final :
	public cClearMetaOnDrop<cMetaRotator<cBlockEntityHandler, 0x7, E_META_HOPPER_FACING_ZP, E_META_HOPPER_FACING_XM, E_META_HOPPER_FACING_ZM, E_META_HOPPER_FACING_XP>>
{
	using Super = cClearMetaOnDrop<cMetaRotator<cBlockEntityHandler, 0x7, E_META_HOPPER_FACING_ZP, E_META_HOPPER_FACING_XM, E_META_HOPPER_FACING_ZM, E_META_HOPPER_FACING_XP>>;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;
