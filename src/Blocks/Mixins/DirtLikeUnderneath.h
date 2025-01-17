
#pragma once

#include "../../Chunk.h"
#include "AllTags/BlockTags.h"

/** Mixin to ensure the block has a dirt-like block underneath. */
template <class Base>
class cDirtLikeUnderneath :
	public Base
{
	using Super = Base;
public:

	using Super::Super;

	constexpr cDirtLikeUnderneath(BlockState a_BlockType):
		Base(a_BlockType)
	{
	}

protected:

	~cDirtLikeUnderneath() = default;

protected:

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

		return BlockTags::Dirt(a_Chunk.GetBlock(BelowPos).Type());
	}
};
