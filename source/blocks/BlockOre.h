#pragma once
#include "Block.h"
#include "../MersenneTwister.h"
#include "../cWorld.h"

class cBlockOreHandler : public cBlockHandler
{
public:
	cBlockOreHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual char GetDropCount() override
	{
		MTRand r1;
		switch(m_BlockID)
		{
		case E_BLOCK_LAPIS_ORE:
			return 4 + (char)r1.randInt(4);
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
			return 4 + (char)r1.randInt(1);
		default:
			return 1;
		}
	}

	virtual NIBBLETYPE GetDropMeta(NIBBLETYPE a_Meta) override
	{
		switch(m_BlockID)
		{
		case E_BLOCK_LAPIS_ORE:
			return 4;
		default:
			return 0;
		}
	}

	virtual int GetDropID() override
	{
		switch(m_BlockID)
		{
		case E_BLOCK_DIAMOND_ORE:
			return E_ITEM_DIAMOND;
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
			return E_ITEM_REDSTONE_DUST;
		case E_BLOCK_EMERALD_ORE:
			return E_ITEM_EMERALD;
		case E_BLOCK_LAPIS_ORE:
			return E_ITEM_DYE;
		case E_BLOCK_COAL_ORE:
			return E_ITEM_COAL;
		}
		return m_BlockID;
	}	
};