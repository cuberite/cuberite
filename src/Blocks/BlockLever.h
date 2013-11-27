#pragma once

#include "BlockHandler.h"





class cBlockLeverHandler :
	public cBlockHandler
{
public:
	cBlockLeverHandler(BLOCKTYPE a_BlockType);
	
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override;


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_BLOCK_LEVER, 1, 0));
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
		a_BlockMeta = LeverDirectionToMetaData(a_BlockFace);
		return true;
	}


	inline static NIBBLETYPE LeverDirectionToMetaData(char a_Dir)
	{
		// Determine lever direction:
		switch (a_Dir)
		{
			case BLOCK_FACE_TOP:    return 0x6;
			case BLOCK_FACE_EAST:   return 0x1;
			case BLOCK_FACE_WEST:   return 0x2;
			case BLOCK_FACE_SOUTH:  return 0x3;
			case BLOCK_FACE_NORTH:  return 0x4;
			case BLOCK_FACE_BOTTOM: return 0x0;
			default:                return 0x6;
		}
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




