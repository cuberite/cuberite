#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"
#include "BlockStairs.h"
#include "../Chunk.h"
#include "BlockType.h"
#include "ChunkInterface.h"
#include "Defines.h"
#include "Mixins.h"





class cBlockTorchBaseHandler :
	public cMetaRotator<cBlockHandler, 0x7, 0x4, 0x1, 0x3, 0x2>
{
	using Super = cMetaRotator<cBlockHandler, 0x7, 0x4, 0x1, 0x3, 0x2>;

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
	static bool CanBePlacedOn(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_BlockFace)
	{
		// upside down slabs
		if (cBlockSlabHandler::IsAnySlabType(a_BlockType))
		{
			return (a_BlockFace == BLOCK_FACE_YP) && (a_BlockMeta & E_META_WOODEN_SLAB_UPSIDE_DOWN);
		}

		// stairs (top and sides)
		if (cBlockStairsHandler::IsAnyStairType(a_BlockType))
		{
			switch (a_BlockFace)
			{
				case eBlockFace::BLOCK_FACE_YP:
					return (a_BlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN);
				case eBlockFace::BLOCK_FACE_XP:
					return ((a_BlockMeta & 0b11) == E_BLOCK_STAIRS_XP);
				case eBlockFace::BLOCK_FACE_XM:
					return ((a_BlockMeta & 0b11) == E_BLOCK_STAIRS_XM);
				case eBlockFace::BLOCK_FACE_ZP:
					return ((a_BlockMeta & 0b11) == E_BLOCK_STAIRS_ZP);
				case eBlockFace::BLOCK_FACE_ZM:
					return ((a_BlockMeta & 0b11) == E_BLOCK_STAIRS_ZM);
				default:
				{
					return false;
				}
			}
		}

		switch (a_BlockType)
		{
			case E_BLOCK_END_PORTAL_FRAME:
			case E_BLOCK_SOULSAND:
			{
				// Exceptional vanilla behaviour
				return true;
			}
			case E_BLOCK_GLASS:
			case E_BLOCK_STAINED_GLASS:
			case E_BLOCK_FENCE:
			case E_BLOCK_NETHER_BRICK_FENCE:
			case E_BLOCK_SPRUCE_FENCE:
			case E_BLOCK_BIRCH_FENCE:
			case E_BLOCK_JUNGLE_FENCE:
			case E_BLOCK_DARK_OAK_FENCE:
			case E_BLOCK_ACACIA_FENCE:
			case E_BLOCK_COBBLESTONE_WALL:
			{
				// Torches can only be placed on top of these blocks
				return (a_BlockFace == BLOCK_FACE_YP);
			}
			default:
			{
				if (cBlockInfo::FullyOccupiesVoxel(a_BlockType))
				{
					// Torches can be placed on all sides of full blocks except the bottom
					return (a_BlockFace != BLOCK_FACE_YM);
				}
				return false;
			}
		}
	}

protected:

	~cBlockTorchBaseHandler() = default;

private:

	/** Converts the torch block's meta to the block face of the neighbor to which the torch is attached. */
	constexpr static eBlockFace MetaDataToBlockFace(NIBBLETYPE a_MetaData)
	{
		switch (a_MetaData)
		{
			case 0:                  return BLOCK_FACE_TOP;  // By default, the torches stand on the ground
			case E_META_TORCH_FLOOR: return BLOCK_FACE_TOP;
			case E_META_TORCH_EAST:  return BLOCK_FACE_EAST;
			case E_META_TORCH_WEST:  return BLOCK_FACE_WEST;
			case E_META_TORCH_NORTH: return BLOCK_FACE_NORTH;
			case E_META_TORCH_SOUTH: return BLOCK_FACE_SOUTH;
			default:
			{
				ASSERT(!"Unhandled torch metadata");
				break;
			}
		}
		return BLOCK_FACE_TOP;
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		auto Face = MetaDataToBlockFace(a_Meta);
		auto NeighborRelPos = AddFaceDirection(a_Position, Face, true);
		BLOCKTYPE NeighborBlockType;
		NIBBLETYPE NeighborBlockMeta;
		if (!a_Chunk.UnboundedRelGetBlock(NeighborRelPos, NeighborBlockType, NeighborBlockMeta))
		{
			// Neighbor in an unloaded chunk, bail out without changint this.
			return false;
		}

		return CanBePlacedOn(NeighborBlockType, NeighborBlockMeta, Face);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;





class cBlockTorchHandler final :
	public cClearMetaOnDrop<cBlockTorchBaseHandler>
{
	using Super = cClearMetaOnDrop<cBlockTorchBaseHandler>;

public:

	using Super::Super;
};





class cBlockRedstoneTorchHandler final :
	public cBlockTorchBaseHandler
{
	using Super = cBlockTorchBaseHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Always drop the ON torch, meta 0:
		return { E_BLOCK_REDSTONE_TORCH_ON };
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
};
