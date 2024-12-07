
// BlockWallBanner.h

#pragma once

#include "../BlockInfo.h"
#include "BlockEntity.h"





class cBlockWallBannerHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

	virtual cItems ConvertToPickups(const BlockState a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_Self) const override
	{
		Vector3i Offset;
		/*
		switch (GetBlockFace(Self))
		{
			case BLOCK_FACE_ZM: Offset = Vector3i( 0, 0,  1); break;
			case BLOCK_FACE_ZP: Offset = Vector3i( 0, 0, -1); break;
			case BLOCK_FACE_XM: Offset = Vector3i( 1, 0,  0); break;
			case BLOCK_FACE_XP: Offset = Vector3i(-1, 0,  0); break;
			default: return false;
		} */

		// TODO: Implement GetBlockFace

		auto NeighborPos = a_Position + Offset;
		BlockState NeighborType;
		if (!a_Chunk.UnboundedRelGetBlock(NeighborPos, NeighborType))
		{
			// The neighbour is not accessible (unloaded chunk), we'll allow it for now.
			return true;
		}
		return cBlockInfo::IsSolid(NeighborType);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
