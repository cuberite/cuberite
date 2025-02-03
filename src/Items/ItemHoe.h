
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemHoeHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
	{
		if ((a_ClickedBlockFace == BLOCK_FACE_NONE) || (a_ClickedBlockPos.y >= cChunkDef::Height))
		{
			return false;
		}

		// Need air above the hoe-d block to transform it:
		BLOCKTYPE UpperBlockType = a_World->GetBlock(a_ClickedBlockPos.addedY(1));
		if (UpperBlockType != E_BLOCK_AIR)
		{
			return false;
		}

		// Can only transform dirt or grass blocks:
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		if (!a_World->GetBlockTypeMeta(a_ClickedBlockPos, BlockType, BlockMeta))
		{
			return false;
		}
		if ((BlockType != E_BLOCK_DIRT) && (BlockType != E_BLOCK_GRASS))
		{
			return false;
		}
		if ((BlockType == E_BLOCK_DIRT) && (BlockMeta == E_META_DIRT_PODZOL))
		{
			return false;
		}

		// Transform:
		auto NewBlockType = ((BlockType == E_BLOCK_DIRT) && (BlockMeta == E_META_DIRT_COARSE)) ? E_BLOCK_DIRT : E_BLOCK_FARMLAND;
		a_World->SetBlock(a_ClickedBlockPos, NewBlockType, 0);
		a_World->BroadcastSoundEffect(eSoundEvent::ItemHoeTill, a_ClickedBlockPos + Vector3d(0.5, 0.5, 0.5), 1.0f, 0.8f);
		a_Player->UseEquippedItem();
		return true;
	}





	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) const override
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
