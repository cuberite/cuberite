
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





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// If using silk-touch, drop self rather than the resource:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType);
		}

		// TODO: Handle the Fortune enchantment here
		auto & random = GetRandomProvider();
		switch (m_BlockType)
		{
			case E_BLOCK_LAPIS_ORE:            return cItem(E_ITEM_DYE, random.RandInt<char>(4, 8), 4);
			case E_BLOCK_REDSTONE_ORE:         return cItem(E_ITEM_REDSTONE_DUST, random.RandInt<char>(4, 5), 0);
			case E_BLOCK_REDSTONE_ORE_GLOWING: return cItem(E_ITEM_REDSTONE_DUST, random.RandInt<char>(4, 5), 0);
			case E_BLOCK_DIAMOND_ORE:          return cItem(E_ITEM_DIAMOND);
			case E_BLOCK_EMERALD_ORE:          return cItem(E_ITEM_EMERALD);
			case E_BLOCK_COAL_ORE:             return cItem(E_ITEM_COAL);
			case E_BLOCK_NETHER_QUARTZ_ORE:    return cItem(E_ITEM_NETHER_QUARTZ);
			case E_BLOCK_CLAY:                 return cItem(E_ITEM_CLAY, 4);
			default:
			{
				return cItem(m_BlockType);
			}
		}
	}





	virtual void OnPlayerBrokeBlock(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		cPlayer & a_Player, Vector3i a_BlockPos,
		BLOCKTYPE a_OldBlockType, NIBBLETYPE a_OldBlockMeta
	) override
	{
		if (!a_Player.IsGameModeSurvival())
		{
			// Don't drop XP unless the player is in survival mode.
			return;
		}

		if (a_Player.GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchSilkTouch) != 0)
		{
			// Don't drop XP when the ore is mined with the Silk Touch enchantment
			return;
		}

		auto & random = GetRandomProvider();
		int reward = 0;

		switch (a_OldBlockType)
		{
			case E_BLOCK_NETHER_QUARTZ_ORE:
			case E_BLOCK_LAPIS_ORE:
			{
				// Lapis and nether quartz get 2 - 5 experience
				reward = random.RandInt(2, 5);
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				// Redstone gets 1 - 5 experience
				reward = random.RandInt(1, 5);
				break;
			}
			case E_BLOCK_DIAMOND_ORE:
			case E_BLOCK_EMERALD_ORE:
			{
				// Diamond and emerald get 3 - 7 experience
				reward = random.RandInt(3, 7);
				break;
			}
			case E_BLOCK_COAL_ORE:
			{
				// Coal gets 0 - 2 experience
				reward = random.RandInt(2);
				break;
			}

			default: break;
		}

		if (reward > 0)
		{
			a_WorldInterface.SpawnSplitExperienceOrbs(Vector3d(0.5, 0.5, 0.5) + a_BlockPos, reward);
		}
	}
} ;




