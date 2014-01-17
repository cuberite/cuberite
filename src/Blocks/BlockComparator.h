
#pragma once

#include "BlockHandler.h"
#include "BlockRedstoneRepeater.h"





class cBlockComparatorHandler :
	public cBlockHandler
{
public:
	cBlockComparatorHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		Meta ^= 0x04; // Toggle 3rd (addition/subtraction) bit with XOR
		a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta);
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_ITEM_COMPARATOR, 1, 0));
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
	
	
	virtual bool CanBeAt(int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) != E_BLOCK_AIR));
	}
	

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
		) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = cBlockRedstoneRepeaterHandler::RepeaterRotationToMetaData(a_Player->GetRot().x);
		return true;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




