
#pragma once

#include "BlockHandler.h"





class cBlockSnowHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	enum
	{
		FullBlockMeta = 7  // Meta value of a full-height snow block
	};


	cBlockSnowHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
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





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
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





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
	{
		if (!cChunkDef::IsValidHeight(a_RelPos.y)) return false;

		Vector3i BelowPos = a_RelPos.addedY(-1);
		BLOCKTYPE BlockBelow;
		NIBBLETYPE MetaBelow;
		a_Chunk.GetBlockTypeMeta(BelowPos, BlockBelow, MetaBelow);

		return CanBeOn(BlockBelow, MetaBelow);
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





	virtual bool IsInsideBlock(const Vector3d a_RelPosition, const NIBBLETYPE a_BlockMeta) override
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




