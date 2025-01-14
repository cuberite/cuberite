#pragma once

#include "BlockHandler.h"
#include "Mixins/Mixins.h"





class cBlockTripwireHookHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		const auto RearPosition = AddFaceDirection(a_Position, Block::TripwireHook::Facing(a_Self), true);

		BlockState Other = 0;
		if (!a_Chunk.UnboundedRelGetBlock(RearPosition, Other))
		{
			return false;
		}

		return cBlockInfo::FullyOccupiesVoxel(Other);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
};




