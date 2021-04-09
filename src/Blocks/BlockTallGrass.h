
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"





/** Handles the grass that is 1 block tall */
class cBlockTallGrassHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, BlockState a_Block) const override
	{
		return true;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// If using shears, drop self:
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			return cItem(BlockItemConverter::FromBlock(m_BlockType));
		}

		// Drop seeds, depending on bernoulli trial result:
		if (GetRandomProvider().RandBool(0.875))  // 87.5% chance of dropping nothing
		{
			return {};
		}

		// 12.5% chance of dropping 0 or more seeds.
		const auto DropNum = FortuneDiscreteRandom(1, 1, 2 * ToolFortuneLevel(a_Tool));
		return cItem(Item::WheatSeeds, DropNum);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		if (a_RelPos.y <= 0)
		{
			return false;
		}

		auto BelowBlock = a_Chunk.GetBlock(a_RelPos.addedY(-1));
		return IsBlockStateOfDirt(BelowBlock);
	}





	/** Growing a tall grass produces a big flower (2-block high fern or double-tall grass). */
	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, unsigned char a_NumStages = 1) const override
	{
		if (a_RelPos.y > (cChunkDef::Height - 2))
		{
			return 0;
		}
		auto Self = a_Chunk.GetBlock(a_RelPos);
		switch (Self.Type())
		{
			case BlockType::Grass:
			{
				a_Chunk.SetBlock(a_RelPos,           Block::TallGrass::TallGrass(Block::TallGrass::Half::Lower));
				a_Chunk.SetBlock(a_RelPos.addedY(1), Block::TallGrass::TallGrass(Block::TallGrass::Half::Upper));
				break;
			}
			case BlockType::Fern:
			{
				a_Chunk.SetBlock(a_RelPos,           Block::LargeFern::LargeFern(Block::LargeFern::Half::Lower));
				a_Chunk.SetBlock(a_RelPos.addedY(1), Block::LargeFern::LargeFern(Block::LargeFern::Half::Upper));
				break;
			}
			default:                      return 0;
		}
		return 1;
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
} ;




