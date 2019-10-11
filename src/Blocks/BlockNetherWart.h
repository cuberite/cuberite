
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





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) override
	{
		auto oldMeta = a_Chunk.GetMeta(a_RelPos);
		auto meta = std::min(oldMeta + a_NumStages, 3);
		if ((oldMeta < 3) && (CanGrow(a_Chunk, a_RelPos) == paGrowth))
		{
			a_Chunk.SetBlock(a_RelPos, m_BlockType, static_cast<NIBBLETYPE>(meta));
			return meta - oldMeta;
		}

		// In older versions of cuberite, there was a bug which made wart grow too much. This check fixes previously saved buggy warts.
		if (oldMeta > 3)
		{
			a_Chunk.FastSetBlock(a_RelPos, m_BlockType, 3);
		}
		return 0;
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
