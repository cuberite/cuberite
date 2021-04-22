
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"
#include "../Blocks/BlockChest.h"





class cItemChestHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemChestHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}


	/** We need an OnPlayerPlace override because we're processing neighbor chests and changing their metas,
	the parent class cannot do that. */
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

			// Check if the chest can overwrite the block at PlacePos:
			BLOCKTYPE PlaceBlock;
			NIBBLETYPE PlaceMeta;
			a_World.GetBlockTypeMeta(PlacePos, PlaceBlock, PlaceMeta);
			if (!cBlockHandler::For(PlaceBlock).DoesIgnoreBuildCollision(ChunkInterface, PlacePos, a_Player, PlaceMeta))
			{
				return false;
			}
		}

		// Check that there is at most one single neighbor of the same chest type:
		static const Vector3i CrossCoords[] =
		{
			{-1, 0,  0},
			{ 0, 0, -1},
			{ 1, 0,  0},
			{ 0, 0,  1},
		};
		int NeighborIdx = -1;
		for (size_t i = 0; i < ARRAYCOUNT(CrossCoords); i++)
		{
			auto NeighborPos = PlacePos + CrossCoords[i];
			if (a_World.GetBlock(NeighborPos) != m_ItemType)
			{
				continue;
			}
			if (NeighborIdx >= 0)
			{
				// Can't place here, there are already two neighbors, this would form a 3-block chest
				return false;
			}
			NeighborIdx = static_cast<int>(i);

			// Check that this neighbor is a single chest:
			for (size_t j = 0; j < ARRAYCOUNT(CrossCoords); j++)
			{
				if (a_World.GetBlock(NeighborPos + CrossCoords[j]) == m_ItemType)
				{
					// Trying to place next to a dblchest
					return false;
				}
			}  // for j
		}  // for i

		// Get the meta of the placed chest; take existing neighbors into account:
		BLOCKTYPE ChestBlockType = static_cast<BLOCKTYPE>(m_ItemType);
		NIBBLETYPE Meta;
		auto yaw = a_Player.GetYaw();
		switch (NeighborIdx)
		{
			case 0:
			case 2:
			{
				// The neighbor is in the X axis, form a X-axis-aligned dblchest:
				Meta = ((yaw >= -90) && (yaw < 90)) ? E_META_CHEST_FACING_ZM : E_META_CHEST_FACING_ZP;
				break;
			}
			case 1:
			case 3:
			{
				// The neighbor is in the Z axis, form a Z-axis-aligned dblchest:
				Meta = (yaw < 0) ? E_META_CHEST_FACING_XM : E_META_CHEST_FACING_XP;
				break;
			}
			default:
			{
				// No neighbor, place based on yaw:
				Meta = cBlockChestHandler::PlayerYawToMetaData(yaw);
				break;
			}
		}  // switch (NeighborIdx)

		// Place the new chest:
		if (!a_Player.PlaceBlock(PlacePos.x, PlacePos.y, PlacePos.z, ChestBlockType, Meta))
		{
			return false;
		}

		// Adjust the existing chest, if any:
		if (NeighborIdx != -1)
		{
			a_World.FastSetBlock(PlacePos + CrossCoords[NeighborIdx], ChestBlockType, Meta);
		}

		// Remove the "placed" item from inventory:
		if (a_Player.IsGameModeSurvival())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
		return true;
	}

private:
	cItemChestHandler(const cItemChestHandler &) = delete;
};
