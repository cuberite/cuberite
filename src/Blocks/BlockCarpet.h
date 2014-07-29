
// BlockCarpet.h

// Declares the cBlockCarpetHandler class representing the handler for the carpet block




#pragma once

#include "BlockHandler.h"





class cBlockCarpetHandler :
	public cBlockHandler
{
public:
	cBlockCarpetHandler(BLOCKTYPE a_BlockType) :
		cBlockHandler(a_BlockType)
	{
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = a_Player->GetEquippedItem().m_ItemDamage & 0x0f;
		return true;
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_CARPET, 1, a_BlockMeta));
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return (a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR);
	}
} ;




