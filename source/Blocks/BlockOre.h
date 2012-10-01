
#pragma once

#include "BlockHandler.h"
#include "../MersenneTwister.h"
#include "../World.h"





class cBlockOreHandler :
	public cBlockHandler
{
public:
	cBlockOreHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		short ItemType = E_ITEM_EMPTY;
		char Count = 1;
		short Meta = 0;
		
		MTRand r1;
		switch (m_BlockID)
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

		switch (m_BlockID)
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
} ;




