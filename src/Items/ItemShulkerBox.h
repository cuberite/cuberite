
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"
#include "../Blocks/BlockShulkerBox.h"
#include "../BlockEntities/ShulkerBoxEntity.h"





class cItemShulkerBoxHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemShulkerBoxHandler(int a_ItemType) :
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
		if (a_ClickedBlockFace < 0)
		{
			// Clicked in air
			return false;
		}

		if (!cChunkDef::IsValidHeight(a_ClickedBlockPos.y))
		{
			// The clicked block is outside the world, ignore this call altogether (#128)
			return false;
		}


		// Check if the block ignores build collision (water, grass etc.):
		BLOCKTYPE ClickedBlockType;
		NIBBLETYPE ClickedBlockMeta;
		a_World.GetBlockTypeMeta(a_ClickedBlockPos, ClickedBlockType, ClickedBlockMeta);
		cChunkInterface ChunkInterface(a_World.GetChunkMap());
		Vector3i PlacePos;


		if (cBlockHandler::For(ClickedBlockType).DoesIgnoreBuildCollision(ChunkInterface, a_ClickedBlockPos, a_Player, ClickedBlockMeta))
		{
			PlacePos = a_ClickedBlockPos;
		}
		else
		{
			PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
			if (!cChunkDef::IsValidHeight(PlacePos.y))
			{
				// The block is being placed outside the world, ignore this packet altogether (#128)
				return false;
			}

			// Check if the shulker box can overwrite the block at PlacePos:
			BLOCKTYPE PlaceBlock;
			NIBBLETYPE PlaceMeta;
			a_World.GetBlockTypeMeta(PlacePos, PlaceBlock, PlaceMeta);
			if (!cBlockHandler::For(PlaceBlock).DoesIgnoreBuildCollision(ChunkInterface, PlacePos, a_Player, PlaceMeta))
			{
				return false;
			}
		}


		// Get the meta of the placed shulker box
		BLOCKTYPE ShulkerBoxType = static_cast<BLOCKTYPE>(m_ItemType);

		// Place the new shulker box:
		if (!a_Player.PlaceBlock(PlacePos.x, PlacePos.y, PlacePos.z, ShulkerBoxType, (int)a_ClickedBlockFace))
		{
			return false;
		}


		a_World.DoWithShulkerBoxAt(PlacePos.x, PlacePos.y, PlacePos.z,
			[a_EquippedItem] (cShulkerBoxEntity & a_ShulkerBox) -> bool
			{
				a_ShulkerBox.Load(a_EquippedItem);
				return true;
			}
		);

		// Remove the "placed" item from inventory:
		if (a_Player.IsGameModeSurvival())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
		return true;
	}

	virtual char GetMaxStackSize(void) override
	{
		return 1;
	}

private:
	cItemShulkerBoxHandler(const cItemShulkerBoxHandler &) = delete;
};




