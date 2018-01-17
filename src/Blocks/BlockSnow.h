
#pragma once

#include "BlockHandler.h"





class cBlockSnowHandler :
	public cBlockHandler
{
public:
	enum
	{
		FullBlockMeta = 7  // Meta value of a full-height snow block
	};

	cBlockSnowHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;

		BLOCKTYPE BlockBeforePlacement;
		NIBBLETYPE MetaBeforePlacement;
		a_ChunkInterface.GetBlockTypeMeta({a_BlockX, a_BlockY, a_BlockZ}, BlockBeforePlacement, MetaBeforePlacement);

		if ((BlockBeforePlacement == E_BLOCK_SNOW) && (MetaBeforePlacement < FullBlockMeta))
		{
			// Only increment if:
			//  - A snow block was already there (not first time placement) AND
			//  - Height is smaller than the maximum possible
			a_BlockMeta = MetaBeforePlacement + 1;
			return true;
		}

		// First time placement, check placement is valid
		a_BlockMeta = 0;

		BLOCKTYPE BlockBelow;
		NIBBLETYPE MetaBelow;
		return (
			(a_BlockY > 0) &&
			a_ChunkInterface.GetBlockTypeMeta({a_BlockX, a_BlockY - 1, a_BlockZ}, BlockBelow, MetaBelow) &&
			CanBeOn(BlockBelow, MetaBelow)
		);
	}

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) override
	{
		if ((a_Player.GetEquippedItem().m_ItemType == E_BLOCK_SNOW) && (a_Meta < FullBlockMeta))
		{
			return true;  // If a player is holding a (thin) snow block and it's size can be increased, return collision ignored
		}

		if (a_Meta == 0)
		{
			return true;  // If at normal snowfall height (lowest), we ignore collision
		}

		return false;
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SNOWBALL, 1, 0));
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY > 0)
		{
			BLOCKTYPE BlockBelow = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
			NIBBLETYPE MetaBelow = a_Chunk.GetMeta(a_RelX, a_RelY - 1, a_RelZ);

			return CanBeOn(BlockBelow, MetaBelow);
		}

		return false;
	}

	virtual bool DoesDropOnUnsuitable(void) override
	{
		return false;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 14;
	}

	virtual bool IsInsideBlock(Vector3d a_Position, const BLOCKTYPE a_BlockType, const NIBBLETYPE a_BlockMeta) override
	{
		return a_Position.y < (cBlockInfo::GetBlockHeight(a_BlockType) * (a_BlockMeta & 0x07));
	}

private:

	/** Returns true if snow can be placed on top of a block with the given type and meta. */
	static bool CanBeOn(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
	{
		// If block below is snowable, or it is a thin slow block and is a full thin snow block, say yay
		return (
			cBlockInfo::IsSnowable(a_BlockType) ||
			(
				(a_BlockType == E_BLOCK_SNOW) &&
				(a_BlockMeta == FullBlockMeta)
			)
		);
	}

} ;




