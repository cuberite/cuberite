
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
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = (NIBBLETYPE)(a_Player->GetEquippedItem().m_ItemDamage);
		a_BlockMeta = BlockFaceToMetaData(a_BlockFace, Meta);
		return true;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.Add(m_BlockType, 1, a_BlockMeta & 0x3);
	}


	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace, NIBBLETYPE a_WoodMeta)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return a_WoodMeta; // Top or bottom, just return original
			}

			case BLOCK_FACE_ZP:
			case BLOCK_FACE_ZM:
			{
				return a_WoodMeta | 0x8; // North or south
			}

			case BLOCK_FACE_XP:
			case BLOCK_FACE_XM:
			{
				return a_WoodMeta | 0x4; // East or west
			}

			default:
			{
				ASSERT(!"Unhandled block face!");
				return a_WoodMeta | 0xC; // No idea, give a special meta (all sides bark)
			}
		}
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




