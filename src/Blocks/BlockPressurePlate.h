
#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"
#include "../Chunk.h"
#include "BlockStairs.h"




class cBlockPressurePlateHandler final :
	public cClearMetaOnDrop<cBlockHandler>
{
	using Super = cClearMetaOnDrop<cBlockHandler>;

public:

	using Super::Super;

	static constexpr bool IsPressurePlate(BlockState a_Block)
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

	// Todo: Add this, once the BlockState is implemented
	/*
	static constexpr void SetPressuredState(cChunkInterface & a_ChunkInterface, Vector3i a_Position, bool a_Pressed)
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
	*/

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		if (a_Position.y <= 0)
		{
			return false;
		}

		BLOCKTYPE Block;
		NIBBLETYPE BlockMeta;
		a_Chunk.GetBlockTypeMeta(a_Position.addedY(-1), Block, BlockMeta);

		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(Block))
		{
			return BlockMeta & E_META_WOODEN_SLAB_UPSIDE_DOWN;
		}

		// upside down stairs
		if (cBlockStairsHandler::IsAnyStairType(Block))
		{
			return BlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN;
		}

		switch (Block)
		{
			case E_BLOCK_ACACIA_FENCE:
			case E_BLOCK_BIRCH_FENCE:
			case E_BLOCK_DARK_OAK_FENCE:
			case E_BLOCK_FENCE:
			case E_BLOCK_HOPPER:
			case E_BLOCK_JUNGLE_FENCE:
			case E_BLOCK_NETHER_BRICK_FENCE:
			case E_BLOCK_SPRUCE_FENCE:
			{
				return true;
			}
			default:
			{
				return !cBlockInfo::IsTransparent(Block);
			}
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		switch (m_BlockType)
		{
			case E_BLOCK_STONE_PRESSURE_PLATE: return 11;
			case E_BLOCK_WOODEN_PRESSURE_PLATE: return 13;
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE: return 6;
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE: return 30;
			default:
			{
				ASSERT(!"Unhandled blocktype in pressure plate handler!");
				return 0;
			}
		}
	}
} ;




