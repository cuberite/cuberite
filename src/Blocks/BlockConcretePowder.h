
#pragma once

#include "BlockHandler.h"





class cBlockConcretePowderHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static constexpr bool IsBlockConcretePowder(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::BlackConcretePowder:
			case BlockType::BlueConcretePowder:
			case BlockType::BrownConcretePowder:
			case BlockType::CyanConcretePowder:
			case BlockType::GrayConcretePowder:
			case BlockType::GreenConcretePowder:
			case BlockType::LightBlueConcretePowder:
			case BlockType::LightGrayConcretePowder:
			case BlockType::LimeConcretePowder:
			case BlockType::MagentaConcretePowder:
			case BlockType::OrangeConcretePowder:
			case BlockType::PinkConcretePowder:
			case BlockType::PurpleConcretePowder:
			case BlockType::RedConcretePowder:
			case BlockType::WhiteConcretePowder:
			case BlockType::YellowConcretePowder:
				return true;
			default: return false;
		}
	}

	static constexpr BlockState GetConcreteFromConcretePowder(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::BlackConcretePowder:     return Block::BlackConcrete::BlackConcrete();
			case BlockType::BlueConcretePowder:      return Block::BlueConcrete::BlueConcrete();
			case BlockType::BrownConcretePowder:     return Block::BrownConcrete::BrownConcrete();
			case BlockType::CyanConcretePowder:      return Block::CyanConcrete::CyanConcrete();
			case BlockType::GrayConcretePowder:      return Block::GrayConcrete::GrayConcrete();
			case BlockType::GreenConcretePowder:     return Block::GreenConcrete::GreenConcrete();
			case BlockType::LightBlueConcretePowder: return Block::LightBlueConcrete::LightBlueConcrete();
			case BlockType::LightGrayConcretePowder: return Block::LightGrayConcrete::LightGrayConcrete();
			case BlockType::LimeConcretePowder:      return Block::LimeConcrete::LimeConcrete();
			case BlockType::MagentaConcretePowder:   return Block::MagentaConcrete::MagentaConcrete();
			case BlockType::OrangeConcretePowder:    return Block::OrangeConcrete::OrangeConcrete();
			case BlockType::PinkConcretePowder:      return Block::PinkConcrete::PinkConcrete();
			case BlockType::PurpleConcretePowder:    return Block::PurpleConcrete::PurpleConcrete();
			case BlockType::RedConcretePowder:       return Block::RedConcrete::RedConcrete();
			case BlockType::WhiteConcretePowder:     return Block::WhiteConcrete::WhiteConcrete();
			case BlockType::YellowConcretePowder:    return Block::YellowConcrete::YellowConcrete();
			default: return a_Block;
		}
	}

private:

	virtual void OnPlaced(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) const override
	{
		OnNeighborChanged(a_ChunkInterface, a_BlockPos, BLOCK_FACE_NONE);
	}

	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		a_ChunkInterface.DoWithChunkAt(a_BlockPos, [&](cChunk & a_Chunk) { CheckSoaked(cChunkDef::AbsoluteToRelative(a_BlockPos), a_Chunk); return true; });
	}

	/** Check blocks above and around to see if they are water. If one is, converts this into concrete block. */
	static void CheckSoaked(Vector3i a_Rel, cChunk & a_Chunk)
	{
		const auto & WaterCheck = cSimulator::AdjacentOffsets;
		const bool ShouldSoak = std::any_of(WaterCheck.cbegin(), WaterCheck.cend(), [a_Rel, & a_Chunk](Vector3i a_Offset)
		{
			BLOCKTYPE NeighborType;
			return (
				a_Chunk.UnboundedRelGetBlockType(a_Rel.x + a_Offset.x, a_Rel.y + a_Offset.y, a_Rel.z + a_Offset.z, NeighborType)
				&& IsBlockWater(NeighborType)
			);
		});

		if (ShouldSoak)
		{
			NIBBLETYPE BlockMeta;
			BlockMeta = a_Chunk.GetMeta(a_Rel.x, a_Rel.y, a_Rel.z);
			a_Chunk.SetBlock(a_Rel, E_BLOCK_CONCRETE, BlockMeta);
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		switch (a_Meta)
		{
			case E_META_CONCRETE_POWDER_WHITE: return 8;
			case E_META_CONCRETE_POWDER_ORANGE: return 15;
			case E_META_CONCRETE_POWDER_MAGENTA: return 16;
			case E_META_CONCRETE_POWDER_LIGHTBLUE: return 17;
			case E_META_CONCRETE_POWDER_YELLOW: return 18;
			case E_META_CONCRETE_POWDER_LIGHTGREEN: return 19;
			case E_META_CONCRETE_POWDER_PINK: return 20;
			case E_META_CONCRETE_POWDER_GRAY: return 21;
			case E_META_CONCRETE_POWDER_LIGHTGRAY: return 22;
			case E_META_CONCRETE_POWDER_CYAN: return 23;
			case E_META_CONCRETE_POWDER_PURPLE: return 24;
			case E_META_CONCRETE_POWDER_BLUE: return 25;
			case E_META_CONCRETE_POWDER_BROWN: return 26;
			case E_META_CONCRETE_POWDER_GREEN: return 27;
			case E_META_CONCRETE_POWDER_RED: return 28;
			case E_META_CONCRETE_POWDER_BLACK: return 29;
			default:
			{
				ASSERT(!"Unhandled meta in concrete powder handler!");
				return 0;
			}
		}
	}
};
