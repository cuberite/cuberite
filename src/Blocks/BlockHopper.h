
// BlockHopper.h

// Declares the cBlockHopperHandler class representing the handler for the Hopper block

#include "Mixins.h"



class cBlockHopperHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		a_Block = Block::Hopper::Hopper(true, a_ClickedBlockFace);
		return true;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
} ;
