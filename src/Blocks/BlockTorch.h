#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"
#include "ChunkInterface.h"
#include "MetaRotator.h"




class cBlockTorchHandler :
	public cMetaRotator<cBlockHandler, 0x7, 0x4, 0x1, 0x3, 0x2>
{
public:
	cBlockTorchHandler(BLOCKTYPE a_BlockType)
		: cMetaRotator<cBlockHandler, 0x7, 0x4, 0x1, 0x3, 0x2>(a_BlockType)
	{
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		BLOCKTYPE Block;
		NIBBLETYPE Meta;
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);  // Set to clicked block
		a_ChunkInterface.GetBlockTypeMeta({a_BlockX, a_BlockY, a_BlockZ}, Block, Meta);

		if (!CanBePlacedOn(Block, Meta, a_BlockFace))  // Try to preserve original direction
		{
			// Torch couldn't be placed on whatever face was clicked, last ditch resort - find another face

			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, false);  // Reset to torch block
			a_BlockFace = FindSuitableFace(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);  // Set a_BlockFace to a valid direction which will be converted later to a metadata
			if (a_BlockFace == BLOCK_FACE_NONE)
			{
				// No attachable face found - don't place the torch
				return false;
			}
		}

		a_BlockType = m_BlockType;
		a_BlockMeta = DirectionToMetaData(a_BlockFace);
		return true;
	}

	inline static NIBBLETYPE DirectionToMetaData(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case BLOCK_FACE_BOTTOM: ASSERT(!"Shouldn't be getting this face"); return 0;
			case BLOCK_FACE_TOP:    return E_META_TORCH_FLOOR;
			case BLOCK_FACE_EAST:   return E_META_TORCH_EAST;
			case BLOCK_FACE_WEST:   return E_META_TORCH_WEST;
			case BLOCK_FACE_NORTH:  return E_META_TORCH_NORTH;
			case BLOCK_FACE_SOUTH:  return E_META_TORCH_SOUTH;
			case BLOCK_FACE_NONE:
			{
				ASSERT(!"Unhandled torch direction!");
				break;
			}
		};
		return 0x0;
	}

	inline static eBlockFace MetaDataToDirection(NIBBLETYPE a_MetaData)
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

	static bool CanBePlacedOn(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_BlockFace)
	{
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
			case E_BLOCK_STONE_SLAB:
			case E_BLOCK_WOODEN_SLAB:
			{
				// Toches can be placed on the top of these slabs only if the occupy the top half of the voxel
				return ((a_BlockFace == BLOCK_FACE_YP) && ((a_BlockMeta & 0x08) == 0x08));
			}
			case E_BLOCK_OAK_WOOD_STAIRS:
			case E_BLOCK_COBBLESTONE_STAIRS:
			case E_BLOCK_BRICK_STAIRS:
			case E_BLOCK_STONE_BRICK_STAIRS:
			case E_BLOCK_NETHER_BRICK_STAIRS:
			case E_BLOCK_SANDSTONE_STAIRS:
			case E_BLOCK_SPRUCE_WOOD_STAIRS:
			case E_BLOCK_BIRCH_WOOD_STAIRS:
			case E_BLOCK_JUNGLE_WOOD_STAIRS:
			case E_BLOCK_QUARTZ_STAIRS:
			case E_BLOCK_ACACIA_WOOD_STAIRS:
			case E_BLOCK_DARK_OAK_WOOD_STAIRS:
			case E_BLOCK_RED_SANDSTONE_STAIRS:
			{
				return (a_BlockFace == BLOCK_FACE_TOP) && (a_BlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN);
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

	/** Finds a suitable face to place the torch, returning BLOCK_FACE_NONE on failure */
	static eBlockFace FindSuitableFace(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		for (int i = BLOCK_FACE_YM; i <= BLOCK_FACE_XP; i++)  // Loop through all directions
		{
			eBlockFace Face = static_cast<eBlockFace>(i);
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, Face, true);
			BLOCKTYPE BlockInQuestion;
			NIBBLETYPE BlockInQuestionMeta;
			a_ChunkInterface.GetBlockTypeMeta({a_BlockX, a_BlockY, a_BlockZ}, BlockInQuestion, BlockInQuestionMeta);

			if (CanBePlacedOn(BlockInQuestion, BlockInQuestionMeta, Face))
			{
				return Face;
			}
			else
			{
				// Reset coords in preparation for next iteration
				AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, Face, false);
			}
		}
		return BLOCK_FACE_NONE;
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		eBlockFace Face = MetaDataToDirection(a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ));
		AddFaceDirection(a_RelX, a_RelY, a_RelZ, Face, true);

		BLOCKTYPE BlockInQuestion;
		NIBBLETYPE BlockInQuestionMeta;
		if (!a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ, BlockInQuestion, BlockInQuestionMeta))
		{
			return false;
		}

		return CanBePlacedOn(BlockInQuestion, BlockInQuestionMeta, Face);
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Always drop meta = 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




