
// BlockStandingBanner.h

#pragma once

#include "../BlockInfo.h"
#include "BlockEntity.h"





class cBlockStandingBannerHandler final :
	public cBlockEntityHandler
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
		const auto BelowPos = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(BelowPos))
		{
			return false;
		}

		return cBlockInfo::IsSolid(a_Chunk.GetBlock(BelowPos));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;
