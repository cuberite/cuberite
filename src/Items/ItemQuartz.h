#pragma once

#include "ItemHandler.h"





class cItemQuartzHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	/** Converts the block face of the pillar block's "base" to the block's metadata. */
	static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return E_META_QUARTZ_PILLAR;  // Top or bottom.
			}

			case BLOCK_FACE_ZP:
			case BLOCK_FACE_ZM:
			{
				return 0x4;  // North or south.
			}

			case BLOCK_FACE_XP:
			case BLOCK_FACE_XM:
			{
				return 0x3;  // East or west.
			}
			default: UNREACHABLE("Unsupported block face");
		}
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		const auto Meta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage);

		// Pillar block needs additional direction in the metadata:
		if (Meta == E_META_QUARTZ_PILLAR)
		{
			return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_QUARTZ_BLOCK, BlockFaceToMetaData(a_ClickedBlockFace));
		}

		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_QUARTZ_BLOCK, Meta);
	}
};
