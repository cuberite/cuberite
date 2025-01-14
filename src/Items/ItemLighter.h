
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "BlockInfo.h"





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
			if (m_ItemType == Item::FlintAndSteel)
			{
				a_Player->UseEquippedItem();
			}
			else  // Fire charge.
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
		}

		// Activate TNT if we clicked on it while not crouched:
		if ((a_World->GetBlock(a_ClickedBlockPos).Type() == BlockType::Tnt) && !a_Player->IsCrouched())
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
		if (IsBlockAir(a_World->GetBlock(FirePos)))
		{
			a_World->PlaceBlock(FirePos, Block::Fire::Fire());

			// The client plays flint and steel sounds, only need to handle fire charges:
			if (m_ItemType == Item::FireCharge)
			{
				a_World->BroadcastSoundEffect("item.firecharge.use", FirePos, 1.0f, 1.04f);
			}
		}

		return false;
	}
} ;
