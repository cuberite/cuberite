
#pragma once

#include "BlockHandler.h"





class cBlockPlanksHandler : public cBlockHandler
{
public:
	cBlockPlanksHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = a_Player->GetEquippedItem().m_ItemDamage;
		a_BlockMeta = Meta;
		return true;
	}

	
	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




