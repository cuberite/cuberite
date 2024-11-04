
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

	constexpr cSolidSurfaceUnderneath(BlockState a_BlockType):
		Base(a_BlockType)
	{
	}

protected:

	~cSolidSurfaceUnderneath() = default;

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_Block) const override
	{
		if (!Super::CanBeAt(a_Chunk, a_Position, a_Block))
		{
			return false;
		}

		const auto BelowPos = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(BelowPos))
		{
			return false;
		}

		BlockState BelowBlock;

		if (cBlockInfo::FullyOccupiesVoxel(BelowBlock))
		{
			return true;
		}

		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(BelowBlock))
		{
			return cBlockSlabHandler::IsSlabUpsideDown(BelowBlock);
		}

		// upside down stairs
		if (cBlockStairsHandler::IsAnyStairType(BelowBlock))
		{
			return cBlockStairsHandler::IsStairsUpsideDown(BelowBlock);
		}

		return false;
	}
};
