
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockSignPost.h"
#include "../Blocks/BlockWallSign.h"
#include "../ClientHandle.h"





class cItemSignHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemSignHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool OnPlayerPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) override
	{
		// Check if placing on something ignoring build collision to edit the correct sign later on:
		auto ClickedBlock = a_World.GetBlock(a_ClickedBlockPos);
		cChunkInterface ChunkInterface(a_World.GetChunkMap());
		bool IsReplacingClickedBlock = cBlockHandler::For(ClickedBlock.Type()).DoesIgnoreBuildCollision(ChunkInterface, a_ClickedBlockPos, a_Player, ClickedBlock);

		// If the regular placement doesn't work, do no further processing:
		if (!Super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_ClickedBlockPos, a_ClickedBlockFace, a_CursorPos))
		{
			return false;
		}

		// Use IsReplacingClickedBlock to make sure we will edit the right sign:
		auto SignPos = IsReplacingClickedBlock ? a_ClickedBlockPos : AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);

		// After successfully placing the sign, open the sign editor for the player:
		a_Player.GetClientHandle()->SendEditSign(SignPos.x, SignPos.y, SignPos.z);
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
	}
} ;




