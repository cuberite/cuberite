
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemLighterHandler final:
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
		if (a_ClickedBlockFace < 0)
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			if (m_ItemType == E_ITEM_FLINT_AND_STEEL)
			{
				a_Player->UseEquippedItem();
			}
			else  // Fire charge.
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
		}

		// Activate TNT if we clicked on it while not crouched:
		if ((a_World->GetBlock(a_ClickedBlockPos) == E_BLOCK_TNT) && !a_Player->IsCrouched())
		{
			a_World->DigBlock(a_ClickedBlockPos, a_Player);
			a_World->SpawnPrimedTNT(Vector3d(a_ClickedBlockPos) + Vector3d(0.5, 0.5, 0.5));  // 80 ticks to boom
			return false;
		}

		const auto FirePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		if (!cChunkDef::IsValidHeight(FirePos))
		{
			return false;
		}

		// Light a fire next to / on top of the block if air:
		if (a_World->GetBlock(FirePos) == E_BLOCK_AIR)
		{
			a_World->PlaceBlock(FirePos, E_BLOCK_FIRE, 0);

			// The client plays flint and steel sounds, only need to handle fire charges:
			if (m_ItemType == E_ITEM_FIRE_CHARGE)
			{
				a_World->BroadcastSoundEffect(eSoundEvent::ItemFirechargeUse, FirePos, 1.0f, 1.04f);
			}
			else
			{
				// 1.8 doesn't play, we need to handle it here
				a_World->BroadcastSoundEffect(eSoundEvent::ItemFlintsteelUse, FirePos, 1.0f, 1.04f);
			}
		}

		return false;
	}
} ;
