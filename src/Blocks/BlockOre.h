
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockOreHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
public:
	cBlockOreHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		cFastRandom Random;

		switch (m_BlockType)
		{
			case E_BLOCK_LAPIS_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_DYE, (char)(4 + Random.NextInt(5)), 4));
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				a_Pickups.push_back(cItem(E_ITEM_REDSTONE_DUST, (char)(4 + Random.NextInt(2)), 0));
				break;
			}
			case E_BLOCK_DIAMOND_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_DIAMOND));
				break;
			}
			case E_BLOCK_EMERALD_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_EMERALD));
				break;
			}
			case E_BLOCK_COAL_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_COAL));
				break;
			}
			default:
			{
				a_Pickups.push_back(cItem(m_BlockType));
				break;
			}
		}
	}

	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		super::OnDestroyedByPlayer(a_ChunkInterface, a_WorldInterface, a_Player, a_BlockX, a_BlockY, a_BlockZ);

		if (a_Player->IsGameModeCreative())
		{
			// Don't drop XP when the player is in creative mode.
			return;
		}

		if (a_Player->GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchSilkTouch) != 0)
		{
			// Don't drop XP when the ore is mined with the Silk Touch enchantment
			return;
		}

		cFastRandom Random;
		int Reward = 0;

		switch (m_BlockType)
		{
			case E_BLOCK_NETHER_QUARTZ_ORE:
			case E_BLOCK_LAPIS_ORE:
			{
				// Lapis and nether quartz get 2 - 5 experience
				Reward = Random.NextInt(4) + 2;
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				// Redstone gets 1 - 5 experience
				Reward = Random.NextInt(5) + 1;
				break;
			}
			case E_BLOCK_DIAMOND_ORE:
			case E_BLOCK_EMERALD_ORE:
			{
				// Diamond and emerald get 3 - 7 experience
				Reward = Random.NextInt(5) + 3;
				break;
			}
			case E_BLOCK_COAL_ORE:
			{
				// Coal gets 0 - 2 experience
				Reward = Random.NextInt(3);
				break;
			}

			default: break;
		}

		if (Reward != 0)
		{
			a_WorldInterface.SpawnExperienceOrb(a_BlockX, a_BlockY, a_BlockZ, Reward);
		}
	}
} ;




