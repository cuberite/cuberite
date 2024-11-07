
#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"





class cBlockWallSignHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(E_ITEM_SIGN, 1, 0);
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		auto NeighborPos = a_Position + GetOffsetBehindTheSign(a_Meta);
		BLOCKTYPE NeighborType;
		if (!a_Chunk.UnboundedRelGetBlockType(NeighborPos, NeighborType))
		{
			// The neighbor is not accessible (unloaded chunk), bail out without changing this
			return true;
		}
		return (NeighborType == E_BLOCK_WALLSIGN) || (NeighborType == E_BLOCK_SIGN_POST) || cBlockInfo::IsSolid(NeighborType);
	}





	/** Returns the offset from the sign coords to the block to which the wallsign is attached, based on the wallsign's block meta.
	Asserts / returns a zero vector on wrong meta. */
	static Vector3i GetOffsetBehindTheSign(NIBBLETYPE a_BlockMeta)
	{
		switch (a_BlockMeta)
		{
			case 2: return Vector3i( 0, 0,  1);
			case 3: return Vector3i( 0, 0, -1);
			case 4: return Vector3i( 1, 0,  0);
			case 5: return Vector3i(-1, 0,  0);
		}
		ASSERT(!"Invalid wallsign block meta");
		return Vector3i();
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 13;
	}
} ;




