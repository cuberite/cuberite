
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockSign.h"





class cItemSignHandler :
	public cItemHandler
{
public:
	cItemSignHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}


	virtual bool IsPlaceable(void) override
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
		if (a_BlockFace == BLOCK_FACE_TOP)
		{
			a_BlockMeta = cBlockSignHandler::RotationToMetaData(a_Player->GetRot().x);
			a_BlockType = E_BLOCK_SIGN_POST;
		}
		else
		{
			a_BlockMeta = cBlockSignHandler::DirectionToMetaData(a_BlockFace);
			a_BlockType = E_BLOCK_WALLSIGN;
		}
		return true;
	}
} ;




