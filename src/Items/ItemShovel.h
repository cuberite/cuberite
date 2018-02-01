
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"


#include "../BlockInServerPluginInterface.h"





class cItemShovelHandler : public cItemHandler
{
	typedef cItemHandler super;
public:
	cItemShovelHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}



	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:      return 2;
			case dlaBreakBlock:        return 1;
			case dlaBreakBlockInstant: return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
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
			a_Player->UseEquippedItem(cItemHandler::dlaBreakBlock);
			return true;
		}
		return false;
	}

	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		if (a_BlockType == E_BLOCK_SNOW)
		{
			return true;
		}
		return super::CanHarvestBlock(a_BlockType);
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

	virtual float GetBlockBreakingStrength(BLOCKTYPE a_Block) override
	{
		switch (a_Block)
		{
			case E_BLOCK_CLAY:
			case E_BLOCK_CONCRETE_POWDER:
			case E_BLOCK_DIRT:
			case E_BLOCK_FARMLAND:
			case E_BLOCK_GRASS:
			case E_BLOCK_GRASS_PATH:
			case E_BLOCK_GRAVEL:
			case E_BLOCK_MYCELIUM:
			case E_BLOCK_SAND:
			case E_BLOCK_SNOW:
			case E_BLOCK_SNOW_BLOCK:
			case E_BLOCK_SOULSAND:
			{
				switch (m_ItemType)
				{
					case E_ITEM_WOODEN_SHOVEL:  return 2.0f;
					case E_ITEM_STONE_SHOVEL:   return 4.0f;
					case E_ITEM_IRON_SHOVEL:    return 6.0f;
					case E_ITEM_GOLD_SHOVEL:    return 12.0f;
					case E_ITEM_DIAMOND_SHOVEL: return 8.0f;
				}
				break;
			}
			default: return super::GetBlockBreakingStrength(a_Block);
		}
		ASSERT(!"Something is wrong here... Maybe they are shovels out of a new material?");
		return 1.0f;
	}
};
