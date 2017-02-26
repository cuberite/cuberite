
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
		BLOCKTYPE Block;
		NIBBLETYPE BlockMeta;
		a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, Block, BlockMeta);

		if ((Block == E_BLOCK_LEAVES) || (Block == E_BLOCK_NEW_LEAVES))
		{
			cItems Drops;
			cBlockHandler * Handler = cBlockInfo::GetHandler(Block);

			Handler->ConvertToPickups(Drops, BlockMeta);
			Drops.Add(Block, 1, BlockMeta & 3);
			a_World->SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);

			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			a_World->SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);
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
			case E_BLOCK_DEAD_BUSH:
			case E_BLOCK_VINES:
			{
				return true;
			}
		}
		return super::CanHarvestBlock(a_BlockType);
	}


	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		return 0;
	}


	virtual void OnBlockDestroyed(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		BLOCKTYPE Block;
		NIBBLETYPE BlockMeta;
		a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, Block, BlockMeta);

		super::OnBlockDestroyed(a_World, a_Player, a_Item, a_BlockX, a_BlockY, a_BlockZ);
		switch (Block)
		{
			case E_BLOCK_COBWEB:
			case E_BLOCK_DEAD_BUSH:
			case E_BLOCK_TRIPWIRE:
			case E_BLOCK_VINES:
			case E_BLOCK_WOOL:
			{
				a_Player->UseEquippedItem();
			}
		}
	}



	virtual float GetBlockBreakingStrength(BLOCKTYPE a_Block) override
	{
		if (IsBlocksWeb(a_Block) || IsBlockMaterialLeaves(a_Block))
		{
			return 15.0f;
		}
		else
		{
			if (IsBlocksWool(a_Block))
			{
				return 5.0f;
			}
			else
			{
				return super::GetBlockBreakingStrength(a_Block);
			}
		}
	}

} ;




