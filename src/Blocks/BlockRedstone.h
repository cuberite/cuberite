
#pragma once

#include "BlockHandler.h"
#include "BlockSlab.h"





class cBlockRedstoneHandler :
	public cBlockHandler
{
public:
	cBlockRedstoneHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk, NIBBLETYPE a_BlockMeta) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock;
		NIBBLETYPE BelowBlockMeta;
		a_Chunk.GetBlockTypeMeta(a_RelX, a_RelY - 1, a_RelZ, BelowBlock, BelowBlockMeta);

		// Redstone dust can be placed on opaque blocks as well as glowstone,
		// upside-down slabs and upside-down stairs, and hoppers
		switch (BelowBlock)
		{
			case E_BLOCK_HOPPER:
			case E_BLOCK_GLOWSTONE:
			{
				return true;
			}
			case E_BLOCK_TNT:
			{
				return false;
			}
			default:
			{
				// On the top of an upside-down slab
				if (cBlockSlabHandler::IsAnySlabType(BelowBlock))
				{
					// Check if the slab is turned up side down
					return (cBlockSlabHandler::IsUpsideDown(BelowBlockMeta));
				}

				// On the top of an upside-down stairs
				if (cBlockStairsHandler::IsAnyStairType(BelowBlock))
				{
					return (cBlockStairsHandler::IsUpsideDown(BelowBlockMeta));
				}

				return cBlockInfo::IsFullSolidOpaqueBlock(BelowBlock);
			}
		}
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(E_ITEM_REDSTONE_DUST, 1, 0));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




