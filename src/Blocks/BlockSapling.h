
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockSaplingHandler :
	public cBlockHandler
{
public:
	cBlockSaplingHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Only the first 2 bits contain the display information and the 4th bit is for the growth indicator, but, we use 0x07 for forward compatibility
		a_Pickups.push_back(cItem(E_BLOCK_SAPLING, 1, a_BlockMeta & 0x07));
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return (a_RelY > 0) && IsBlockTypeOfDirt(a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ));
	}
	

	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		
		if ((Meta & 0x08) != 0)
		{
			int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
			int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
			a_Chunk.GetWorld()->GrowTree(BlockX, a_RelY, BlockZ);
		}
		else
		{
			a_Chunk.SetMeta(a_RelX, a_RelY, a_RelZ, Meta | 0x08);
		}
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




