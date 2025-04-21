
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemSpawnEggHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
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
			(MonsterType != eEntityType::etInvalid) &&  // Valid monster type
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
	static eEntityType ItemToMonsterType(const cItem & a_Item)
	{
		using ET = eEntityType;

		switch (a_Item.m_ItemType)
		{
			case Item::AllaySpawnEgg:           return ET::etAllay;
			case Item::ArmadilloSpawnEgg:       return ET::etArmadillo;
			case Item::AxolotlSpawnEgg:         return ET::etAxolotl;
			case Item::BatSpawnEgg:             return ET::etBat;
			case Item::BeeSpawnEgg:             return ET::etBee;
			case Item::BlazeSpawnEgg:           return ET::etBlaze;
			case Item::BoggedSpawnEgg:          return ET::etBogged;
			case Item::BreezeSpawnEgg:          return ET::etBreeze;
			case Item::CamelSpawnEgg:           return ET::etCamel;
			case Item::CatSpawnEgg:             return ET::etCat;
			case Item::CaveSpiderSpawnEgg:      return ET::etCaveSpider;
			case Item::ChickenSpawnEgg:         return ET::etChicken;
			case Item::CodSpawnEgg:             return ET::etCod;
			case Item::CowSpawnEgg:             return ET::etCow;
			case Item::CreakingSpawnEgg:        return ET::etCreaking;
			case Item::CreeperSpawnEgg:         return ET::etCreeper;
			case Item::DolphinSpawnEgg:         return ET::etDolphin;
			case Item::DonkeySpawnEgg:          return ET::etDonkey;
			case Item::DrownedSpawnEgg:         return ET::etDrowned;
			case Item::ElderGuardianSpawnEgg:   return ET::etElderGuardian;
			case Item::EnderDragonSpawnEgg:     return ET::etEnderDragon;
			case Item::EndermanSpawnEgg:        return ET::etEnderman;
			case Item::EndermiteSpawnEgg:       return ET::etEndermite;
			case Item::EvokerSpawnEgg:          return ET::etEvoker;
			case Item::FoxSpawnEgg:             return ET::etFox;
			case Item::FrogSpawnEgg:            return ET::etFrog;
			case Item::GhastSpawnEgg:           return ET::etGhast;
			case Item::GlowSquidSpawnEgg:       return ET::etGlowSquid;
			case Item::GoatSpawnEgg:            return ET::etGoat;
			case Item::GuardianSpawnEgg:        return ET::etGuardian;
			case Item::HoglinSpawnEgg:          return ET::etHoglin;
			case Item::HorseSpawnEgg:           return ET::etHorse;
			case Item::HuskSpawnEgg:            return ET::etHusk;
			case Item::IronGolemSpawnEgg:       return ET::etIronGolem;
			case Item::LlamaSpawnEgg:           return ET::etLlama;
			case Item::MagmaCubeSpawnEgg:       return ET::etMagmaCube;
			case Item::MooshroomSpawnEgg:       return ET::etMooshroom;
			case Item::MuleSpawnEgg:            return ET::etMule;
			case Item::OcelotSpawnEgg:          return ET::etOcelot;
			case Item::PandaSpawnEgg:           return ET::etPanda;
			case Item::ParrotSpawnEgg:          return ET::etParrot;
			case Item::PhantomSpawnEgg:         return ET::etPhantom;
			case Item::PigSpawnEgg:             return ET::etPig;
			case Item::PiglinBruteSpawnEgg:     return ET::etPiglinBrute;
			case Item::PiglinSpawnEgg:          return ET::etPiglin;
			case Item::PillagerSpawnEgg:        return ET::etPillager;
			case Item::PolarBearSpawnEgg:       return ET::etPolarBear;
			case Item::PufferfishSpawnEgg:      return ET::etPufferfish;
			case Item::RabbitSpawnEgg:          return ET::etRabbit;
			case Item::RavagerSpawnEgg:         return ET::etRavager;
			case Item::SalmonSpawnEgg:          return ET::etSalmon;
			case Item::SheepSpawnEgg:           return ET::etSheep;
			case Item::ShulkerSpawnEgg:         return ET::etShulker;
			case Item::SilverfishSpawnEgg:      return ET::etSilverfish;
			case Item::SkeletonHorseSpawnEgg:   return ET::etSkeletonHorse;
			case Item::SkeletonSpawnEgg:        return ET::etSkeleton;
			case Item::SlimeSpawnEgg:           return ET::etSlime;
			case Item::SnifferSpawnEgg:         return ET::etSniffer;
			case Item::SnowGolemSpawnEgg:       return ET::etSnowGolem;
			case Item::SpiderSpawnEgg:          return ET::etSpider;
			case Item::SquidSpawnEgg:           return ET::etSquid;
			case Item::StraySpawnEgg:           return ET::etStray;
			case Item::StriderSpawnEgg:         return ET::etStrider;
			case Item::TadpoleSpawnEgg:         return ET::etTadpole;
			case Item::TraderLlamaSpawnEgg:     return ET::etTraderLlama;
			case Item::TropicalFishSpawnEgg:    return ET::etTropicalFish;
			case Item::TurtleSpawnEgg:          return ET::etTurtle;
			case Item::VexSpawnEgg:             return ET::etVex;
			case Item::VillagerSpawnEgg:        return ET::etVillager;
			case Item::VindicatorSpawnEgg:      return ET::etVindicator;
			case Item::WanderingTraderSpawnEgg: return ET::etWanderingTrader;
			case Item::WardenSpawnEgg:          return ET::etWarden;
			case Item::WitchSpawnEgg:           return ET::etWitch;
			case Item::WitherSkeletonSpawnEgg:  return ET::etWitherSkeleton;
			case Item::WitherSpawnEgg:          return ET::etWither;
			case Item::WolfSpawnEgg:            return ET::etWolf;
			case Item::ZoglinSpawnEgg:          return ET::etZoglin;
			case Item::ZombieHorseSpawnEgg:     return ET::etZombieHorse;
			case Item::ZombieSpawnEgg:          return ET::etZombie;
			case Item::ZombieVillagerSpawnEgg:  return ET::etZombieVillager;
			case Item::ZombifiedPiglinSpawnEgg: return ET::etZombifiedPiglin;
			default: return ET::etInvalid;
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
			case Item::CreakingSpawnEgg:
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
