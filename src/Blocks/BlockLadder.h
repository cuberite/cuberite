
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"





class cBlockLadderHandler final :
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
		// Try finding a suitable neighbor block face for the ladder; start with the given one.
		if (!LadderCanBePlacedAt(a_ChunkInterface, a_PlacedBlockPos, a_ClickedBlockFace))
		{
			a_ClickedBlockFace = FindSuitableBlockFace(a_ChunkInterface, a_PlacedBlockPos);
			if (a_ClickedBlockFace == BLOCK_FACE_NONE)
			{
				return false;
			}
		}

		a_Block = Block::Ladder::Ladder(a_ClickedBlockFace);
		return true;
	}





	/** Finds a suitable block face value for the Ladder.
	The returned value is the block face of the neighbor of the specified block to which a ladder at a_LadderPos can be attached.
	Returns BLOCK_FACE_NONE if no valid location is found */
	static eBlockFace FindSuitableBlockFace(cChunkInterface & a_ChunkInterface, const Vector3i a_LadderPos)
	{
		for (int FaceInt = BLOCK_FACE_ZM; FaceInt <= BLOCK_FACE_XP; FaceInt++)
		{
			eBlockFace Face = static_cast<eBlockFace>(FaceInt);
			if (LadderCanBePlacedAt(a_ChunkInterface, a_LadderPos, Face))
			{
				return Face;
			}
		}
		return BLOCK_FACE_NONE;
	}





	/** Returns true if the ladder in the specified position will be supported by its neghbor through the specified neighbor's blockface. */
	static bool LadderCanBePlacedAt(cChunkInterface & a_ChunkInterface, const Vector3i a_LadderPos, eBlockFace a_NeighborBlockFace)
	{
		if (
			(a_NeighborBlockFace == BLOCK_FACE_NONE) ||
			(a_NeighborBlockFace == BLOCK_FACE_BOTTOM) ||
			(a_NeighborBlockFace == BLOCK_FACE_TOP)
		)
		{
			return false;
		}

		auto NeighborPos = AddFaceDirection(a_LadderPos, a_NeighborBlockFace, true);
		return cBlockInfo::IsSolid(a_ChunkInterface.GetBlock(NeighborPos));
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		auto LadderAbsPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		return LadderCanBePlacedAt(a_ChunkInterface, LadderAbsPos, Block::Ladder::Facing(a_Chunk.GetBlock(a_RelPos)));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;




