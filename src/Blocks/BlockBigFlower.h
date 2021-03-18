
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

	static inline bool IsBlockBigFlower(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::TallGrass:
			case BlockType::LargeFern:
			case BlockType::Lilac:
			case BlockType::Peony:
			case BlockType::RoseBush:
			case BlockType::Sunflower:
				return true;
			default: return false;
		}
	}

	static inline bool IsTopPart(BlockState a_Block)
	{
		using namespace Block;
		switch (a_Block.Type())
		{
			case BlockType::TallGrass: return TallGrass::Half(a_Block) == TallGrass::Half::Upper;
			case BlockType::LargeFern: return LargeFern::Half(a_Block) == LargeFern::Half::Upper;
			case BlockType::Lilac:     return Lilac::Half(a_Block)     == Lilac::Half::Upper;
			case BlockType::Peony:     return Peony::Half(a_Block)     == Peony::Half::Upper;
			case BlockType::RoseBush:  return RoseBush::Half(a_Block)  == RoseBush::Half::Upper;
			case BlockType::Sunflower: return Sunflower::Half(a_Block) == Sunflower::Half::Upper;
			default: return false;
		}
	}

private:

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, BlockState a_Block) const override
	{
		if (IsTopPart(a_Block))
		{
			if (
				auto BottomBlock = a_ChunkInterface.GetBlock(a_Pos - Vector3i(0, 1, 0));
				(a_Pos.y < 1) ||
				(!IsBlockBigFlower(BottomBlock))
			)
			{
				// Can't find the flower meta so assume grass
				return true;
			}
		}

		switch (a_Block.Type())
		{
			case BlockType::TallGrass:
			case BlockType::LargeFern:
				return true;
			default: return false;
		}
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		if (IsTopPart(a_Block))
		{
			return {};  // No drops from the top part
		}

		// With shears, drop self (even tall grass and fern):
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			switch (a_Block.Type())
			{
				case BlockType::TallGrass: return cItem(Item::TallGrass);
				case BlockType::LargeFern: return cItem(Item::LargeFern);
				case BlockType::Lilac:     return cItem(Item::Lilac);
				case BlockType::Peony:     return cItem(Item::Peony);
				case BlockType::RoseBush:  return cItem(Item::RoseBush);
				case BlockType::Sunflower: return cItem(Item::Sunflower);
				default: return {};
			}
		}

		// Tall grass drops seeds, large fern drops nothing, others drop self:
		switch (a_Block.Type())
		{
			case BlockType::TallGrass:
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
			case BlockType::LargeFern:
			{
				return cItem(Item::LargeFern);
			}
			default: return {};
		}
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}
		auto BlockBelow = a_Chunk.GetBlock(a_RelPos.addedY(-1));

		return IsBlockStateOfDirt(BlockBelow) || (IsBlockBigFlower(BlockBelow) && !IsTopPart(BlockBelow));
	}





	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		const Vector3i a_BlockPos,
		BlockState a_OldBlock,
		const cEntity * a_Digger
	) const override
	{
		UNUSED(a_Digger);
		if (IsTopPart(a_OldBlock))
		{
			// Was upper part of flower
			auto LowerPartPos = a_BlockPos.addedY(-1);
			if (a_ChunkInterface.GetBlock(LowerPartPos).Type() == a_OldBlock.Type())
			{
				a_ChunkInterface.DropBlockAsPickups(LowerPartPos);
			}
		}
		else
		{
			// Was lower part
			auto UpperPartPos = a_BlockPos.addedY(1);
			if (a_ChunkInterface.GetBlock(UpperPartPos).Type() == a_OldBlock.Type())
			{
				a_ChunkInterface.DropBlockAsPickups(UpperPartPos);
			}
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
} ;
