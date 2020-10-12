
#pragma once

#include "BlockHandler.h"





/** Handler for blocks that have 3 orientations (hay bale, log), specified by the upper 2 bits in meta.
Handles setting the correct orientation on placement.
Additionally supports the metadata specifying block sub-type in its lower 2 bits. */
class cBlockSidewaysHandler final :
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
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage);
		a_BlockMeta = BlockFaceToMetaData(a_ClickedBlockFace, Meta);
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Reset the orientation part of meta, keep the sub-type part of meta
		return cItem(m_BlockType, 1, a_BlockMeta & 0x03);
	}





	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace, NIBBLETYPE a_Meta)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return a_Meta;  // Top or bottom, just return original
			}

			case BLOCK_FACE_ZP:
			case BLOCK_FACE_ZM:
			{
				return a_Meta | 0x8;  // North or south
			}

			case BLOCK_FACE_XP:
			case BLOCK_FACE_XM:
			{
				return a_Meta | 0x4;  // East or west
			}

			case BLOCK_FACE_NONE:
			{
				ASSERT(!"Unhandled block face!");
				return a_Meta | 0xC;  // No idea, give a special meta
			}
		}
		UNREACHABLE("Unsupported block face");
	}
} ;




