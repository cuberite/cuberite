
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"

#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"





class cItemAxeHandler : public cItemHandler
{
public:
	cItemAxeHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	float AxePower()
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_AXE:
				return 2.0F;
			case E_ITEM_STONE_AXE:
				return 4.0F;
			case E_ITEM_IRON_AXE:
				return 6.0F;
			case E_ITEM_DIAMOND_AXE:
				return 8.0F;
			case E_ITEM_GOLD_AXE:
				return 12.0F;
			default:
				return 2.0F;
		}
	}
	
	virtual float GetDestroySpeed(BLOCKTYPE a_BlockType) override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_PLANKS:
			case E_BLOCK_BOOKCASE:
			case E_BLOCK_LOG:
			case E_BLOCK_NEW_LOG:
			case E_BLOCK_CHEST:
			case E_BLOCK_PUMPKIN:
			case E_BLOCK_JACK_O_LANTERN:
				return AxePower();
			default:
				return 1.0F;
		}
	}
};
