
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"

#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"





class cItemShovelHandler : public cItemHandler
{
public:
	cItemShovelHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool OnDiggingBlock(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		if (Block == E_BLOCK_SNOW)
		{
			cChunkInterface ChunkInterface(a_World->GetChunkMap());
			cBlockInServerPluginInterface PluginInterface(*a_World);
			BlockHandler(Block)->DropBlock(ChunkInterface, *a_World, PluginInterface, a_Player, a_BlockX, a_BlockY, a_BlockZ);

			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			a_Player->UseEquippedItem();
			return true;
		}
		return false;
	}
	
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		return (a_BlockType == E_BLOCK_SNOW);
	}

	virtual bool CanRepairWithRawMaterial(short a_ItemType) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_SHOVEL:  return (a_ItemType == E_BLOCK_PLANKS);
			case E_ITEM_STONE_SHOVEL:   return (a_ItemType == E_BLOCK_COBBLESTONE);
			case E_ITEM_IRON_SHOVEL:    return (a_ItemType == E_ITEM_IRON);
			case E_ITEM_GOLD_SHOVEL:    return (a_ItemType == E_ITEM_GOLD);
			case E_ITEM_DIAMOND_SHOVEL: return (a_ItemType == E_ITEM_DIAMOND);
		}
		return false;
	}

};
