
#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"
#include "../Chunk.h"
#include "BlockStairs.h"




class cBlockPressurePlateHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool PolishedBlackstonePressurePlate(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaPressurePlate:
			case BlockType::BirchPressurePlate:
			case BlockType::CrimsonPressurePlate:
			case BlockType::DarkOakPressurePlate:
			case BlockType::HeavyWeightedPressurePlate:
			case BlockType::JunglePressurePlate:
			case BlockType::LightWeightedPressurePlate:
			case BlockType::OakPressurePlate:
			case BlockType::PolishedBlackstonePressurePlate:
			case BlockType::SprucePressurePlate:
			case BlockType::StonePressurePlate:
			case BlockType::WarpedPressurePlate:
				return true;
			default: return false;
		}
	}

	static inline void SetPressuredState(cChunkInterface & a_ChunkInterface, Vector3i a_Position, bool a_Pressed)
	{
		using namespace Block;
		auto Self = a_ChunkInterface.GetBlock(a_Position);
		switch (Self.Type())
		{
			case BlockType::AcaciaPressurePlate:             Self = Block::AcaciaPressurePlate::AcaciaPressurePlate(a_Pressed); break;
			case BlockType::BirchPressurePlate:              Self = Block::BirchPressurePlate::BirchPressurePlate(a_Pressed); break;
			case BlockType::CrimsonPressurePlate:            Self = Block::CrimsonPressurePlate::CrimsonPressurePlate(a_Pressed); break;
			case BlockType::DarkOakPressurePlate:            Self = Block::DarkOakPressurePlate::DarkOakPressurePlate(a_Pressed); break;
			case BlockType::HeavyWeightedPressurePlate:      Self = Block::HeavyWeightedPressurePlate::HeavyWeightedPressurePlate(a_Pressed); break;
			case BlockType::JunglePressurePlate:             Self = Block::JunglePressurePlate::JunglePressurePlate(a_Pressed); break;
			case BlockType::LightWeightedPressurePlate:      Self = Block::LightWeightedPressurePlate::LightWeightedPressurePlate(a_Pressed); break;
			case BlockType::OakPressurePlate:                Self = Block::OakPressurePlate::OakPressurePlate(a_Pressed); break;
			case BlockType::PolishedBlackstonePressurePlate: Self = Block::PolishedBlackstonePressurePlate::PolishedBlackstonePressurePlate(a_Pressed); break;
			case BlockType::SprucePressurePlate:             Self = Block::SprucePressurePlate::SprucePressurePlate(a_Pressed); break;
			case BlockType::StonePressurePlate:              Self = Block::StonePressurePlate::StonePressurePlate(a_Pressed); break;
			case BlockType::WarpedPressurePlate:             Self = Block::WarpedPressurePlate::WarpedPressurePlate(a_Pressed); break;
			default: return;
		}
		a_ChunkInterface.FastSetBlock(a_Position, Self);
	}

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		if (a_Position.y <= 0)
		{
			return false;
		}

		auto Block = a_Chunk.GetBlock(a_Position.addedY(-1));

		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(Block))
		{
			return cBlockSlabHandler::IsSlabTop(Block) || cBlockSlabHandler::IsSlabFull(Block);
		}

		// upside down stairs
		if (cBlockStairsHandler::IsAnyStairType(Block))
		{
			return cBlockStairsHandler::IsStairsTopHalf(Block);
		}

		switch (Block)
		{
			return cBlockSlabHandler::IsSlabTop(Block) || cBlockSlabHandler::IsSlabFull(Block);
		}
		switch (Block.Type())
		{
			case BlockType::AcaciaFence:
			case BlockType::BirchFence:
			case BlockType::CrimsonFence:
			case BlockType::DarkOakFence:
			case BlockType::OakFence:
			case BlockType::Hopper:
			case BlockType::JungleFence:
			case BlockType::NetherBrickFence:
			case BlockType::SpruceFence:
			case BlockType::WarpedFence:
			{
				return true;
			}
			default:
			{
				return !cBlockInfo::IsTransparent(Block);
			}
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::AcaciaPressurePlate:             return 15;
			case BlockType::BirchPressurePlate:              return 2;
			case BlockType::CrimsonPressurePlate:            return 35;
			case BlockType::DarkOakPressurePlate:            return 26;
			case BlockType::HeavyWeightedPressurePlate:      return 6;
			case BlockType::JunglePressurePlate:             return 10;
			case BlockType::LightWeightedPressurePlate:      return 30;
			case BlockType::OakPressurePlate:                return 13;
			case BlockType::PolishedBlackstonePressurePlate: return 0;  // TODO(12xx12)
			case BlockType::SprucePressurePlate:             return 34;
			case BlockType::StonePressurePlate:              return 11;
			case BlockType::WarpedPressurePlate:             return 56;
			default:
			{
				ASSERT(!"Unhandled blocktype in pressure plate handler!");
				return 0;
			}
		}
	}
} ;




