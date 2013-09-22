
#pragma once

#include "BlockHandler.h"





class cBlockWoodHandler : public cBlockHandler
{
public:
	cBlockWoodHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = (NIBBLETYPE)(a_Player->GetEquippedItem().m_ItemDamage);
		a_BlockMeta = BlockFaceToMetaData(a_BlockFace, Meta);
		return true;
	}

	
	inline static NIBBLETYPE BlockFaceToMetaData(char a_BlockFace, NIBBLETYPE a_WoodMeta)
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




