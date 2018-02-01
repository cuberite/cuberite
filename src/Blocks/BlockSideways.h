
#pragma once

#include "BlockHandler.h"





class cBlockSidewaysHandler : public cBlockHandler
{
public:
	cBlockSidewaysHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage);
		a_BlockMeta = BlockFaceToMetaData(a_BlockFace, Meta);
		return true;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.Add(m_BlockType, 1, a_BlockMeta & 0x3);  // Reset meta
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




