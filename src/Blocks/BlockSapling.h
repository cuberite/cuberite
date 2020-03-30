
#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"





class cBlockSaplingHandler :
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockSaplingHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// The low 3 bits store the sapling type; bit 0x08 is the growth timer (not used in pickups)
		return cItem(m_BlockType, 1, a_BlockMeta & 0x07);
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return (a_RelY > 0) && IsBlockTypeOfDirt(a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ));
	}





	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		// Only grow if we have the right amount of light
		if ((a_Chunk.GetLightAltered(Vector3i(a_RelX, a_RelY, a_RelZ)) > 8) && GetRandomProvider().RandBool(0.45))
		{
			Grow(a_Chunk, Vector3i(a_RelX, a_RelY, a_RelZ));
		}
	}





	virtual int Grow(cChunk & a_Chunk, Vector3i a_RelPos, int a_NumStages = 1) override
	{
		auto blockMeta = a_Chunk.GetMeta(a_RelPos);
		auto typeMeta = blockMeta & 0x07;
		auto growState = blockMeta >> 3;
		int res = 0;

		// Try to increase the sapling's growState:
		if (growState < 1)
		{
			++growState;
			a_Chunk.FastSetBlock(a_RelPos, m_BlockType, static_cast<NIBBLETYPE>(growState << 3 | typeMeta));
			if (a_NumStages == 1)
			{
				// Only asked to grow one stage, which we did. Bail out.
				return 1;
			}
			res = 1;
		}

		// The sapling is grown, now it becomes a tree:
		a_Chunk.GetWorld()->GrowTreeFromSapling(a_Chunk.RelativeToAbsolute(a_RelPos), blockMeta);
		return res + 1;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 7;
	}





private:

	bool IsLargeTree(cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_Meta)
	{
		BLOCKTYPE type;
		NIBBLETYPE meta;
		bool LargeTree = true;
		a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY, a_RelZ, type, meta);
		LargeTree = LargeTree && (type == E_BLOCK_SAPLING) && ((a_Meta & meta) == a_Meta);

		a_Chunk.UnboundedRelGetBlock(a_RelX + 1, a_RelY, a_RelZ + 1, type, meta);
		LargeTree = LargeTree && (type == E_BLOCK_SAPLING) && ((a_Meta & meta) == a_Meta);

		a_Chunk.UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ + 1, type, meta);
		LargeTree = LargeTree && (type == E_BLOCK_SAPLING) && ((a_Meta & meta) == a_Meta);

		return LargeTree;
	}
} ;




