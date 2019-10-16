
#pragma once

#include "BlockPlant.h"
#include "../FastRandom.h"





class cBlockNetherWartHandler :
	public cBlockPlant<false>
{
	using super = cBlockPlant<false>;

public:

	cBlockNetherWartHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		if (a_BlockMeta == 0x03)
		{
			// Fully grown, drop the entire produce:
			auto & rand = GetRandomProvider();
			return cItem(E_ITEM_NETHER_WART, 1 + (rand.RandInt<char>(2) + rand.RandInt<char>(2)) / 2, 0);
		}
		else
		{
			return cItem(E_ITEM_NETHER_WART);
		}
	}





	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		if ((Meta < 3) && (CanGrow(a_Chunk, a_RelX, a_RelY, a_RelZ) == paGrowth))
		{
			a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_NETHER_WART, ++Meta);
		}
		else if (Meta > 3)  // In older versions of cuberite, there was a bug which made wart grow too much. This check fixes previously saved buggy warts.
		{
			a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_NETHER_WART, 3);
		}
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		// Needs to be placed on top of a Soulsand block:
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_SOULSAND));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 35;
	}
} ;
