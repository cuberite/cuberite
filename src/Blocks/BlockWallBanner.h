
// BlockWallBanner.h

#pragma once

#include "../BlockInfo.h"
#include "BlockEntity.h"





class cBlockWallBannerHandler final : public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

  public:
	using Super::Super;

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		Vector3i Offset;

		switch (a_Meta)
		{
			case BLOCK_FACE_ZM: Offset = Vector3i(0, 0, 1); break;
			case BLOCK_FACE_ZP: Offset = Vector3i(0, 0, -1); break;
			case BLOCK_FACE_XM: Offset = Vector3i(1, 0, 0); break;
			case BLOCK_FACE_XP: Offset = Vector3i(-1, 0, 0); break;
			default:            return false;
		}

		auto NeighborPos = a_Position + Offset;
		BLOCKTYPE NeighborType;
		if (!a_Chunk.UnboundedRelGetBlockType(NeighborPos, NeighborType))
		{
			// The neighbour is not accessible (unloaded chunk), we'll allow it for now.
			return true;
		}
		return cBlockInfo::IsSolid(NeighborType);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
};
