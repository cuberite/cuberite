
#pragma once

#include "Mixins.h"




class cBlockFurnaceHandler final :
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

		a_Block = Block::Furnace::Furnace(RotationToBlockFace(a_Player.GetYaw()), false);
		return true;
	}




	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
} ;




