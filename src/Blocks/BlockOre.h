
#pragma once

#include "BlockHandler.h"
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
		cFastRandom Random;

		switch (m_BlockType)
		{
			case E_BLOCK_LAPIS_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_DYE, (char)(4 + Random.NextInt(5)), 4));
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				a_Pickups.push_back(cItem(E_ITEM_REDSTONE_DUST, (char)(4 + Random.NextInt(2)), 0));
				break;
			}
			case E_BLOCK_DIAMOND_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_DIAMOND));
				break;
			}
			case E_BLOCK_EMERALD_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_EMERALD));
				break;
			}
			case E_BLOCK_COAL_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_COAL));
				break;
			}
			default:
			{
				a_Pickups.push_back(cItem(m_BlockType));
				break;
			}
		}
	}
} ;




