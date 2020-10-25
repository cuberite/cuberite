
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemLighterHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemLighterHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		if (a_ClickedBlockFace < 0)
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			switch (m_ItemType)
			{
				case E_ITEM_FLINT_AND_STEEL:
				{
					a_Player->UseEquippedItem();
					break;
				}
				case E_ITEM_FIRE_CHARGE:
				{
					a_Player->GetInventory().RemoveOneEquippedItem();
					break;
				}
				default:
				{
					ASSERT(!"Unknown Lighter Item!");
				}
			}
		}

		switch (a_World->GetBlock(a_ClickedBlockPos))
		{
			case E_BLOCK_TNT:
			{
				// Activate the TNT:
				a_World->DigBlock(a_ClickedBlockPos, a_Player);
				a_World->SpawnPrimedTNT(Vector3d(a_ClickedBlockPos) + Vector3d(0.5, 0.5, 0.5));  // 80 ticks to boom
				break;
			}
			default:
			{
				// Light a fire next to / on top of the block if air:
				auto FirePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
				if (!cChunkDef::IsValidHeight(FirePos.y))
				{
					break;
				}
				if (a_World->GetBlock(FirePos) == E_BLOCK_AIR)
				{
					a_World->PlaceBlock(FirePos, E_BLOCK_FIRE, 0);
					a_World->BroadcastSoundEffect("item.flintandsteel.use", FirePos, 1.0f, 1.04f);
					break;
				}
			}
		}

		return false;
	}
} ;
