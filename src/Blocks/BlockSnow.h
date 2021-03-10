
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
		FullBlockLayers = 7  // Meta value of a full-height snow block
	};

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		unsigned char NewLayerCount = 0;

		// Check if incrementing existing snow height:
		auto BlockToReplace = a_ChunkInterface.GetBlock(a_PlacedBlockPos);
		if ((BlockToReplace.Type() == BlockType::Snow) && (Block::Snow::Layers(a_Block) < FullBlockLayers))
		{
			// Only increment if:
			//  - A snow block was already there (not first time placement) AND
			//  - Height is smaller than the maximum possible
			NewLayerCount = Block::Snow::Layers(a_Block) + 1;
			a_Block = Block::Snow::Snow(NewLayerCount);
			return true;
		}

		// First time placement, check placement is valid
		a_Block = Block::Snow::Snow();
		auto BlockBelow = a_ChunkInterface.GetBlock(a_PlacedBlockPos.addedY(-1));
		return (
			(a_PlacedBlockPos.y > 0) &&
			CanBeOn(BlockBelow)
		);
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, BlockState a_Block) const override
	{
		if ((a_Player.GetEquippedItem().m_ItemType == E_BLOCK_SNOW) && (Block::Snow::Layers(a_Block) < FullBlockLayers))
		{
			return true;  // If a player is holding a (thin) snow block and it's size can be increased, return collision ignored
		}

		if (Block::Snow::Layers(a_Block) == 0)
		{
			return true;  // If at normal snowfall height (lowest), we ignore collision
		}

		return false;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// No drop unless dug up with a shovel
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
			return cItem(Item::Snowball, 1 + Block::Snow::Layers(a_Block));
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
		// If block below is snowable, or it is a thin slow block and is a full thin snow block, say yay
		return (
			cBlockInfo::IsSnowable(a_Block) ||
			(
				(a_Block.Type() == BlockType::Snow) &&
				(Block::Snow::Layers(a_Block) == FullBlockLayers)
			)
		);
	}

} ;




