
#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"





class cBlockRedstoneHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	cBlockRedstoneHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) override
	{
		if (!cChunkDef::IsValidHeight(a_RelPos.y)) return false;

		Vector3i BelowBlockPos = a_RelPos.addedY(-1);
		BLOCKTYPE BelowBlock;
		NIBBLETYPE BelowBlockMeta;
		if (!a_Chunk.UnboundedRelGetBlock(BelowBlockPos, BelowBlock, BelowBlockMeta)) return false;

		if (cBlockInfo::FullyOccupiesVoxel(BelowBlock))
		{
			return true;
		}
		else if (cBlockSlabHandler::IsAnySlabType(BelowBlock))
		{
			// Check if the slab is turned up side down
			if ((BelowBlockMeta & 0x08) == 0x08)
			{
				return true;
			}
		}
		return false;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		return cItem(E_ITEM_REDSTONE_DUST, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




