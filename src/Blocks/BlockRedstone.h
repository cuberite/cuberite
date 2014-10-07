
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockRedstoneHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	

	inline static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x7)
		{
			case 0x1: return BLOCK_FACE_XP;
			case 0x2: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_ZM;
			case 0x5:
			case 0x6: return BLOCK_FACE_YP;
			case 0x7:
			case 0x0: return BLOCK_FACE_YM;
			default:
			{
				ASSERT(!"Unhandled block meta!");
				return BLOCK_FACE_NONE;
			}
		}
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if ((a_RelY <= 0) && (a_RelY >= cChunkDef::Height))
		{
			return false;
		}

		NIBBLETYPE Meta;
		if (!a_Chunk.UnboundedRelGetBlockMeta(a_RelX, a_RelY, a_RelZ, Meta))
		{
			return false;
		}
		eBlockFace Face = BlockMetaDataToBlockFace(Meta);

		AddFaceDirection(a_RelX, a_RelY, a_RelZ, Face, true);
		BLOCKTYPE BlockIsOn;
		a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockIsOn);

		if (
			(
				((BlockIsOn == E_BLOCK_WOODEN_SLAB) && ((Meta & 0x08) == 0x08)) ||
				((BlockIsOn == E_BLOCK_STONE_SLAB) && ((Meta & 0x08) == 0x08))
			) &&
			((a_RelY < cChunkDef::Height) && (Face == BLOCK_FACE_TOP))
		)
		{
			return true;
		}
		else
		{
			return ((a_RelY > 0) && cBlockInfo::FullyOccupiesVoxel(a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ)));
		}
	}
	
	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_DUST, 1, 0));
	}
} ;




