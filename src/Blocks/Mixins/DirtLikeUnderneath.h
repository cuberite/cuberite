
#pragma once

#include "../../Chunk.h"

/** Mixin to ensure the block has a dirt-like block underneath. */
template <class Base>
class cDirtLikeUnderneath :
	public Base
{
	using Super = Base;
public:

	using Super::Super;

	constexpr cDirtLikeUnderneath(BLOCKTYPE a_BlockType):
		Base(a_BlockType)
	{
	}

protected:

	~cDirtLikeUnderneath() = default;

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

		return IsBlockTypeOfDirt(a_Chunk.GetBlock(BelowPos));
	}
};