
#pragma once

#include "BlockHandler.h"

class cBlockOreHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

private:
	inline unsigned int fortuneDropMult(unsigned int fortuneLevel)
	{
		/** Returns the correct drop multiplier for ores
			EXCLUDING REDSTONE ORE which is handled differently
			works for coal, diamond, emerald, nether gold, nether quartz and lapis
			https://minecraft.gamepedia.com/Fortune#Ore */
		auto & random = GetRandomProvider();
		float rand = random.RandReal();
		switch (fortuneLevel)
		{
			case 0:
				return 1;
			case 1:
				if (rand<0.66)
				{
					return 1;
				}
				else
				{
					return 2;
				}
				break;
			case 2:
				if (rand<0.5)
				{
					return 1;
				}
				else if (rand<0.75)
				{
					return 2;
				}
				else
				{
					return 3;
				}
				break;
			case 3:
			{
				if (rand < 0.4)
				{
					return 1;
				}
				else if (rand < 0.6)
				{
					return 2;
				}
				else if (rand < 0.8)
				{
					return 3;
				}
				else
				{
					return 4;
				}
			}
		}
		return 1;
	}





public:
	cBlockOreHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// If using silk-touch, drop self rather than the resource:
		if (ToolHasSilkTouch(a_Tool))
		{
			switch (m_BlockType)
			{
				// If it was a glowing redstone ore, drop a normal redstone ore
				case E_BLOCK_REDSTONE_ORE_GLOWING:   return cItem(E_BLOCK_REDSTONE_ORE);
				default:                             return cItem(m_BlockType);
			}
		}

		unsigned int fortuneLevel = ToolFortuneLevel(a_Tool);
		unsigned int dropMult = fortuneDropMult(fortuneLevel);

		auto & random = GetRandomProvider();
		switch (m_BlockType)
		{
			case E_BLOCK_LAPIS_ORE:            return cItem(E_ITEM_DYE, dropMult * random.RandInt<char>(4, 8), 4);
			case E_BLOCK_REDSTONE_ORE:         // handled by next case (glowing redstone), no dropMult
			case E_BLOCK_REDSTONE_ORE_GLOWING: return cItem(E_ITEM_REDSTONE_DUST, random.RandInt<char>(4, 5 + fortuneLevel), 0);
			case E_BLOCK_DIAMOND_ORE:          return cItem(E_ITEM_DIAMOND, dropMult);
			case E_BLOCK_EMERALD_ORE:          return cItem(E_ITEM_EMERALD, dropMult);
			case E_BLOCK_COAL_ORE:             return cItem(E_ITEM_COAL, dropMult);
			case E_BLOCK_NETHER_QUARTZ_ORE:    return cItem(E_ITEM_NETHER_QUARTZ, dropMult);
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




