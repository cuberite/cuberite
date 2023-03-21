
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
		FullBlockMeta = 7  // Meta value of a full-height snow block.
	};


	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, const Vector3i a_Position, const NIBBLETYPE a_Meta, const eBlockFace a_ClickedBlockFace, const bool a_ClickedDirectly) const override
	{
		if (a_Meta == 0)
		{
			return true;  // If at normal snowfall height (lowest), we ignore collision.
		}

		// Special case if a player is holding a (thin) snow block and its size can be increased:
		if ((a_HeldItem.m_ItemType == E_BLOCK_SNOW) && (a_Meta < FullBlockMeta))
		{
			return !a_ClickedDirectly || (a_ClickedBlockFace == BLOCK_FACE_YP);  // If clicked an adjacent block, or clicked YP directly, we ignore collision.
		}

		return false;
	}





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// No drop unless dug up with a shovel:
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





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		if (a_Position.y <= 0)
		{
			return false;
		}
		auto BelowPos = a_Position.addedY(-1);
		auto BlockBelow = a_Chunk.GetBlock(BelowPos);
		auto MetaBelow = a_Chunk.GetMeta(BelowPos);
		return CanBeOn(BlockBelow, MetaBelow);
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
		// If block below is snowable, or it is a thin snow block and is a full thin snow block, say yay:
		return (
			cBlockInfo::IsSnowable(a_BlockType) ||
			(
				(a_BlockType == E_BLOCK_SNOW) &&
				(a_BlockMeta == FullBlockMeta)
			)
		);
	}

} ;




