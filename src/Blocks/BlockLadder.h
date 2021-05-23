
#pragma once

#include "BlockHandler.h"
#include "BlockInfo.h"
#include "Mixins.h"





class cBlockLadderHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;


	/** Returns true if the ladder will be supported by the block through the given blockface. */
	static bool CanBePlacedOn(const BLOCKTYPE a_BlockType, const eBlockFace a_BlockFace)
	{
		if (
			(a_BlockFace == BLOCK_FACE_NONE) ||
			(a_BlockFace == BLOCK_FACE_BOTTOM) ||
			(a_BlockFace == BLOCK_FACE_TOP)
		)
		{
			return false;
		}

		return cBlockInfo::IsSolid(a_BlockType);
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
		auto Face = MetaDataToBlockFace(a_Meta);
		auto NeighborRelPos = AddFaceDirection(a_Position, Face, true);
		BLOCKTYPE NeighborBlockType;
		a_Chunk.UnboundedRelGetBlockType(NeighborRelPos, NeighborBlockType);
		return CanBePlacedOn(NeighborBlockType, Face);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;




