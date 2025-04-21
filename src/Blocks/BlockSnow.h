
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
		FullBlockLayers = 8  // Meta value of a full-height snow block.
	};

	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, BlockState a_ClickedBlock, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const override
	{
		if (Block::Snow::Layers(a_ClickedBlock) == 0)
		{
			return true;  // If at normal snowfall height (lowest), we ignore collision.
		}

		// Special case if a player is holding a (thin) snow block and its size can be increased:
		if ((a_HeldItem.m_ItemType == Item::Snow) && (Block::Snow::Layers(a_ClickedBlock) < FullBlockLayers))
		{
			return !a_ClickedDirectly || (a_ClickedBlockFace == BLOCK_FACE_YP);  // If clicked an adjacent block, or clicked YP directly, we ignore collision.
		}

		return false;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// No drop unless dug up with a shovel:
		if ((a_Tool == nullptr) || !ItemCategory::IsShovel(a_Tool->m_ItemType))
		{
			return {};
		}

		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(Item::Snow);
		}
		else
		{
			// Drop as many snowballs as there were "layers" of snow:
			return cItem(Item::Snowball, static_cast<char>(Block::Snow::Layers(a_Block) + 1));
		}
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		const auto BelowPos = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(BelowPos))
		{
			return false;
		}
		auto BlockBelow = a_Chunk.GetBlock(BelowPos);
		return CanBeOn(BlockBelow);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 14;
	}





	virtual bool IsInsideBlock(const Vector3d a_RelPosition, BlockState a_Block) const override
	{
		return a_RelPosition.y < (cBlockInfo::GetBlockHeight(m_BlockType) * (Block::Snow::Layers(a_Block)));
	}


private:

	/** Returns true if snow can be placed on top of a block with the given type and meta. */
	static bool CanBeOn(BlockState a_Block)
	{
		// If block below is snowable, or it is a thin snow block and is a full thin snow block, say yay:
		return (
			cBlockInfo::IsSnowable(a_Block) ||
			(
				(a_Block.Type() == BlockType::Snow) &&
				(Block::Snow::Layers(a_Block) == FullBlockLayers)
			)
		);
	}

} ;




