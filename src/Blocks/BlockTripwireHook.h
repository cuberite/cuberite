#pragma once

#include "BlockHandler.h"
#include "Mixins.h"





class cBlockTripwireHookHandler final :
	public cMetaRotator<cClearMetaOnDrop<cBlockHandler>, 0x03, 0x02, 0x03, 0x00, 0x01>
{
	using Super = cMetaRotator<cClearMetaOnDrop<cBlockHandler>, 0x03, 0x02, 0x03, 0x00, 0x01>;

public:

	using Super::Super;

	inline static eBlockFace MetadataToDirection(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x03)
		{
			case 0x1: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_XP;
			case 0x2: return BLOCK_FACE_ZM;
			case 0x0: return BLOCK_FACE_ZP;
			default: ASSERT(!"Unhandled tripwire hook metadata!"); return BLOCK_FACE_NONE;
		}
	}

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		const auto RearPosition = AddFaceDirection(a_Position, MetadataToDirection(a_Meta), true);

		BLOCKTYPE NeighborBlockType;
		if (!a_Chunk.UnboundedRelGetBlockType(RearPosition, NeighborBlockType))
		{
			return false;
		}

		return cBlockInfo::FullyOccupiesVoxel(NeighborBlockType);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
};




