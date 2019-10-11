
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"





/** Handles the grass that is 1 block tall */
class cBlockTallGrassHandler:
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockTallGrassHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual bool DoesIgnoreBuildCollision(cChunkInterface & a_ChunkInterface, Vector3i a_Pos, cPlayer & a_Player, NIBBLETYPE a_Meta) override
	{
		return true;
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// If using shears, drop self:
		if ((a_Tool != nullptr) && (a_Tool->m_ItemType == E_ITEM_SHEARS))
		{
			return cItem(m_BlockType, 1, a_BlockMeta);
		}

		// Drop seeds, sometimes:
		if (GetRandomProvider().RandBool(0.125))
		{
			return cItem(E_ITEM_SEEDS);
		}
		return {};
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY <= 0)
		{
			return false;
		}

		BLOCKTYPE BelowBlock = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		return IsBlockTypeOfDirt(BelowBlock);
	}





	/** Growing a tall grass produces a big flower (2-block high fern or double-tall grass). */
	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) override
	{
		if (a_RelPos.y > (cChunkDef::Height - 2))
		{
			return 0;
		}
		auto blockMeta = a_Chunk.GetMeta(a_RelPos);
		NIBBLETYPE largeFlowerMeta;
		switch (blockMeta)
		{
			case E_META_TALL_GRASS_GRASS: largeFlowerMeta = E_META_BIG_FLOWER_DOUBLE_TALL_GRASS; break;
			case E_META_TALL_GRASS_FERN:  largeFlowerMeta = E_META_BIG_FLOWER_LARGE_FERN; break;
			default:                      return 0;
		}
		a_Chunk.SetBlock(a_RelPos,           E_BLOCK_BIG_FLOWER, largeFlowerMeta);
		a_Chunk.SetBlock(a_RelPos.addedY(1), E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_TOP);
		return 1;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




