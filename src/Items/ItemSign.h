
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../ClientHandle.h"





class cItemSignHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	/** Converts the block face of the neighbor to which the wallsign is attached to the wallsign block's meta. */
	static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_NeighborBlockFace)
	{
		switch (a_NeighborBlockFace)
		{
			case BLOCK_FACE_ZM: return 0x02;
			case BLOCK_FACE_ZP: return 0x03;
			case BLOCK_FACE_XM: return 0x04;
			case BLOCK_FACE_XP: return 0x05;
			case BLOCK_FACE_NONE:
			case BLOCK_FACE_YP:
			case BLOCK_FACE_YM:
			{
				break;
			}
		}
		return 0x02;
	}


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		if (a_ClickedBlockFace == BLOCK_FACE_TOP)
		{
			if (!a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_SIGN_POST, RotationToMetaData(a_Player.GetYaw())))
			{
				return false;
			}
		}
		else if (!a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_WALLSIGN, BlockFaceToMetaData(a_ClickedBlockFace)))
		{
			return false;
		}

		// After successfully placing the sign, open the sign editor for the player:
		a_Player.GetClientHandle()->SendEditSign(a_PlacePosition.x, a_PlacePosition.y, a_PlacePosition.z);
		return true;
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}



	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) override
	{
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_YP:
			{
				a_Block = Block::OakSign::OakSign(RotationToFineFace(a_Player->GetYaw()));
				break;
			}
			case BLOCK_FACE_XM:
			case BLOCK_FACE_XP:
			case BLOCK_FACE_ZM:
			case BLOCK_FACE_ZP:
			{
				a_Block = Block::OakWallSign::OakWallSign(RotationToBlockFace(a_Player->GetYaw()));
				break;
			}
			default: return false;
		}
		return true;
} ;




