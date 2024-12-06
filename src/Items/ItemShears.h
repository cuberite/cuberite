
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Blocks/BlockWool.h"
#include "../Blocks/BlockLeaves.h"





class cItemShearsHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;





	virtual bool OnDiggingBlock(
		cWorld * a_World,
		cPlayer * a_Player,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
	{
		BlockState BrokenBlock;
		if (a_World->GetBlock(a_ClickedBlockPos, BrokenBlock))
		{
			return false;
		}
		if (cBlockLeavesHandler::IsBlockLeaves(BrokenBlock))
		{
			a_World->DropBlockAsPickups(a_ClickedBlockPos, a_Player, &a_HeldItem);
			return true;
		}

		return false;
	}





	virtual bool CanHarvestBlock(BlockState a_Block) const override
	{
		switch (a_Block.Type())
		{
			case BlockType::Cobweb:
			case BlockType::DeadBush:
			case BlockType::Vine:
				return true;
			default: return Super::CanHarvestBlock(a_Block);
		}
	}





	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) const override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 0;
			case dlaBreakBlock:         return 0;
			case dlaBreakBlockInstant:  return 1;
		}
		UNREACHABLE("Unsupported durability loss action");
		return 0;
	}





	virtual float GetBlockBreakingStrength(BlockState a_Block) const override
	{
		if ((a_Block == BlockType::Cobweb) || cBlockLeavesHandler::IsBlockLeaves(a_Block))
		{
			return 15.0f;
		}
		else if (cBlockWoolHandler::IsBlockWool(a_Block))
		{
			return 5.0f;
		}
		else
		{
			return Super::GetBlockBreakingStrength(a_Block);
		}
	}
} ;
