
#pragma once

#include "ItemHandler.h"
#include "../BlockInfo.h"
#include "../Blocks/BlockChest.h"





class cItemChestHandler:
	public cItemHandler
{
	typedef cItemHandler super;
public:
	cItemChestHandler(int a_ItemType):
		super(a_ItemType)
	{
	}


	/** We need an OnPlayerPlace override because we're processing neighbor chests and changing their metas,
	the parent class cannot do that. */
	virtual bool OnPlayerPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ
	) override
	{
		Vector3i absPos(a_BlockX, a_BlockY, a_BlockZ);

		if (a_BlockFace < 0)
		{
			// Clicked in air
			return false;
		}

		if (!cChunkDef::IsValidHeight(absPos.y))
		{
			// The clicked block is outside the world, ignore this call altogether (#128)
			return false;
		}

		// Check if the block ignores build collision (water, grass etc.):
		BLOCKTYPE clickedBlock;
		NIBBLETYPE clickedBlockMeta;
		a_World.GetBlockTypeMeta(absPos.x, absPos.y, absPos.z, clickedBlock, clickedBlockMeta);

		cChunkInterface ChunkInterface(a_World.GetChunkMap());
		// Check if the block ignores build collision (water, grass etc.):
		if (BlockHandler(clickedBlock)->DoesIgnoreBuildCollision(ChunkInterface, absPos, a_Player, clickedBlockMeta))
		{
			a_World.DropBlockAsPickups(absPos, &a_Player, nullptr);
		}
		else
		{
			AddFaceDirection(absPos.x, absPos.y, absPos.z, a_BlockFace);

			if (!cChunkDef::IsValidHeight(absPos.y))
			{
				// The block is being placed outside the world, ignore this packet altogether (#128)
				return false;
			}

			NIBBLETYPE PlaceMeta;
			BLOCKTYPE PlaceBlock;
			a_World.GetBlockTypeMeta(absPos.x, absPos.y, absPos.z, PlaceBlock, PlaceMeta);

			// Check for another block here, remove it if appropriate.
			if (!BlockHandler(PlaceBlock)->DoesIgnoreBuildCollision(ChunkInterface, absPos, a_Player, clickedBlockMeta))
			{
				// Tried to place a block into another?
				// Happens when you place a block aiming at side of block with a torch on it or stem beside it
				return false;
			}

			a_World.DropBlockAsPickups(absPos, &a_Player, nullptr);
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
			if (a_World.GetBlock(absPos + CrossCoords[i]) != m_ItemType)
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
				if (a_World.GetBlock(absPos + CrossCoords[i] + CrossCoords[j]) == m_ItemType)
				{
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
				Meta = cBlockChestHandler::PlayerYawToMetaData(yaw);
				break;
			}
		}  // switch (NeighborIdx)

		// Place the new chest:
		if (!a_Player.PlaceBlock(absPos.x, absPos.y, absPos.z, ChestBlockType, Meta))
		{
			return false;
		}

		// Adjust the existing chest, if any:
		if (NeighborIdx != -1)
		{
			a_World.FastSetBlock(absPos + CrossCoords[NeighborIdx], ChestBlockType, Meta);
		}

		// Remove the "placed" item:
		if (a_Player.IsGameModeSurvival())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
		return true;
	}

private:
	cItemChestHandler(const cItemChestHandler &) = delete;
};
