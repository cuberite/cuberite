
#pragma once

#include "BlockHandler.h"





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
		auto & Random = GetRandomProvider();

		switch (m_BlockType)
		{
			case E_BLOCK_LAPIS_ORE:
			{
				a_Pickups.emplace_back(E_ITEM_DYE, Random.RandInt<char>(4, 8), 4);
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				a_Pickups.emplace_back(E_ITEM_REDSTONE_DUST,  Random.RandInt<char>(4, 5), 0);
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
			case E_BLOCK_NETHER_QUARTZ_ORE:
			{
				a_Pickups.push_back(cItem(E_ITEM_NETHER_QUARTZ));
				break;
			}
			case E_BLOCK_CLAY:
			{
				a_Pickups.push_back(cItem(E_ITEM_CLAY, 4));
				break;
			}
			default:
			{
				a_Pickups.push_back(cItem(m_BlockType));
				break;
			}
		}
	}

	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		super::OnDestroyedByPlayer(a_ChunkInterface, a_WorldInterface, a_Player, a_BlockX, a_BlockY, a_BlockZ);

		if (a_Player.IsGameModeCreative())
		{
			// Don't drop XP when the player is in creative mode.
			return;
		}

		if (a_Player.GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchSilkTouch) != 0)
		{
			// Don't drop XP when the ore is mined with the Silk Touch enchantment
			return;
		}

		auto & Random = GetRandomProvider();
		int Reward = 0;

		switch (m_BlockType)
		{
			case E_BLOCK_NETHER_QUARTZ_ORE:
			case E_BLOCK_LAPIS_ORE:
			{
				// Lapis and nether quartz get 2 - 5 experience
				Reward = Random.RandInt(2, 5);
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				// Redstone gets 1 - 5 experience
				Reward = Random.RandInt(1, 5);
				break;
			}
			case E_BLOCK_DIAMOND_ORE:
			case E_BLOCK_EMERALD_ORE:
			{
				// Diamond and emerald get 3 - 7 experience
				Reward = Random.RandInt(3, 7);
				break;
			}
			case E_BLOCK_COAL_ORE:
			{
				// Coal gets 0 - 2 experience
				Reward = Random.RandInt(2);
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




