#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"
#include "ChunkInterface.h"
#include "Mixins.h"





class cBlockTorchBaseHandler :
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

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) const override
	{
		auto ClickedBlockPos = AddFaceDirection(a_PlacedBlockPos, a_ClickedBlockFace, true);
		auto ClickedBlock = a_ChunkInterface.GetBlock(ClickedBlockPos);
		if (!CanBePlacedOn(ClickedBlock, a_ClickedBlockFace))
		{
			// Couldn't be placed on whatever face was clicked, last ditch resort - find another face
			a_ClickedBlockFace = FindSuitableFace(a_ChunkInterface, a_PlacedBlockPos);  // Set a_BlockFace to a valid direction which will be converted later to a metadata
			if (a_ClickedBlockFace == BLOCK_FACE_NONE)
			{
				// No attachable face found - don't place the torch
				return false;
			}
		}
		switch (m_BlockType)
		{
			case BlockType::Torch:
			case BlockType::WallTorch:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Block = Block::Torch::Torch();
						return true;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Block = Block::WallTorch::WallTorch(a_ClickedBlockFace);
						return true;
					}
					case BLOCK_FACE_NONE:
					case BLOCK_FACE_YM:
					{
						return false;
					}
				}
			}
			case BlockType::RedstoneTorch:
			case BlockType::RedstoneWallTorch:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Block = Block::RedstoneTorch::RedstoneTorch();
						return true;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Block = Block::RedstoneWallTorch::RedstoneWallTorch(a_ClickedBlockFace, true);
						return true;
					}
					case BLOCK_FACE_NONE:
					case BLOCK_FACE_YM:
					{
						return false;
					}
				}
			}
			case BlockType::SoulTorch:
			case BlockType::SoulWallTorch:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Block = Block::SoulTorch::SoulTorch();
						return true;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Block = Block::SoulWallTorch::SoulWallTorch(a_ClickedBlockFace);
						return true;
					}
					case BLOCK_FACE_NONE:
					case BLOCK_FACE_YM:
					{
						return false;
					}
				}
			}
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





	/** Returns a suitable neighbor's blockface to place the torch at the specified pos
	Returns BLOCK_FACE_NONE on failure */
	static eBlockFace FindSuitableFace(cChunkInterface & a_ChunkInterface, const Vector3i a_TorchPos)
	{
		for (int i = BLOCK_FACE_YM; i <= BLOCK_FACE_XP; i++)  // Loop through all faces
		{
			auto Face = static_cast<eBlockFace>(i);
			auto NeighborPos = AddFaceDirection(a_TorchPos, Face, true);
			auto Neighbor = a_ChunkInterface.GetBlock(NeighborPos);
			if (CanBePlacedOn(Neighbor, Face))
			{
				return Face;
			}
		}
		return BLOCK_FACE_NONE;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		eBlockFace Face;
		auto Self = a_Chunk.GetBlock(a_RelPos);
		switch (Self.Type())
		{
			case BlockType::Torch:             Face = eBlockFace::BLOCK_FACE_YP; break;
			case BlockType::WallTorch:         Face = Block::WallTorch::Facing(Self); break;
			case BlockType::RedstoneTorch:     Face = eBlockFace::BLOCK_FACE_YP; break;
			case BlockType::RedstoneWallTorch: Face = Block::RedstoneWallTorch::Facing(Self); break;
			case BlockType::SoulTorch:         Face = eBlockFace::BLOCK_FACE_YP; break;
			case BlockType::SoulWallTorch:     Face = Block::SoulWallTorch::Facing(Self); break;
			default: return false;
		}
		auto NeighborRelPos = AddFaceDirection(a_RelPos, Face, true);
		BlockState Neighbor;
		if (!a_Chunk.UnboundedRelGetBlock(NeighborRelPos, Neighbor))
		{
			// Neighbor in an unloaded chunk, bail out without changint this.
			return false;
		}

		return CanBePlacedOn(Neighbor, Face);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;
