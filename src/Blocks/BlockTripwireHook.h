#pragma once

#include "BlockHandler.h"
#include "Mixins.h"





class cBlockTripwireHookHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		const auto RearPosition = AddFaceDirection(a_Position, MetadataToDirection(a_Meta), true);

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




