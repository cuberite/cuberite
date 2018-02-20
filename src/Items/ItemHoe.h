
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemHoeHandler :
	public cItemHandler
{
public:
	cItemHoeHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if ((a_BlockFace == BLOCK_FACE_NONE) || (a_BlockY >= cChunkDef::Height))
		{
			return false;
		}
		BLOCKTYPE UpperBlock = a_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ);

		BLOCKTYPE Block;
		NIBBLETYPE BlockMeta;
		a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, Block, BlockMeta);

		if (((Block == E_BLOCK_DIRT) || (Block == E_BLOCK_GRASS)) && (UpperBlock == E_BLOCK_AIR))
		{
			BLOCKTYPE NewBlock = E_BLOCK_FARMLAND;
			if (Block == E_BLOCK_DIRT)
			{
				switch (BlockMeta)
				{
					case E_META_DIRT_COARSE:
					{
						// Transform to normal dirt
						NewBlock = E_BLOCK_DIRT;
						break;
					}
					case E_META_DIRT_PODZOL:
					{
						// You can't transform this block with a hoe in vanilla
						return false;
					}
					default: break;
				}
			}

			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, NewBlock, 0);
			a_World->BroadcastSoundEffect("item.hoe.till", {a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5}, 1.0f, 0.8f);
			a_Player->UseEquippedItem();
			return true;
		}

		return false;
	}


	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 1;
			case dlaBreakBlock:         return 0;
			case dlaBreakBlockInstant:  return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
	}
} ;
