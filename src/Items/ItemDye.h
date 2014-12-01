
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Blocks/BlockCocoaPod.h"





class cItemDyeHandler :
	public cItemHandler
{
public:
	cItemDyeHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		// Handle growing the plants:
		if (a_Item.m_ItemDamage == E_META_DYE_WHITE)
		{
			if (a_World->GrowRipePlant(a_BlockX, a_BlockY, a_BlockZ, true))
			{
				// Particle effects are in GrowRipePlant
				if (!a_Player->IsGameModeCreative())
				{
					a_Player->GetInventory().RemoveOneEquippedItem();
					return true;
				}
			}
		}
		else if ((a_Item.m_ItemDamage == E_META_DYE_BROWN) && (a_BlockFace >= BLOCK_FACE_ZM) && (a_BlockFace <= BLOCK_FACE_XP))
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, BlockType, BlockMeta);

			if ((BlockType != E_BLOCK_LOG) || ((BlockMeta & 0x3) != E_META_LOG_JUNGLE))
			{
				return false;
			}

			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, false);
			BlockMeta = cBlockCocoaPodHandler::BlockFaceToMeta(a_BlockFace);

			if (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) != E_BLOCK_AIR)
			{
				return false;
			}

			if (cRoot::Get()->GetPluginManager()->CallHookPlayerPlacingBlock(*a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, 0, 0, 0, E_BLOCK_COCOA_POD, BlockMeta))
			{
				a_World->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
				a_Player->GetInventory().SendEquippedSlot();
				return false;
			}

			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_COCOA_POD, BlockMeta);
			a_World->BroadcastSoundEffect("dig.stone", a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5, 1.0f, 0.8f);

			if (!a_Player->IsGameModeCreative())
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
			cRoot::Get()->GetPluginManager()->CallHookPlayerPlacedBlock(*a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, 0, 0, 0, E_BLOCK_COCOA_POD, BlockMeta);
			return true;
		}
		return false;
	}

} ;




