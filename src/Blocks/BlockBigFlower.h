
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../Entities/Player.h"
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

	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, const Vector3i a_Position, BlockState a_Block, const eBlockFace a_ClickedBlockFace, const bool a_ClickedDirectly) const override
	{
		if (IsTopPart(a_Block))
		{
			const auto BottomPosition = a_Position.addedY(-1);
			if (
				!cChunkDef::IsValidHeight(BottomPosition) ||
				(IsBlockBigFlower(a_Block))
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





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		if (IsTopPart(a_Block))
		{
			return {};
		}

		// With shears, drop self (even tall grass and fern):
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == Item::Shears))
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
				return cItem(Item::WheatSeeds, DropNum);
			}
			case BlockType::LargeFern:
			{
				return cItem(Item::LargeFern);
			}
			default: return {};
		}
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		// CanBeAt is also called on placement, so the top part can't check for the bottom part.
		// Both parts can only that they're rooted in grass.

		const auto RootPosition = a_Position.addedY(IsTopPart(a_Self) ? -2 : -1);
		return cChunkDef::IsValidHeight(RootPosition) && IsBlockMaterialDirt(a_Chunk.GetBlock(RootPosition));
	}





	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		const Vector3i a_BlockPos,
		BlockState a_OldBlock,
		const cEntity * a_Digger
	) const override
	{
		if (IsTopPart(a_OldBlock))
		{
			const auto LowerPartPos = a_BlockPos.addedY(-1);
			if (a_ChunkInterface.GetBlock(LowerPartPos).Type() == a_OldBlock.Type())
			{
				// Prevent creative punches from dropping pickups.
				// TODO: Simplify to SetBlock and remove the IsMetaTopPart check in DropBlockAsPickups when 1.13 blockstates arrive.
				if ((a_Digger != nullptr) && a_Digger->IsPlayer() && static_cast<const cPlayer *>(a_Digger)->IsGameModeCreative())
				{
					a_ChunkInterface.SetBlock(LowerPartPos, Block::Air::Air());
				}
				else
				{
					a_ChunkInterface.DropBlockAsPickups(LowerPartPos);
				}
			}
		}
		else
		{
			const auto UpperPartPos = a_BlockPos.addedY(1);
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
