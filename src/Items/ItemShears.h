
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemShearsHandler :
	public cItemHandler
{
public:
	cItemShearsHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}
	
	
	virtual bool IsTool(void) override
	{
		return true;
	}
	
	
	virtual bool OnDiggingBlock(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		switch (Block)
		{
			case E_BLOCK_LEAVES:
			case E_BLOCK_NEW_LEAVES:
			{
				cItems Drops;
				Drops.push_back(cItem(Block, 1, a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x03));
				a_World->SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);

				a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
				a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 0);
				a_Player->UseEquippedItem();
				return true;
			}
			case E_BLOCK_VINES:
			{
				cItems Drops;
				Drops.push_back(cItem(E_BLOCK_VINES, 1, a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x03));
				a_World->SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);

				a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
				a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 0);
				a_Player->UseEquippedItem();
				return true;
			}
			default:
			{
				return false;
			}
		}
	}


	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_COBWEB:
			case E_BLOCK_VINES:
			case E_BLOCK_NEW_LEAVES:
			case E_BLOCK_LEAVES:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}
	
	/** Get the Destroy Speed for the Block */
	virtual float GetDestroySpeed(BLOCKTYPE a_BlockType) override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_COBWEB:
			case E_BLOCK_NEW_LEAVES:
			case E_BLOCK_LEAVES:
			{
				return 15.0F;
			}
			case E_BLOCK_WOOL:
			{
				return 5.0F;
			}
			default:
			{
				return 1.0F;
			}
		}
	}
} ;




