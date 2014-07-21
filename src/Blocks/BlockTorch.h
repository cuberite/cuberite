#pragma once

#include "BlockHandler.h"
#include "../Chunk.h"
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
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// Find proper placement of torch

		if ((a_BlockFace == BLOCK_FACE_TOP) || (a_BlockFace == BLOCK_FACE_BOTTOM))
		{
			a_BlockFace = FindSuitableFace(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);  // Top or bottom faces clicked, find a suitable face
			if (a_BlockFace == BLOCK_FACE_NONE)
			{
				// Client wouldn't have sent anything anyway, but whatever
				return false;
			}
		}
		else
		{
			// Not top or bottom faces, try to preserve whatever face was clicked
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);  // Set to clicked block
			if (!CanBePlacedOn(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ), a_BlockFace))
			{
				AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, false);  // Reset to torch block
				// Torch couldn't be placed on whatever face was clicked, last ditch resort - find another face
				a_BlockFace = FindSuitableFace(a_ChunkInterface, a_BlockX, a_BlockY, a_BlockZ);
				if (a_BlockFace == BLOCK_FACE_NONE)
				{
					return false;
				}
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
			default:
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


	static bool CanBePlacedOn(BLOCKTYPE a_BlockType, eBlockFace a_BlockFace)
	{
		if (!cBlockInfo::FullyOccupiesVoxel(a_BlockType))
		{
			return (a_BlockFace == BLOCK_FACE_TOP);  // Allow placement only when torch upright (for glass, etc.); exceptions won't even be sent by client, no need to handle
		}
		else
		{
			return true;
		}
	}
	
	
	/// Finds a suitable face to place the torch, returning BLOCK_FACE_NONE on failure
	static eBlockFace FindSuitableFace(cChunkInterface & a_ChunkInterface, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		for (int i = BLOCK_FACE_YM; i <= BLOCK_FACE_XP; i++)  // Loop through all directions
		{
			eBlockFace Face = static_cast<eBlockFace>(i);
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, Face, true);
			BLOCKTYPE BlockInQuestion = a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ);

			// If on a block that can only hold a torch if torch is standing on it, return that face
			if (
				((BlockInQuestion == E_BLOCK_GLASS) ||
				(BlockInQuestion == E_BLOCK_FENCE) ||
				(BlockInQuestion == E_BLOCK_NETHER_BRICK_FENCE) ||
				(BlockInQuestion == E_BLOCK_COBBLESTONE_WALL)) &&
				(Face == BLOCK_FACE_TOP)
				)
			{
				return Face;
			}
			else if (cBlockInfo::FullyOccupiesVoxel(BlockInQuestion) && (i != BLOCK_FACE_BOTTOM))
			{
				// Otherwise, if block in that direction is torch placeable and we haven't gotten to it via the bottom face, return that face
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
		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockInQuestion);

		if (
			(BlockInQuestion == E_BLOCK_GLASS) ||
			(BlockInQuestion == E_BLOCK_STAINED_GLASS) ||
			(BlockInQuestion == E_BLOCK_FENCE) ||
			(BlockInQuestion == E_BLOCK_SOULSAND) ||
			(BlockInQuestion == E_BLOCK_MOB_SPAWNER) ||
			(BlockInQuestion == E_BLOCK_END_PORTAL_FRAME) ||  // Actual vanilla behaviour
			(BlockInQuestion == E_BLOCK_NETHER_BRICK_FENCE) ||
			(BlockInQuestion == E_BLOCK_COBBLESTONE_WALL)
			)
		{
			// Torches can be placed on tops of glass and fences, despite them being 'untorcheable'
			// No need to check for upright orientation, it was done when the torch was placed
			return true;
		}
		else if (!cBlockInfo::FullyOccupiesVoxel(BlockInQuestion))
		{
			return false;
		}
		else
		{
			return true;
		}
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Always drop meta = 0
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




