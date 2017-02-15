
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



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		// Handle growing the plants:
		if ((a_Item.m_ItemDamage == E_META_DYE_WHITE) && (a_BlockFace != BLOCK_FACE_NONE))
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

			// Check if the block that the player clicked is a jungle log.
			if ((BlockType != E_BLOCK_LOG) || ((BlockMeta & 0x3) != E_META_LOG_JUNGLE))
			{
				return false;
			}

			// Get the location from the new cocoa pod.
			AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, false);
			BlockMeta = cBlockCocoaPodHandler::BlockFaceToMeta(a_BlockFace);

			if (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) != E_BLOCK_AIR)
			{
				return false;
			}

			// Place the cocoa pod:
			if (a_Player->PlaceBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_COCOA_POD, BlockMeta))
			{
				if (a_Player->IsGameModeSurvival())
				{
					a_Player->GetInventory().RemoveOneEquippedItem();
				}
				return true;
			}
		}
		return false;
	}

} ;




