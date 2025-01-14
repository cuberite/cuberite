
#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"
#include "../Chunk.h"
#include "BlockStairs.h"
#include "BlockFence.h"




class cBlockPressurePlateHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsPressurePlate(BlockState a_Block)
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
			case BlockType::BambooPressurePlate:
			case BlockType::CherryPressurePlate:
			case BlockType::MangrovePressurePlate:
			case BlockType::PaleOakPressurePlate:
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
			case BlockType::BambooPressurePlate:             Self = Block::BambooPressurePlate::BambooPressurePlate(a_Pressed); break;
			case BlockType::CherryPressurePlate:             Self = Block::CherryPressurePlate::CherryPressurePlate(a_Pressed); break;
			case BlockType::MangrovePressurePlate:           Self = Block::MangrovePressurePlate::MangrovePressurePlate(a_Pressed); break;
			case BlockType::PaleOakPressurePlate:            Self = Block::PaleOakPressurePlate::PaleOakPressurePlate(a_Pressed); break;
			default: return;
		}
		a_ChunkInterface.FastSetBlock(a_Position, Self);
	}

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		const auto PosBelow = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(PosBelow))
		{
			return false;
		}

		auto Block = a_Chunk.GetBlock(PosBelow);

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

		if (cBlockFenceHandler::IsBlockFence(Block.Type()))
		{
			return true;
		}

		return !cBlockInfo::IsTransparent(Block);
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
			case BlockType::BambooPressurePlate:
			case BlockType::CherryPressurePlate:
			case BlockType::MangrovePressurePlate:
			case BlockType::PaleOakPressurePlate: return 0;
			default:
			{
				ASSERT(!"Unhandled blocktype in pressure plate handler!");
				return 0;
			}
		}
	}
} ;




