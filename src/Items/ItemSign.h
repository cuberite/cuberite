
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockSignPost.h"
#include "../Blocks/BlockWallSign.h"





class cItemSignHandler :
	public cItemHandler
{
	typedef cItemHandler super;
public:
	cItemSignHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}


	virtual bool OnPlayerPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ
	) override
	{
		// If the regular placement doesn't work, do no further processing:
		if (!super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			return false;
		}

		// After successfully placing the sign, open the sign editor for the player:
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
		a_Player.GetClientHandle()->SendEditSign(a_BlockX, a_BlockY, a_BlockZ);
		return true;
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}


	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (a_BlockFace == BLOCK_FACE_TOP)
		{
			a_BlockMeta = cBlockSignPostHandler::RotationToMetaData(a_Player->GetYaw());
			a_BlockType = E_BLOCK_SIGN_POST;
		}
		else
		{
			a_BlockMeta = cBlockWallSignHandler::DirectionToMetaData(a_BlockFace);
			a_BlockType = E_BLOCK_WALLSIGN;
		}
		return true;
	}
} ;




