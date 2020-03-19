#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"
#include "ChunkInterface.h"
#include "Mixins.h"




class cBlockTorchHandler :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x7, 0x4, 0x1, 0x3, 0x2>>
{
	using super = cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x7, 0x4, 0x1, 0x3, 0x2>>;

public:

	cBlockTorchHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (!IsValidDirection(a_BlockFace))
		{
			return false;
		}

		Vector3i Pos{ a_BlockX, a_BlockY, a_BlockZ };
		a_BlockType = m_BlockType;
		a_BlockMeta = DirectionToMetaData(a_BlockFace);
		bool CheckResult = a_Player.GetWorld()->DoWithChunkAt(Pos, [&](cChunk & a_Chunk)
		{
			auto RelPos = cChunkDef::AbsoluteToRelative(Pos);
			return CanBeAt(a_ChunkInterface, RelPos.x, RelPos.y, RelPos.z, a_Chunk, a_BlockMeta);
		});

		if (CheckResult)
		{
			return true;
		}
		else
		{
			a_BlockFace = FindSuitableFace(a_Player.GetWorld(), a_BlockX, a_BlockY, a_BlockZ);  // Set a_BlockFace to a valid direction which will be converted later to a metadata
			if (a_BlockFace == BLOCK_FACE_NONE)
			{
				// No attachable face found - don't place the torch
				return false;
			}
			else
			{
				a_BlockMeta = DirectionToMetaData(a_BlockFace);
				return true;
			}
		}
	}

	bool IsValidDirection(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case BLOCK_FACE_TOP:
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_WEST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
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
		}
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

	/** Finds a suitable face to place the torch, returning BLOCK_FACE_NONE on failure */
	eBlockFace FindSuitableFace(cWorld * World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		Vector3i Pos{ a_BlockX, a_BlockY, a_BlockZ };

		for (int i = BLOCK_FACE_YP; i <= BLOCK_FACE_XP; i++)  // Loop through all directions
		{
			eBlockFace Face = static_cast<eBlockFace>(i);

			NIBBLETYPE BlockMeta = DirectionToMetaData(Face);

			bool CheckResult = World->DoWithChunkAt(Pos, [&](cChunk & a_Chunk)
			{
				auto RelPos = cChunkDef::AbsoluteToRelative(Pos);
				cChunkInterface ChunkInterface(World->GetChunkMap());
				return CanBeAt(ChunkInterface, RelPos.x, RelPos.y, RelPos.z, a_Chunk, BlockMeta);
			});

			if (CheckResult)
			{
				return Face;
			}
		}
		return BLOCK_FACE_NONE;
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk, NIBBLETYPE a_BlockMeta) override
	{
		eBlockFace Face = MetaDataToDirection(a_BlockMeta);
		AddFaceDirection(a_RelX, a_RelY, a_RelZ, Face, true);

		BLOCKTYPE BlockInQuestion;
		NIBBLETYPE BlockInQuestionMeta;
		if (!a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ, BlockInQuestion, BlockInQuestionMeta))
		{
			return false;
		}

		switch (BlockInQuestion)
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
				return (Face == BLOCK_FACE_YP);
			}
			case E_BLOCK_STONE_SLAB:
			case E_BLOCK_WOODEN_SLAB:
			{
				// Toches can be placed on the top of these slabs only if the occupy the top half of the voxel
				return ((Face == BLOCK_FACE_YP) && ((BlockInQuestionMeta & 0x08) == 0x08));
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
				return (Face == BLOCK_FACE_TOP) && (BlockInQuestionMeta & E_BLOCK_STAIRS_UPSIDE_DOWN);
			}
			default:
			{
				if (cBlockInfo::IsFullSolidOpaqueBlock(BlockInQuestion))
				{
					// Torches can be placed on all sides of full blocks except the bottom
					return (Face != BLOCK_FACE_YM);
				}
				else
				{
					return false;
				}
			}
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




