
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemShearsHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemShearsHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool IsTool(void) override
	{
		return true;
	}





	virtual bool OnDiggingBlock(
		cWorld * a_World,
		cPlayer * a_Player,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		BLOCKTYPE Block;
		NIBBLETYPE BlockMeta;
		a_World->GetBlockTypeMeta(a_ClickedBlockPos, Block, BlockMeta);

		if ((Block == E_BLOCK_LEAVES) || (Block == E_BLOCK_NEW_LEAVES))
		{
			a_World->DropBlockAsPickups(a_ClickedBlockPos, a_Player, &a_HeldItem);
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
		return Super::CanHarvestBlock(a_BlockType);
	}





	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 0;
			case dlaBreakBlock:         return 0;
			case dlaBreakBlockInstant:  return 1;
		}
		UNREACHABLE("Unsupported durability loss action");
	}





	virtual float GetBlockBreakingStrength(BLOCKTYPE a_Block) override
	{
		if ((a_Block == E_BLOCK_COBWEB) || IsBlockMaterialLeaves(a_Block))
		{
			return 15.0f;
		}
		else if (a_Block == E_BLOCK_WOOL)
		{
			return 5.0f;
		}
		else
		{
			return Super::GetBlockBreakingStrength(a_Block);
		}
	}
} ;
