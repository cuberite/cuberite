
#pragma once

#include "ItemHandler.h"
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
		if (a_BlockFace < 0)
		{
			// Clicked in air
			return false;
		}

		if ((a_BlockY < 0) || (a_BlockY >= cChunkDef::Height))
		{
			// The clicked block is outside the world, ignore this call altogether (#128)
			return false;
		}

		// Check if the block ignores build collision (water, grass etc.):
		BLOCKTYPE ClickedBlock;
		NIBBLETYPE ClickedBlockMeta;
		a_World.GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, ClickedBlock, ClickedBlockMeta);
		cChunkInterface ChunkInterface(a_World.GetChunkMap());
		if (BlockHandler(ClickedBlock)->DoesIgnoreBuildCollision(ChunkInterface, { a_BlockX, a_BlockY, a_BlockZ }, a_Player, ClickedBlockMeta))
		{
			BlockHandler(ClickedBlock)->OnDestroyedByPlayer(ChunkInterface, a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ);
		}
		else
		{
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

			if ((a_BlockY < 0) || (a_BlockY >= cChunkDef::Height))
			{
				// The block is being placed outside the world, ignore this packet altogether (#128)
				return false;
			}

			NIBBLETYPE PlaceMeta;
			BLOCKTYPE PlaceBlock;
			a_World.GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, PlaceBlock, PlaceMeta);

			// Clicked on side of block, make sure that placement won't be cancelled if there is a slab able to be double slabbed.
			// No need to do combinability (dblslab) checks, client will do that here.
			if (BlockHandler(ClickedBlock)->DoesIgnoreBuildCollision(ChunkInterface, { a_BlockX, a_BlockY, a_BlockZ }, a_Player, ClickedBlockMeta))
			{
				// Tried to place a block into another?
				// Happens when you place a block aiming at side of block with a torch on it or stem beside it
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
			if (a_World.GetBlock(a_BlockX + CrossCoords[i].x, a_BlockY, a_BlockZ + CrossCoords[i].z) != m_ItemType)
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
			int bx = a_BlockX + CrossCoords[i].x;
			int bz = a_BlockZ + CrossCoords[i].z;
			for (size_t j = 0; j < ARRAYCOUNT(CrossCoords); j++)
			{
				if (a_World.GetBlock(bx + CrossCoords[j].x, a_BlockY, bz + CrossCoords[j].z) == m_ItemType)
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
		if (!a_Player.PlaceBlock(a_BlockX, a_BlockY, a_BlockZ, ChestBlockType, Meta))
		{
			return false;
		}

		// Adjust the existing chest, if any:
		if (NeighborIdx != -1)
		{
			a_World.FastSetBlock(a_BlockX + CrossCoords[NeighborIdx].x, a_BlockY, a_BlockZ + CrossCoords[NeighborIdx].z, ChestBlockType, Meta);
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
