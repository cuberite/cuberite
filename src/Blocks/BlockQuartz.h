
#pragma once

#include "BlockHandler.h"





class cBlockQuartzHandler final :
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
		auto Meta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage);

		// Pillar block needs additional direction in the metadata:
		if (Meta != E_META_QUARTZ_PILLAR)
		{
			a_BlockMeta = Meta;
			return true;
		}
		a_BlockMeta = BlockFaceToMetaData(a_ClickedBlockFace);
		return true;
	}





	/** Converts the block face of the pillar block's "base" to the block's metadata. */
	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return E_META_QUARTZ_PILLAR;  // Top or bottom
			}

			case BLOCK_FACE_ZP:
			case BLOCK_FACE_ZM:
			{
				return 0x4;  // North or south
			}

			case BLOCK_FACE_XP:
			case BLOCK_FACE_XM:
			{
				return 0x3;  // East or west
			}

			default:
			{
				return E_META_QUARTZ_PILLAR;
			}
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 8;
	}
} ;
