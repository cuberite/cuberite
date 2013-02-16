
#pragma once

#include "BlockHandler.h"





class cBlockFlowerPotHandler :
	public cBlockHandler
{
public:
	cBlockFlowerPotHandler(BLOCKTYPE a_BlockType) :
		cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_FLOWER_POT, 1, 0));
		if (a_BlockMeta == 0)
		{
			return;
		}
		cItem Plant;
		switch (a_BlockMeta)
		{
			case 1:  Plant = cItem(E_BLOCK_RED_ROSE, 1, 0); break;
			case 2:  Plant = cItem(E_BLOCK_YELLOW_FLOWER, 1, 0); break;
			case 3:  Plant = cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_APPLE); break;
			case 4:  Plant = cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_CONIFER); break;
			case 5:  Plant = cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_BIRCH); break;
			case 6:  Plant = cItem(E_BLOCK_SAPLING, 1, E_META_SAPLING_JUNGLE); break;
			case 7:  Plant = cItem(E_BLOCK_RED_MUSHROOM, 1, 0); break;
			case 8:  Plant = cItem(E_BLOCK_BROWN_MUSHROOM, 1, 0); break;
			case 9:  Plant = cItem(E_BLOCK_CACTUS, 1, 0); break;
			case 10: Plant = cItem(E_BLOCK_DEAD_BUSH, 1, 0); break;
			case 11: Plant = cItem(E_BLOCK_TALL_GRASS, 1, E_META_TALL_GRASS_FERN); break;
			default: return;
		}
		a_Pickups.push_back(Plant);
	}


	void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		NIBBLETYPE Meta = a_World->GetBlockMeta( a_BlockX, a_BlockY, a_BlockZ );
		if (Meta != 0)
		{
			// Already filled
			return;
		}
		
		switch (a_Player->GetEquippedItem().m_ItemType)
		{
			case E_BLOCK_RED_ROSE: Meta = 1; break;
			case E_BLOCK_YELLOW_FLOWER: Meta = 2; break;
			case E_BLOCK_SAPLING:
			{
				switch (a_Player->GetEquippedItem().m_ItemDamage)
				{
					case E_META_SAPLING_APPLE: Meta = 3; break;
					case E_META_SAPLING_CONIFER: Meta = 4; break;
					case E_META_SAPLING_BIRCH: Meta = 5; break;
					case E_META_SAPLING_JUNGLE: Meta = 6; break;
				}
				break;
			}
			case E_BLOCK_RED_MUSHROOM: Meta = 7; break;
			case E_BLOCK_BROWN_MUSHROOM: Meta = 8; break;
			case E_BLOCK_CACTUS: Meta = 9; break;
			case E_BLOCK_DEAD_BUSH: Meta = 10; break;
			case E_BLOCK_TALL_GRASS:
			{
				if (a_Player->GetEquippedItem().m_ItemDamage == E_META_TALL_GRASS_FERN)
				{
					Meta = 11;
				}
				else
				{
					return;
				}
				break;
			}
		}
		
		if (a_Player->GetGameMode() != eGameMode_Creative)
		{
			cItem Item(a_Player->GetEquippedItem().m_ItemType, 1);
			a_Player->GetInventory().RemoveItem(Item);
		}
		a_World->SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta);
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
} ;




