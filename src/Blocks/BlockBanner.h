
// BlockBanner.h

#pragma once

#include "../BlockInfo.h"

class cBlockBannerHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Drops handled by the block entity:
		return {};
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (!cChunkDef::IsValidRelPos(a_RelPos))
		{
			return false;
		}
		BLOCKTYPE Type = a_Chunk.GetBlock(a_RelPos.addedY(-1));
		return (cBlockInfo::IsSolid(Type));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;
