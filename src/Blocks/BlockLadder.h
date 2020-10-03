
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"





class cBlockLadderHandler final :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x05, 0x03, 0x04> >
{
	using Super = cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x05, 0x03, 0x04>>;

public:

	using Super::Super;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
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

		a_BlockType = m_BlockType;
		a_BlockMeta = BlockFaceToMetaData(a_ClickedBlockFace);
		return true;
	}





	/** Converts the block face of the neighbor to which the ladder is attached to the ladder block's meta. */
	static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_NeighborBlockFace)
	{
		switch (a_NeighborBlockFace)
		{
			case BLOCK_FACE_ZM: return 0x2;
			case BLOCK_FACE_ZP: return 0x3;
			case BLOCK_FACE_XM: return 0x4;
			case BLOCK_FACE_XP: return 0x5;
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return 0x2;
			}
		}
		UNREACHABLE("Unsupported neighbor block face");
	}





	/** Converts the ladder block's meta to the block face of the neighbor to which the ladder is attached. */
	static eBlockFace MetaDataToBlockFace(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0x2: return BLOCK_FACE_ZM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_XM;
			case 0x5: return BLOCK_FACE_XP;
			default:  return BLOCK_FACE_ZM;
		}
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
		auto NeighborBlockFace = MetaDataToBlockFace(a_Chunk.GetMeta(a_RelPos));
		auto LadderAbsPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		return LadderCanBePlacedAt(a_ChunkInterface, LadderAbsPos, NeighborBlockFace);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




