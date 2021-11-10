
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../BlockInfo.h"
#include "../Items/ItemHandler.h"




class cBlockBigFlowerHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, const Vector3i a_Position, NIBBLETYPE a_Meta, const eBlockFace a_ClickedBlockFace, const bool a_ClickedDirectly) const override
	{
		if (IsMetaTopPart(a_Meta))
		{
			BLOCKTYPE BottomType;
			if (
				(a_Position.y < 1) ||
				!a_World.GetBlockTypeMeta(a_Position - Vector3i(0, 1, 0), BottomType, a_Meta) ||
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





	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
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

			// Drop seeds, depending on bernoulli trial result:
			if (GetRandomProvider().RandBool(0.875))
			{
				// 87.5% chance of dropping nothing:
				return {};
			}

			// 12.5% chance of dropping some seeds.
			const auto DropNum = FortuneDiscreteRandom(1, 1, 2 * ToolFortuneLevel(a_Tool));
			return cItem(E_ITEM_SEEDS, DropNum);
		}
		else if (flowerType != E_META_BIG_FLOWER_LARGE_FERN)
		{
			return cItem(m_BlockType, 1, static_cast<short>(flowerType));
		}

		return {};
	}





	static bool IsMetaTopPart(NIBBLETYPE a_Meta)
	{
		return ((a_Meta & 0x08) != 0);
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		// CanBeAt is also called on placement, so the top part can't check for the bottom part.
		// Both parts can only that they're rooted in grass.

		const auto RootPosition = a_Position.addedY(IsMetaTopPart(a_Meta) ? -2 : -1);
		return (RootPosition.y >= 0) && IsBlockTypeOfDirt(a_Chunk.GetBlock(RootPosition));
	}





	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		const Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta,
		const cEntity * a_Digger
	) const override
	{
		UNUSED(a_Digger);
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





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;
