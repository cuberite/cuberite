
#pragma once

#include "BlockHandler.h"
#include "BlockInfo.h"
#include "Mixins.h"





class cBlockLadderHandler final :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x05, 0x03, 0x04> >
{
	using Super = cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x05, 0x03, 0x04>>;

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

private:

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





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		auto Face = MetaDataToBlockFace(a_Meta);
		auto NeighborRelPos = AddFaceDirection(a_Position, Face, true);
		BLOCKTYPE NeighborBlockType;
		a_Chunk.UnboundedRelGetBlockType(NeighborRelPos, NeighborBlockType);
		return CanBePlacedOn(NeighborBlockType, Face);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




