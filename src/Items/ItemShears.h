
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemShearsHandler :
	public cItemHandler
{
	typedef cItemHandler super;
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
		if ((Block == E_BLOCK_LEAVES) || (Block == E_BLOCK_NEW_LEAVES))
		{
			NIBBLETYPE Meta = a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
			cBlockHandler * Handler = cBlockInfo::GetHandler(Block);

			cItems Drops;
			Handler->ConvertToPickups(Drops, Meta);
			Drops.push_back(cItem(Block, 1, Meta & 3));
			a_World->SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);

			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			a_Player->UseEquippedItem();
			return true;
		}
		return false;
	}


	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_COBWEB:
			case E_BLOCK_VINES:
			case E_BLOCK_LEAVES:
			case E_BLOCK_NEW_LEAVES:
			{
				return true;
			}
		}  // switch (a_BlockType)
		return super::CanHarvestBlock(a_BlockType);
	}


	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		return 0;
	}


	virtual void OnBlockDestroyed(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		super::OnBlockDestroyed(a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ);

		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		if ((Block == E_BLOCK_TRIPWIRE) || (Block == E_BLOCK_VINES))
		{
			a_Player->UseEquippedItem();
		}
	}
} ;




