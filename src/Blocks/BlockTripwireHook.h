#pragma once

#include "BlockHandler.h"
#include "Mixins.h"





class cBlockTripwireHookHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		using namespace Block;
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				a_Block = Block::TripwireHook::TripwireHook(false, a_ClickedBlockFace, false);
				return true;
			}
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return false;
			}
		}
		UNREACHABLE("Unsupported block face");
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		using namespace Block;
		const auto Self = a_Chunk.GetBlock(a_RelPos);
		const auto RearPosition = AddFaceDirection(a_RelPos, TripwireHook::Facing(Self), true);

		BlockState Other = 0;
		if (!a_Chunk.UnboundedRelGetBlock(RearPosition, Other))
		{
			return false;
		}

		return cBlockInfo::FullyOccupiesVoxel(Other);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
};




