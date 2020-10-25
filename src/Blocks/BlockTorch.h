#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"
#include "ChunkInterface.h"
#include "Mixins.h"





class cBlockTorchBaseHandler :
	public cMetaRotator<cBlockHandler, 0x7, 0x4, 0x1, 0x3, 0x2>
{
	using Super = cMetaRotator<cBlockHandler, 0x7, 0x4, 0x1, 0x3, 0x2>;

public:

	using Super::Super;

protected:

	~cBlockTorchBaseHandler() = default;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		BLOCKTYPE ClickedBlockType;
		NIBBLETYPE ClickedBlockMeta;
		auto ClickedBlockPos = AddFaceDirection(a_PlacedBlockPos, a_ClickedBlockFace, true);
		a_ChunkInterface.GetBlockTypeMeta(ClickedBlockPos, ClickedBlockType, ClickedBlockMeta);
		if (!CanBePlacedOn(ClickedBlockType, ClickedBlockMeta, a_ClickedBlockFace))
		{
			// Couldn't be placed on whatever face was clicked, last ditch resort - find another face
			a_ClickedBlockFace = FindSuitableFace(a_ChunkInterface, a_PlacedBlockPos);  // Set a_BlockFace to a valid direction which will be converted later to a metadata
			if (a_ClickedBlockFace == BLOCK_FACE_NONE)
			{
				// No attachable face found - don't place the torch
				return false;
			}
		}
		a_BlockType = m_BlockType;
		a_BlockMeta = BlockFaceToMetaData(a_ClickedBlockFace);
		return true;
	}





	/** Converts the block face of the neighbor to which the torch is attached, to the torch block's meta. */
	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_NeighborBlockFace)
	{
		switch (a_NeighborBlockFace)
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





	/** Converts the torch block's meta to the block face of the neighbor to which the torch is attached. */
	inline static eBlockFace MetaDataToBlockFace(NIBBLETYPE a_MetaData)
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





	/** Returns true if the torch can be placed on the specified block's face. */
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
				// Toches can be placed only on the top of top-half-slabs
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





	/** Returns a suitable neighbor's blockface to place the torch at the specified pos
	Returns BLOCK_FACE_NONE on failure */
	static eBlockFace FindSuitableFace(cChunkInterface & a_ChunkInterface, const Vector3i a_TorchPos)
	{
		for (int i = BLOCK_FACE_YM; i <= BLOCK_FACE_XP; i++)  // Loop through all faces
		{
			auto Face = static_cast<eBlockFace>(i);
			auto NeighborPos = AddFaceDirection(a_TorchPos, Face, true);
			BLOCKTYPE NeighborBlockType;
			NIBBLETYPE NeighborBlockMeta;
			a_ChunkInterface.GetBlockTypeMeta(NeighborPos, NeighborBlockType, NeighborBlockMeta);
			if (CanBePlacedOn(NeighborBlockType, NeighborBlockMeta, Face))
			{
				return Face;
			}
		}
		return BLOCK_FACE_NONE;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		auto Face = MetaDataToBlockFace(a_Chunk.GetMeta(a_RelPos));
		auto NeighborRelPos = AddFaceDirection(a_RelPos, Face, true);
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

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
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
