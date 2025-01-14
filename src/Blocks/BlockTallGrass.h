
#pragma once

#include "BlockHandler.h"
#include "Mixins/DirtLikeUnderneath.h"
#include "ChunkInterface.h"





/** Handles the grass that is 1 block tall */
class cBlockTallGrassHandler final :
	public cDirtLikeUnderneath<cBlockHandler>
{
	using Super = cDirtLikeUnderneath<cBlockHandler>;

public:

	using Super::Super;

private:

	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, Vector3i a_Position, BlockState a_ClickedBlock, eBlockFace a_ClickedBlockFace, bool a_ClickedDirectly) const override
	{
		return true;
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// If using shears, drop self:
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == Item::Shears))
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





	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		if (!cChunkDef::IsValidHeight(a_Position))
		{
			return false;
		}

		auto BelowBlock = a_Chunk.GetBlock(a_Position.addedY(-1));
		return IsBlockMaterialDirt(BelowBlock);
	}





	/** Growing a tall grass produces a big flower (2-block high fern or double-tall grass). */
	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, char a_NumStages = 1) const override
	{
		const auto TopPos = a_RelPos.addedY(1);
		if (!cChunkDef::IsValidHeight(TopPos))
		{
			return 0;
		}
		auto Self = a_Chunk.GetBlock(a_RelPos);
		switch (Self.Type())
		{
			case BlockType::ShortGrass:
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




