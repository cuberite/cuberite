
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../Items/ItemHandler.h"




class cBlockBigFlowerHandler :
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockBigFlowerHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) override
	{
		if (IsMetaTopPart(a_Meta))
		{
			BLOCKTYPE BottomType;
			if (
				(a_Pos.y < 1) ||
				!a_ChunkInterface.GetBlockTypeMeta(a_Pos - Vector3i(0, 1, 0), BottomType, a_Meta) ||
				(BottomType != E_BLOCK_BIG_FLOWER)
			)
			{
				// Can't find the flower meta so assume grass
				return true;
			}
		}

		NIBBLETYPE FlowerMeta = a_Meta & 0x07;
		return (
			(FlowerMeta == E_META_BIG_FLOWER_DOUBLE_TALL_GRASS) ||
			(FlowerMeta == E_META_BIG_FLOWER_LARGE_FERN)
		);
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		if (IsMetaTopPart(a_BlockMeta))
		{
			return {};  // No drops from the top part
		}

		// With shears, drop self (even tall grass and fern):
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			// Bit 0x08 specifies whether this is a top part or bottom; cut it off from the pickup:
			return cItem(m_BlockType, 1, a_BlockMeta & 0x07);
		}

		// Tall grass drops seeds, large fern drops nothing, others drop self:
		auto flowerType = a_BlockMeta & 0x07;
		if (flowerType == E_META_BIG_FLOWER_DOUBLE_TALL_GRASS)
		{
			if (GetRandomProvider().RandBool(1.0 / 24.0))
			{
				return cItem(E_ITEM_SEEDS);
			}
		}
		else if (flowerType != E_META_BIG_FLOWER_LARGE_FERN)
		{
			return cItem(m_BlockType, 1, static_cast<short>(flowerType));
		}
		return {};
	}





	bool IsMetaTopPart(NIBBLETYPE a_Meta)
	{
		return ((a_Meta & 0x08) != 0);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		a_Chunk.GetBlockTypeMeta(a_RelX, a_RelY - 1, a_RelZ, BlockType, BlockMeta);

		return IsBlockTypeOfDirt(BlockType) || ((BlockType == E_BLOCK_BIG_FLOWER) && !IsMetaTopPart(BlockMeta));
	}





	virtual void OnBroken(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, Vector3i a_BlockPos, BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta) override
	{
		if ((a_OldBlockMeta & 0x8) != 0)
		{
			// Was upper part of flower
			auto lowerPartPos = a_BlockPos - Vector3i(0, 1, 0);
			if (a_ChunkInterface.GetBlock(lowerPartPos) == a_OldBlockType)
			{
				a_ChunkInterface.DropBlockAsPickups(lowerPartPos);
			}
		}
		else
		{
			// Was lower part
			auto upperPartPos = a_BlockPos + Vector3i(0, 1, 0);
			if (a_ChunkInterface.GetBlock(upperPartPos) == a_OldBlockType)
			{
				a_ChunkInterface.DropBlockAsPickups(upperPartPos);
			}
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;
