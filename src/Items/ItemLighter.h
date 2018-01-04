
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemLighterHandler :
	public cItemHandler
{
public:
	cItemLighterHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if (a_BlockFace < 0)
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

		switch (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ))
		{
			case E_BLOCK_TNT:
			{
				// Activate the TNT:
				a_World->BroadcastSoundEffect("entity.tnt.primed", Vector3d(a_BlockX, a_BlockY, a_BlockZ), 1.0f, 1.0f);
				a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
				a_World->SpawnPrimedTNT({a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5});  // 80 ticks to boom
				break;
			}
			default:
			{
				// Light a fire next to / on top of the block if air:
				AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
				if ((a_BlockY < 0) || (a_BlockY >= cChunkDef::Height))
				{
					break;
				}
				if (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_AIR)
				{
					a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FIRE, 0);
					a_World->BroadcastSoundEffect("item.flintandsteel.use", Vector3d(a_BlockX, a_BlockY, a_BlockZ), 1.0F, 1.04F);
					break;
				}
			}
		}

		return false;
	}
} ;
