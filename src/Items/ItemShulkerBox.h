#pragma once

#include "Items/ItemHandler.h"

#include "BlockInfo.h"

#include "Blocks/BlockShulkerBox.h"
#include "BlockEntities/ShulkerBoxEntity.h"





class cItemShulkerBoxHandler:
public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemShulkerBoxHandler(int a_ItemType) :
	Super(a_ItemType)
	{
	}


	virtual void OnPlayerPlace(
		cPlayer & a_Player,
		const cItem & a_HeldItem,
		Vector3i a_ClickedPosition,
		BLOCKTYPE a_ClickedBlockType,
		NIBBLETYPE a_ClickedBlockMeta,
		eBlockFace a_ClickedBlockFace,
		Vector3i a_CursorPosition
	) const override
	{
		if (a_ClickedBlockFace < 0)
		{
		// Clicked in air
		return;
		}

		if (!cChunkDef::IsValidHeight(a_ClickedPosition))
		{
			// The clicked block is outside the world, ignore this call altogether (#128)
			return;
		}


		// Check if the block ignores build collision (water, grass etc.):
		BLOCKTYPE ClickedBlockType;
		NIBBLETYPE ClickedBlockMeta;cChunkInterface ChunkInterface(a_Player.GetWorld()->GetChunkMap());
		Vector3i PlacePos;


		if (cBlockHandler::For(ClickedBlockType).DoesIgnoreBuildCollision(ChunkInterface, a_ClickedPosition, a_Player, ClickedBlockMeta))
		{
			PlacePos = a_ClickedPosition;
		}
		else
		{
			PlacePos = AddFaceDirection(a_ClickedPosition, a_ClickedBlockFace);
			if (!cChunkDef::IsValidHeight(PlacePos))
			{
				// The block is being placed outside the world, ignore this packet altogether (#128)
				return;
			}

			// Check if the shulker box can overwrite the block at PlacePos:
			BLOCKTYPE PlaceBlock;
			NIBBLETYPE PlaceMeta;
			a_Player.GetWorld()->GetBlockTypeMeta(PlacePos, PlaceBlock, PlaceMeta);
			if (!cBlockHandler::For(PlaceBlock).DoesIgnoreBuildCollision(a_Player.GetWorld(), a_Player.GetEquippedItem(), PlacePos, a_Player, PlaceMeta, a_ClickedBlockFace, true))
			{
				return;
			}
		}


		// Get the meta of the placed shulker box
		auto ShulkerBoxType = static_cast<BLOCKTYPE>(m_ItemType);
		auto ShulkerBoxMeta = static_cast<NIBBLETYPE>(a_ClickedBlockFace);

		// Place the new shulker box:
		if (!a_Player.PlaceBlock(PlacePos, ShulkerBoxType, ShulkerBoxMeta))
		{
			return;
		}


		a_Player.GetWorld()->DoWithBlockEntityAt(PlacePos, [a_Player, &a_HeldItem](cBlockEntity & a_BlockEntity)
		{
			switch (a_BlockEntity.GetBlockType())
			{
				case E_BLOCK_WHITE_SHULKER_BOX:
				case E_BLOCK_ORANGE_SHULKER_BOX:
				case E_BLOCK_MAGENTA_SHULKER_BOX:
				case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:
				case E_BLOCK_YELLOW_SHULKER_BOX:
				case E_BLOCK_LIME_SHULKER_BOX:
				case E_BLOCK_PINK_SHULKER_BOX:
				case E_BLOCK_GRAY_SHULKER_BOX:
				case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:
				case E_BLOCK_CYAN_SHULKER_BOX:
				case E_BLOCK_PURPLE_SHULKER_BOX:
				case E_BLOCK_BLUE_SHULKER_BOX:
				case E_BLOCK_BROWN_SHULKER_BOX:
				case E_BLOCK_GREEN_SHULKER_BOX:
				case E_BLOCK_RED_SHULKER_BOX:
				case E_BLOCK_BLACK_SHULKER_BOX:
				{
					auto & ShulkerBoxEntity = static_cast<cShulkerBoxEntity &>(a_BlockEntity);
					ShulkerBoxEntity.Load(a_HeldItem);
					return true;
				}

			}
		});

		// Remove the "placed" item from inventory:
		if (a_Player.IsGameModeSurvival())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
		return;
	}

	virtual char GetMaxStackSize(void) const override
	{
		return 1;
	}

private:
	cItemShulkerBoxHandler(const cItemShulkerBoxHandler &) = delete;
};
