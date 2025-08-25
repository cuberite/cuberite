
// BlockHopper.h

// Declares the cBlockHopperHandler class representing the handler for the Hopper block

#include "Mixins/Mixins.h"



class cBlockHopperHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
} ;
