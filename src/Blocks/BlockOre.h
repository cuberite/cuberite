
#pragma once

#include "BlockHandler.h"





#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cBlockOreHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// If using silk-touch, drop self rather than the resource:
		if (ToolHasSilkTouch(a_Tool))
		{
			switch (m_BlockType)
			{
				// If it was a glowing redstone ore, drop a normal redstone ore:
				case E_BLOCK_REDSTONE_ORE_GLOWING:   return cItem(E_BLOCK_REDSTONE_ORE);
				default:                             return cItem(m_BlockType);
			}
		}

		const auto FortuneLevel = ToolFortuneLevel(a_Tool);

		if ((m_BlockType == E_BLOCK_REDSTONE_ORE) || (m_BlockType == E_BLOCK_REDSTONE_ORE_GLOWING))
		{   // Redstone follows the discrete random distribution, unlike other ores
			const auto DropNum = FortuneDiscreteRandom(4, 5, FortuneLevel);
			return cItem(E_ITEM_REDSTONE_DUST, DropNum);
		}

		auto & Random = GetRandomProvider();
		const auto DropMult = std::max(static_cast<char>(1), FloorC<char>(Random.RandReal(FortuneLevel + 2.0)));
		switch (m_BlockType)
		{
			case E_BLOCK_LAPIS_ORE:            return cItem(E_ITEM_DYE, DropMult * Random.RandInt<char>(4, 9), 4);
			case E_BLOCK_DIAMOND_ORE:          return cItem(E_ITEM_DIAMOND, DropMult);
			case E_BLOCK_EMERALD_ORE:          return cItem(E_ITEM_EMERALD, DropMult);
			case E_BLOCK_COAL_ORE:             return cItem(E_ITEM_COAL, DropMult);
			case E_BLOCK_NETHER_QUARTZ_ORE:    return cItem(E_ITEM_NETHER_QUARTZ, DropMult);
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
	) const override
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

#ifdef __clang__
#pragma clang diagnostic pop
#endif




