
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemSpawnEggHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemSpawnEggHandler(Item a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		// Must click a valid block:
		if (a_ClickedBlockFace < 0)
		{
			return false;
		}

		auto PlacementPos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		if (a_ClickedBlockFace == BLOCK_FACE_YM)
		{
			PlacementPos.y--;
		}

		auto MonsterType = ItemToMonsterType(a_HeldItem.m_ItemType);
		if (
			(MonsterType != mtInvalidType) &&  // Valid monster type
			(a_World->SpawnMob(PlacementPos.x + 0.5, PlacementPos.y, PlacementPos.z + 0.5, MonsterType, false) != cEntity::INVALID_ID))  // Spawning succeeded
		{
			if (!a_Player->IsGameModeCreative())
			{
				// The mob was spawned, "use" the item:
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
			return true;
		}

		return false;
	}





	/** Converts the Spawn egg item damage to the monster type to spawn.
	Returns mtInvalidType for invalid damage values. */
	static eMonsterType ItemToMonsterType(const cItem & a_Item)
	{
		switch (a_Item.m_ItemType)
		{
			case Item::BatSpawnEgg:            return mtBat;
			case Item::BlazeSpawnEgg:          return mtBlaze;
			case Item::CaveSpiderSpawnEgg:     return mtCaveSpider;
			case Item::ChickenSpawnEgg:        return mtChicken;
			case Item::CowSpawnEgg:            return mtCow;
			case Item::CreeperSpawnEgg:        return mtCreeper;
			case Item::EndermanSpawnEgg:       return mtEnderman;
			case Item::GhastSpawnEgg:          return mtGhast;
			case Item::GuardianSpawnEgg:       return mtGuardian;
			case Item::HorseSpawnEgg:          return mtHorse;
			case Item::MagmaCubeSpawnEgg:      return mtMagmaCube;
			case Item::MooshroomSpawnEgg:      return mtMooshroom;
			case Item::OcelotSpawnEgg:         return mtOcelot;
			case Item::PigSpawnEgg:            return mtPig;
			case Item::RabbitSpawnEgg:         return mtRabbit;
			case Item::SheepSpawnEgg:          return mtSheep;
			case Item::SilverfishSpawnEgg:     return mtSilverfish;
			case Item::SkeletonSpawnEgg:       return mtSkeleton;
			case Item::SlimeSpawnEgg:          return mtSlime;
			case Item::SpiderSpawnEgg:         return mtSpider;
			case Item::SquidSpawnEgg:          return mtSquid;
			case Item::VillagerSpawnEgg:       return mtVillager;
			case Item::WitchSpawnEgg:          return mtWitch;
			case Item::WitherSkeletonSpawnEgg: return mtWitherSkeleton;
			case Item::WolfSpawnEgg:           return mtWolf;
			case Item::ZombieSpawnEgg:         return mtZombie;
			case Item::ZombiePigmanSpawnEgg:   return mtZombiePigman;
			case Item::ZombieVillagerSpawnEgg: return mtZombieVillager;
			//todo impelemnt all new eggs
			default: return mtInvalidType;
		}
	}

	static inline bool IsSpawnEgg(const cItem & a_Item)
	{
		switch (a_Item.m_ItemType)
		{
			case Item::AllaySpawnEgg:
			case Item::ArmadilloSpawnEgg:
			case Item::AxolotlSpawnEgg:
			case Item::BatSpawnEgg:
			case Item::BeeSpawnEgg:
			case Item::BlazeSpawnEgg:
			case Item::BoggedSpawnEgg:
			case Item::BreezeSpawnEgg:
			case Item::CamelSpawnEgg:
			case Item::CatSpawnEgg:
			case Item::CaveSpiderSpawnEgg:
			case Item::ChickenSpawnEgg:
			case Item::CodSpawnEgg:
			case Item::CowSpawnEgg:
			case Item::CreeperSpawnEgg:
			case Item::DolphinSpawnEgg:
			case Item::DonkeySpawnEgg:
			case Item::DrownedSpawnEgg:
			case Item::ElderGuardianSpawnEgg:
			case Item::EnderDragonSpawnEgg:
			case Item::EndermanSpawnEgg:
			case Item::EndermiteSpawnEgg:
			case Item::EvokerSpawnEgg:
			case Item::FoxSpawnEgg:
			case Item::FrogSpawnEgg:
			case Item::GhastSpawnEgg:
			case Item::GlowSquidSpawnEgg:
			case Item::GoatSpawnEgg:
			case Item::GuardianSpawnEgg:
			case Item::HoglinSpawnEgg:
			case Item::HorseSpawnEgg:
			case Item::HuskSpawnEgg:
			case Item::IronGolemSpawnEgg:
			case Item::LlamaSpawnEgg:
			case Item::MagmaCubeSpawnEgg:
			case Item::MooshroomSpawnEgg:
			case Item::MuleSpawnEgg:
			case Item::OcelotSpawnEgg:
			case Item::PandaSpawnEgg:
			case Item::ParrotSpawnEgg:
			case Item::PhantomSpawnEgg:
			case Item::PigSpawnEgg:
			case Item::PiglinBruteSpawnEgg:
			case Item::PiglinSpawnEgg:
			case Item::PillagerSpawnEgg:
			case Item::PolarBearSpawnEgg:
			case Item::PufferfishSpawnEgg:
			case Item::RabbitSpawnEgg:
			case Item::RavagerSpawnEgg:
			case Item::SalmonSpawnEgg:
			case Item::SheepSpawnEgg:
			case Item::ShulkerSpawnEgg:
			case Item::SilverfishSpawnEgg:
			case Item::SkeletonHorseSpawnEgg:
			case Item::SkeletonSpawnEgg:
			case Item::SlimeSpawnEgg:
			case Item::SnifferSpawnEgg:
			case Item::SnowGolemSpawnEgg:
			case Item::SpiderSpawnEgg:
			case Item::SquidSpawnEgg:
			case Item::StraySpawnEgg:
			case Item::StriderSpawnEgg:
			case Item::TadpoleSpawnEgg:
			case Item::TraderLlamaSpawnEgg:
			case Item::TropicalFishSpawnEgg:
			case Item::TurtleSpawnEgg:
			case Item::VexSpawnEgg:
			case Item::VillagerSpawnEgg:
			case Item::VindicatorSpawnEgg:
			case Item::WanderingTraderSpawnEgg:
			case Item::WardenSpawnEgg:
			case Item::WitchSpawnEgg:
			case Item::WitherSkeletonSpawnEgg:
			case Item::WitherSpawnEgg:
			case Item::WolfSpawnEgg:
			case Item::ZoglinSpawnEgg:
			case Item::ZombieHorseSpawnEgg:
			case Item::ZombieSpawnEgg:
			case Item::ZombieVillagerSpawnEgg:
			case Item::ZombifiedPiglinSpawnEgg:
				return true;
			default: return false;
		}
	}
} ;
