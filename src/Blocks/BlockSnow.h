
#pragma once

#include "BlockHandler.h"





class cBlockSnowHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	enum
	{
		FullBlockMeta = 7  // Meta value of a full-height snow block
	};

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		a_BlockType = m_BlockType;

		// Check if incrementing existing snow height:
		BLOCKTYPE BlockBeforePlacement;
		NIBBLETYPE MetaBeforePlacement;
		a_ChunkInterface.GetBlockTypeMeta(a_PlacedBlockPos, BlockBeforePlacement, MetaBeforePlacement);
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
			(a_PlacedBlockPos.y > 0) &&
			a_ChunkInterface.GetBlockTypeMeta(a_PlacedBlockPos.addedY(-1), BlockBelow, MetaBelow) &&
			CanBeOn(BlockBelow, MetaBelow)
		);
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) const override
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





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// No drop unless dug up with a shovel
		if ((a_Tool == nullptr) || !ItemCategory::IsShovel(a_Tool->m_ItemType))
		{
			return {};
		}

		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType, 1, 0);
		}
		else
		{
			// Drop as many snowballs as there were "layers" of snow:
			return cItem(E_ITEM_SNOWBALL, 1 + (a_BlockMeta & 0x07), 0);
		}
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}
		auto BelowPos = a_RelPos.addedY(-1);
		auto BlockBelow = a_Chunk.GetBlock(BelowPos);
		auto MetaBelow = a_Chunk.GetMeta(BelowPos);
		return CanBeOn(BlockBelow, MetaBelow);
	}





	virtual bool DoesDropOnUnsuitable(void) const override
	{
		return false;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 14;
	}





	virtual bool IsInsideBlock(const Vector3d a_RelPosition, const NIBBLETYPE a_BlockMeta) const override
	{
		return a_RelPosition.y < (cBlockInfo::GetBlockHeight(m_BlockType) * (a_BlockMeta & 0x07));
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




