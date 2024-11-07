
#pragma once

#include "../../Chunk.h"
#include "../BlockSlab.h"
#include "../BlockStairs.h"

/** Mixin to ensure the block has a solid surface underneath. */
template <class Base>
class cSolidSurfaceUnderneath :
	public Base
{
	using Super = Base;
public:

	using Super::Super;

	constexpr cSolidSurfaceUnderneath(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{
	}

protected:

	~cSolidSurfaceUnderneath() = default;

protected:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		if (!Super::CanBeAt(a_Chunk, a_Position, a_Meta))
		{
			return false;
		}

		const auto BelowPos = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(BelowPos))
		{
			return false;
		}

		BLOCKTYPE BelowBlock;
		NIBBLETYPE BelowBlockMeta;
		a_Chunk.GetBlockTypeMeta(BelowPos, BelowBlock, BelowBlockMeta);

		if (cBlockInfo::FullyOccupiesVoxel(BelowBlock))
		{
			return true;
		}

		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(BelowBlock))
		{
			return BelowBlockMeta & E_META_WOODEN_SLAB_UPSIDE_DOWN;
		}

		// upside down stairs
		if (cBlockStairsHandler::IsAnyStairType(BelowBlock))
		{
			return BelowBlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN;
		}

		return false;
	}
};