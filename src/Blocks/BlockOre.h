
#pragma once

#include "Blocks/BlockHandler.h"
#include "Items/ItemHandler.h"





class cBlockOreHandler:
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

protected:
	~cBlockOreHandler() = default;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// If using silk-touch, drop self rather than the resource:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(BlockItemConverter::FromBlock(m_BlockType));
		}

		const auto FortuneLevel = ToolFortuneLevel(a_Tool);

		if ((m_BlockType == BlockType::RedstoneOre) || (m_BlockType == BlockType::DeepslateRedstoneOre))
		{   // Redstone follows the discrete random distribution, unlike other ores
			const auto DropNum = FortuneDiscreteRandom(4, 5, FortuneLevel);
			return cItem(Item::Redstone, DropNum);
		}

		// TODO: drop raw ores
		auto & Random = GetRandomProvider();
		const auto DropMult = std::max(static_cast<char>(1), FloorC<char>(Random.RandReal(FortuneLevel + 2.0)));
		switch (m_BlockType)
		{
			case BlockType::DeepslateLapisOre:
			case BlockType::LapisOre:        return cItem(Item::LapisLazuli, DropMult * Random.RandInt<char>(4, 9), 4);
			case BlockType::DeepslateDiamondOre:
			case BlockType::DiamondOre:      return cItem(Item::Diamond,     DropMult);
			case BlockType::DeepslateEmeraldOre:
			case BlockType::EmeraldOre:      return cItem(Item::Emerald,     DropMult);
			case BlockType::DeepslateCoalOre:
			case BlockType::CoalOre:         return cItem(Item::Coal,        DropMult);
			case BlockType::NetherQuartzOre: return cItem(Item::Quartz,      DropMult);
			case BlockType::Clay:            return cItem(Item::ClayBall,    4);
			case BlockType::CopperOre:
			case BlockType::DeepslateCopperOre: return cItem(Item::RawCopper, DropMult * Random.RandInt<char>(2, 5), 4);
			case BlockType::IronOre:
			case BlockType::DeepslateIronOre: return cItem(Item::RawIron, static_cast<char>(1 + FortuneLevel));
			case BlockType::GoldOre:
			case BlockType::DeepslateGoldOre: return cItem(Item::RawGold, static_cast<char>(1 + FortuneLevel));
			case BlockType::NetherGoldOre: return cItem(Item::GoldNugget, DropMult * Random.RandInt<char>(2, 6));
			default:
			{
				return cItem(BlockItemConverter::FromBlock(m_BlockType));
			}
		}
	}





	virtual void OnBroken(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface,
		Vector3i a_BlockPos,
		BlockState a_OldBlock,
		const cEntity * a_Digger
	) const override
	{
		if (a_Digger == nullptr)
		{
			return;
		}
		if (!a_Digger->IsPlayer())
		{
			return;
		}

		const auto Player = static_cast<const cPlayer *>(a_Digger);
		if (!Player->IsGameModeSurvival())
		{
			// Don't drop XP unless the player is in survival mode.
			return;
		}

		if (Player->GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchSilkTouch) != 0)
		{
			// Don't drop XP when the ore is mined with the Silk Touch enchantment
			return;
		}

		if (!Player->GetEquippedItem().GetHandler().CanHarvestBlock(m_BlockType))
		{
			return;
		}

		auto & Random = GetRandomProvider();
		int Reward = 0;

		switch (a_OldBlock.Type())
		{
			case BlockType::NetherQuartzOre:
			case BlockType::LapisOre:
			{
				// Lapis and nether quartz get 2 - 5 experience
				Reward = Random.RandInt(2, 5);
				break;
			}
			case BlockType::RedstoneOre:
			{
				// Redstone gets 1 - 5 experience
				Reward = Random.RandInt(1, 5);
				break;
			}
			case BlockType::DiamondOre:
			case BlockType::EmeraldOre:
			{
				// Diamond and emerald get 3 - 7 experience
				Reward = Random.RandInt(3, 7);
				break;
			}
			case BlockType::CoalOre:
			{
				// Coal gets 0 - 2 experience
				Reward = Random.RandInt(2);
				break;
			}

			default: break;
		}

		if (Reward > 0)
		{
			a_WorldInterface.SpawnSplitExperienceOrbs(Vector3d(0.5, 0.5, 0.5) + a_BlockPos, Reward);
		}
	}
} ;





class cDefaultOreHandler final :
	public cBlockOreHandler
{
public:

	using cBlockOreHandler::cBlockOreHandler;
};
