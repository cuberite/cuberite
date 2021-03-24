
#pragma once

#include "BlockHandler.h"





class cBlockConcretePowderHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockConcretePowder(BlockState a_Block)
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

	static inline BlockState GetConcreteFromConcretePowder(BlockState a_Block)
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
		BlockState a_Block
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
		const auto & WaterCheck = cSimulator::ThreeDimensionalNeighborCoords;
		const bool ShouldSoak = std::any_of(WaterCheck.cbegin(), WaterCheck.cend(), [a_Rel, & a_Chunk](Vector3i a_Offset)
		{
			BlockState Neighbor = 0;
			return (
				a_Chunk.UnboundedRelGetBlock(a_Rel.x + a_Offset.x, a_Rel.y + a_Offset.y, a_Rel.z + a_Offset.z, Neighbor)
				&& (Neighbor.Type() == BlockType::Water)
			);
		});

		if (ShouldSoak)
		{
			a_Chunk.SetBlock(a_Rel, GetConcreteFromConcretePowder(a_Chunk.GetBlock(a_Rel)));
		}
	}

	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::WhiteConcretePowder: return 8;
			case BlockType::OrangeConcretePowder: return 15;
			case BlockType::MagentaConcretePowder: return 16;
			case BlockType::LightBlueConcretePowder: return 17;
			case BlockType::YellowConcretePowder: return 18;
			case BlockType::LimeConcretePowder: return 19;
			case BlockType::PinkConcretePowder: return 20;
			case BlockType::GrayConcretePowder: return 21;
			case BlockType::LightGrayConcretePowder: return 22;
			case BlockType::CyanConcretePowder: return 23;
			case BlockType::PurpleConcretePowder: return 24;
			case BlockType::BlueConcretePowder: return 25;
			case BlockType::BrownConcretePowder: return 26;
			case BlockType::GreenConcretePowder: return 27;
			case BlockType::RedConcretePowder: return 28;
			case BlockType::BlackConcretePowder: return 29;
			default:
			{
				ASSERT(!"Unhandled meta in concrete powder handler!");
				return 0;
			}
		}
	}
};
