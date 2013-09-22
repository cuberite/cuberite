#pragma once

#include "BlockHandler.h"





class cBlockButtonHandler :
	public cBlockHandler
{
public:
	cBlockButtonHandler(BLOCKTYPE a_BlockType);
	
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(m_BlockType == E_BLOCK_WOODEN_BUTTON ? E_BLOCK_WOODEN_BUTTON : E_BLOCK_STONE_BUTTON, 1, 0));
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = BlockFaceToMetaData(a_BlockFace);
		return true;
	}


	virtual const char * GetStepSound(void) override
	{
		return m_BlockType == E_BLOCK_WOODEN_BUTTON ? "step.wood" : "step.stone";
	}


	inline static NIBBLETYPE BlockFaceToMetaData(char a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_ZP: { return 0x4; }
			case BLOCK_FACE_ZM: { return 0x3; }
			case BLOCK_FACE_XP: { return 0x2; }
			case BLOCK_FACE_XM: { return 0x1; }
			default:
			{
				ASSERT(!"Unhandled block face!");
				return 0x0; // No idea, give a special meta (button in centre of block)
			}
		}
	}
} ;




