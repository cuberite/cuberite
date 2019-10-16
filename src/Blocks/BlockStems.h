
#pragma once

#include "BlockPlant.h"





class cBlockStemsHandler :
	public cBlockPlant<true>
{
	using super = cBlockPlant<true>;

public:

	cBlockStemsHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		auto itemType = (m_BlockType == E_BLOCK_MELON_STEM) ? E_ITEM_MELON_SEEDS : E_ITEM_PUMPKIN_SEEDS;
		return cItem(itemType, 1, 0);
	}





	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		auto Action = CanGrow(a_Chunk, a_RelX, a_RelY, a_RelZ);
		if (Action == paGrowth)
		{
			NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
			if (Meta >= 7)
			{
				// Grow the produce:
				int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
				int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
				a_Chunk.GetWorld()->GrowMelonPumpkin(BlockX, a_RelY, BlockZ, m_BlockType);
			}
			else
			{
				// Grow the stem:
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, m_BlockType, Meta + 1);
			}
		}
		else if (Action == paDeath)
		{
			a_Chunk.GetWorld()->DigBlock(a_RelX + a_Chunk.GetPosX() * cChunkDef::Width, a_RelY, a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width);
		}
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_FARMLAND));
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




