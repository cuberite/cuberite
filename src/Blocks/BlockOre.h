
#pragma once

#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"





class cBlockOreHandler :
	public cBlockHandler
{
public:
	cBlockOreHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		short ItemType = m_BlockType;
		char Count = 1;
		short Meta = 0;
		
		MTRand r1;
		switch (m_BlockType)
		{
			case E_BLOCK_LAPIS_ORE:
			{
				ItemType = E_ITEM_DYE;
				Count = 4 + (char)r1.randInt(4);
				Meta = 4;
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				Count = 4 + (char)r1.randInt(1);
				break;
			}
			default:
			{
				Count = 1;
				break;
			}
		}

		switch (m_BlockType)
		{
			case E_BLOCK_DIAMOND_ORE:
			{
				ItemType = E_ITEM_DIAMOND;
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				ItemType = E_ITEM_REDSTONE_DUST;
				break;
			}
			case E_BLOCK_NETHER_QUARTZ_ORE:
			{
				ItemType = E_ITEM_NETHER_QUARTZ;
				break;
			}
			case E_BLOCK_EMERALD_ORE:
			{
				ItemType = E_ITEM_EMERALD;
				break;
			}
			case E_BLOCK_COAL_ORE:
			{
				ItemType = E_ITEM_COAL;
				break;
			}
		}
		a_Pickups.push_back(cItem(ItemType, Count, Meta));
	}
	
	virtual int GetExpDrop(void) override
	{
		MTRand r1;
		switch (m_BlockType)
		{
			case E_BLOCK_COAL_ORE:
				return (char)r1.randInt(1) * 2;
			case E_BLOCK_DIAMOND_ORE:
				return (char)r1.randInt(1) * 4 + 3;
			case E_BLOCK_EMERALD_ORE:
				return (char)r1.randInt(1) * 4 + 3;
			case E_BLOCK_LAPIS_ORE:
				return (char)r1.randInt(1) * 3 + 2;
			case E_BLOCK_NETHER_QUARTZ_ORE:
				return (char)r1.randInt(1) * 3 + 2;
			default:
				return 0;
		}
	}
} ;




