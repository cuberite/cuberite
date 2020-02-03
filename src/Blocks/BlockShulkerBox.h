
// BlockShulkerBox.h

// Declares the cBlockShulkerBoxHandler class representing the BlockHandler for Shulker Box blocks

#pragma once

#include "../BlockEntities/ShulkerBoxEntity.h"
#include "Mixins.h"





class cBlockShulkerBoxHandler :
	public cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>
{
	using super = cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>;

public:

	cBlockShulkerBoxHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
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

		a_BlockMeta = BlockFaceToMetaData(a_BlockFace, a_BlockMeta);
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		cItems res(cItem(m_BlockType, 1));
		if (a_BlockEntity != nullptr)
		{
			auto be = static_cast<cShulkerBoxEntity *>(a_BlockEntity);
			// TODO keep inventory
			res.AddItemGrid(be->GetContents());
		}
		return res;
	}





	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace, NIBBLETYPE a_Meta)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YM:
			{
				return a_Meta;  // Down
			}
			case BLOCK_FACE_YP:
			{
				return a_Meta | 0x1;  // Up
			}

			case BLOCK_FACE_ZM:
			{
				return a_Meta | 0x2;  // North
			}
			case BLOCK_FACE_ZP:
			{
				return a_Meta | 0x3;  // South
			}

			case BLOCK_FACE_XM:
			{
				return a_Meta | 0x4;  // West
			}
			case BLOCK_FACE_XP:
			{
				return a_Meta | 0x5;  // East
			}

			case BLOCK_FACE_NONE:
			{
				ASSERT(!"Unhandled block face!");
				return a_Meta | 0x6;  // No idea, give a special meta
			}
		}
		UNREACHABLE("Unsupported block face");
	}
} ;




