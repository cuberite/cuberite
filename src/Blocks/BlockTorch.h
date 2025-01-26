#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"
#include "BlockStairs.h"
#include "../Chunk.h"
#include "BlockType.h"
#include "ChunkInterface.h"
#include "Defines.h"
#include "Mixins/Mixins.h"





class cBlockTorchHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockTorch(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Torch:
			case BlockType::WallTorch:
			case BlockType::RedstoneTorch:
			case BlockType::RedstoneWallTorch:
			case BlockType::SoulTorch:
			case BlockType::SoulWallTorch:
				return true;
			default: return false;
		}
	}


	/** Returns true if the torch can be placed on the specified block's face. */
	static bool CanBePlacedOn(BlockState a_Block, eBlockFace a_BlockFace)
	{
		if (cBlockStairsHandler::IsBlockStairs(a_Block) && cBlockStairsHandler::IsStairsTopHalf(a_Block))
		{
			return true;
		}

		if (cBlockSlabHandler::IsAnySlabType(a_Block))
		{
			if (cBlockSlabHandler::IsSlabTop(a_Block) && (a_BlockFace == eBlockFace::BLOCK_FACE_YP))
			{
				return true;
			}
			else if (cBlockSlabHandler::IsSlabFull(a_Block))
			{
				return (a_BlockFace != BLOCK_FACE_YM);
			}
		}
		if (cBlockStairsHandler::IsAnyStairType(a_Block))
		{
			return false;  // TODO: fix (a_BlockFace == cBlockStairsHandler::GetRotation);
		}

		switch (a_Block.Type())
		{
			case BlockType::EndPortalFrame:
			case BlockType::SoulSand:
			{
				// Exceptional vanilla behaviour
				return true;
			}
			case BlockType::Glass:
			case BlockType::BlackStainedGlass:
			case BlockType::BlueStainedGlass:
			case BlockType::BrownStainedGlass:
			case BlockType::CyanStainedGlass:
			case BlockType::GrayStainedGlass:
			case BlockType::GreenStainedGlass:
			case BlockType::LightBlueStainedGlass:
			case BlockType::LightGrayStainedGlass:
			case BlockType::LimeStainedGlass:
			case BlockType::MagentaStainedGlass:
			case BlockType::OrangeStainedGlass:
			case BlockType::PinkStainedGlass:
			case BlockType::PurpleStainedGlass:
			case BlockType::RedStainedGlass:
			case BlockType::YellowStainedGlass:
			case BlockType::WhiteStainedGlass:
			case BlockType::AcaciaFence:
			case BlockType::BirchFence:
			case BlockType::DarkOakFence:
			case BlockType::JungleFence:
			case BlockType::NetherBrickFence:
			case BlockType::OakFence:
			case BlockType::SpruceFence:
			case BlockType::WarpedFence:
			{
				// Torches can only be placed on top of these blocks
				return (a_BlockFace == BLOCK_FACE_YP);
			}
			default:
			{
				if (cBlockInfo::FullyOccupiesVoxel(a_Block))
				{
					// Torches can be placed on all sides of full blocks except the bottom
					return (a_BlockFace != BLOCK_FACE_YM);
				}
				return false;
			}
		}
	}

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		eBlockFace Face;
		switch (a_Self.Type())
		{
			case BlockType::Torch:             Face = eBlockFace::BLOCK_FACE_YP; break;
			case BlockType::WallTorch:         Face = Block::WallTorch::Facing(a_Self); break;
			case BlockType::RedstoneTorch:     Face = eBlockFace::BLOCK_FACE_YP; break;
			case BlockType::RedstoneWallTorch: Face = Block::RedstoneWallTorch::Facing(a_Self); break;
			case BlockType::SoulTorch:         Face = eBlockFace::BLOCK_FACE_YP; break;
			case BlockType::SoulWallTorch:     Face = Block::SoulWallTorch::Facing(a_Self); break;
			default: return false;
		}
		auto NeighborRelPos = AddFaceDirection(a_Position, Face, true);
		BlockState Neighbor;
		if (!a_Chunk.UnboundedRelGetBlock(NeighborRelPos, Neighbor))
		{
			// Neighbor in an unloaded chunk, bail out without changing this.
			return false;
		}

		return CanBePlacedOn(Neighbor, Face);
	}





	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool = nullptr) const override
	{
		UNUSED(a_Tool);
		auto BlockType = a_Block.Type();
		Item to_drop;
		switch (BlockType)
		{
			case BlockType::WallTorch: to_drop = Item::Torch; break;
			case BlockType::RedstoneWallTorch: to_drop = Item::RedstoneTorch; break;
			case BlockType::SoulWallTorch: to_drop = Item::SoulTorch; break;
			default: to_drop = BlockItemConverter::FromBlock(BlockType);
		}
		return cItems(to_drop);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
