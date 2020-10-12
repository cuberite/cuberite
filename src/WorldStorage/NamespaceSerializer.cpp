#include "Globals.h"

#include "NamespaceSerializer.h"

namespace NamespaceSerializer
{
	unsigned DataVersion()
	{
		return 2566;
	}

	const char * From(const Statistic a_ID)
	{
		switch (a_ID)
		{
			case Statistic::AnimalsBred:                  return "animals_bred";
			case Statistic::AviateOneCm:                  return "aviate_one_cm";
			case Statistic::BellRing:                     return "bell_ring";
			case Statistic::BoatOneCm:                    return "boat_one_cm";
			case Statistic::CleanArmor:                   return "clean_armor";
			case Statistic::CleanBanner:                  return "clean_banner";
			case Statistic::CleanShulkerBox:              return "clean_shulker_box";
			case Statistic::ClimbOneCm:                   return "climb_one_cm";
			case Statistic::CrouchOneCm:                  return "crouch_one_cm";
			case Statistic::DamageAbsorbed:               return "damage_absorbed";
			case Statistic::DamageBlockedByShield:        return "damage_blocked_by_shield";
			case Statistic::DamageDealt:                  return "damage_dealt";
			case Statistic::DamageDealtAbsorbed:          return "damage_dealt_absorbed";
			case Statistic::DamageDealtResisted:          return "damage_dealt_resisted";
			case Statistic::DamageResisted:               return "damage_resisted";
			case Statistic::DamageTaken:                  return "damage_taken";
			case Statistic::Deaths:                       return "deaths";
			case Statistic::Drop:                         return "drop";
			case Statistic::EatCakeSlice:                 return "eat_cake_slice";
			case Statistic::EnchantItem:                  return "enchant_item";
			case Statistic::FallOneCm:                    return "fall_one_cm";
			case Statistic::FillCauldron:                 return "fill_cauldron";
			case Statistic::FishCaught:                   return "fish_caught";
			case Statistic::FlyOneCm:                     return "fly_one_cm";
			case Statistic::HorseOneCm:                   return "horse_one_cm";
			case Statistic::InspectDispenser:             return "inspect_dispenser";
			case Statistic::InspectDropper:               return "inspect_dropper";
			case Statistic::InspectHopper:                return "inspect_hopper";
			case Statistic::InteractWithAnvil:            return "interact_with_anvil";
			case Statistic::InteractWithBeacon:           return "interact_with_beacon";
			case Statistic::InteractWithBlastFurnace:     return "interact_with_blast_furnace";
			case Statistic::InteractWithBrewingstand:     return "interact_with_brewingstand";
			case Statistic::InteractWithCampfire:         return "interact_with_campfire";
			case Statistic::InteractWithCartographyTable: return "interact_with_cartography_table";
			case Statistic::InteractWithCraftingTable:    return "interact_with_crafting_table";
			case Statistic::InteractWithFurnace:          return "interact_with_furnace";
			case Statistic::InteractWithGrindstone:       return "interact_with_grindstone";
			case Statistic::InteractWithLectern:          return "interact_with_lectern";
			case Statistic::InteractWithLoom:             return "interact_with_loom";
			case Statistic::InteractWithSmithingTable:    return "interact_with_smithing_table";
			case Statistic::InteractWithSmoker:           return "interact_with_smoker";
			case Statistic::InteractWithStonecutter:      return "interact_with_stonecutter";
			case Statistic::Jump:                         return "jump";
			case Statistic::LeaveGame:                    return "leave_game";
			case Statistic::MinecartOneCm:                return "minecart_one_cm";
			case Statistic::MobKills:                     return "mob_kills";
			case Statistic::OpenBarrel:                   return "open_barrel";
			case Statistic::OpenChest:                    return "open_chest";
			case Statistic::OpenEnderchest:               return "open_enderchest";
			case Statistic::OpenShulkerBox:               return "open_shulker_box";
			case Statistic::PigOneCm:                     return "pig_one_cm";
			case Statistic::PlayNoteblock:                return "play_noteblock";
			case Statistic::PlayOneMinute:                return "play_one_minute";
			case Statistic::PlayRecord:                   return "play_record";
			case Statistic::PlayerKills:                  return "player_kills";
			case Statistic::PotFlower:                    return "pot_flower";
			case Statistic::RaidTrigger:                  return "raid_trigger";
			case Statistic::RaidWin:                      return "raid_win";
			case Statistic::SleepInBed:                   return "sleep_in_bed";
			case Statistic::SneakTime:                    return "sneak_time";
			case Statistic::SprintOneCm:                  return "sprint_one_cm";
			case Statistic::StriderOneCm:                 return "strider_one_cm";
			case Statistic::SwimOneCm:                    return "swim_one_cm";
			case Statistic::TalkedToVillager:             return "talked_to_villager";
			case Statistic::TargetHit:                    return "target_hit";
			case Statistic::TimeSinceDeath:               return "time_since_death";
			case Statistic::TimeSinceRest:                return "time_since_rest";
			case Statistic::TradedWithVillager:           return "traded_with_villager";
			case Statistic::TriggerTrappedChest:          return "trigger_trapped_chest";
			case Statistic::TuneNoteblock:                return "tune_noteblock";
			case Statistic::UseCauldron:                  return "use_cauldron";
			case Statistic::WalkOnWaterOneCm:             return "walk_on_water_one_cm";
			case Statistic::WalkOneCm:                    return "walk_one_cm";
			case Statistic::WalkUnderWaterOneCm:          return "walk_under_water_one_cm";

			// Old ones just for compatibility
			case Statistic::JunkFished:                   return "junk_fished";
			case Statistic::TreasureFished:               return "treasure_fished";

			// The old advancements
			case Statistic::AchOpenInventory:             return "cuberite:achievement.openInventory";
			case Statistic::AchMineWood:                  return "cuberite:achievement.mineWood";
			case Statistic::AchBuildWorkBench:            return "cuberite:achievement.buildWorkBench";
			case Statistic::AchBuildPickaxe:              return "cuberite:achievement.buildPickaxe";
			case Statistic::AchBuildFurnace:              return "cuberite:achievement.buildFurnace";
			case Statistic::AchAcquireIron:               return "cuberite:achievement.acquireIron";
			case Statistic::AchBuildHoe:                  return "cuberite:achievement.buildHoe";
			case Statistic::AchMakeBread:                 return "cuberite:achievement.makeBread";
			case Statistic::AchBakeCake:                  return "cuberite:achievement.bakeCake";
			case Statistic::AchBuildBetterPickaxe:        return "cuberite:achievement.buildBetterPickaxe";
			case Statistic::AchCookFish:                  return "cuberite:achievement.cookFish";
			case Statistic::AchOnARail:                   return "cuberite:achievement.onARail";
			case Statistic::AchBuildSword:                return "cuberite:achievement.buildSword";
			case Statistic::AchKillEnemy:                 return "cuberite:achievement.killEnemy";
			case Statistic::AchKillCow:                   return "cuberite:achievement.killCow";
			case Statistic::AchFlyPig:                    return "cuberite:achievement.flyPig";
			case Statistic::AchSnipeSkeleton:             return "cuberite:achievement.snipeSkeleton";
			case Statistic::AchDiamonds:                  return "cuberite:achievement.diamonds";
			case Statistic::AchPortal:                    return "cuberite:achievement.portal";
			case Statistic::AchGhast:                     return "cuberite:achievement.ghast";
			case Statistic::AchBlazeRod:                  return "cuberite:achievement.blazeRod";
			case Statistic::AchPotion:                    return "cuberite:achievement.potion";
			case Statistic::AchTheEnd:                    return "cuberite:achievement.theEnd";
			case Statistic::AchTheEnd2:                   return "cuberite:achievement.theEnd2";
			case Statistic::AchEnchantments:              return "cuberite:achievement.enchantments";
			case Statistic::AchOverkill:                  return "cuberite:achievement.overkill";
			case Statistic::AchBookcase:                  return "cuberite:achievement.bookcase";
			case Statistic::AchExploreAllBiomes:          return "cuberite:achievement.exploreAllBiomes";
			case Statistic::AchSpawnWither:               return "cuberite:achievement.spawnWither";
			case Statistic::AchKillWither:                return "cuberite:achievement.killWither";
			case Statistic::AchFullBeacon:                return "cuberite:achievement.fullBeacon";
			case Statistic::AchBreedCow:                  return "cuberite:achievement.breedCow";
			case Statistic::AchDiamondsToYou:             return "cuberite:achievement.diamondsToYou";
		}

		UNREACHABLE("Tried to save unhandled statistic");
	}
	
	



	const char * From(eMonsterType a_ID)
	{
		switch (a_ID)
		{
			case mtInvalidType:    return "";
			case mtBat:            return "minecraft:bat";
			case mtBlaze:          return "minecraft:blaze";
			case mtCat:            return "minecraft:cat";
			case mtCaveSpider:     return "minecraft:cave_spider";
			case mtChicken:        return "minecraft:chicken";
			case mtCod:            return "minecraft:cod";
			case mtCow:            return "minecraft:cow";
			case mtCreeper:        return "minecraft:creeper";
			case mtDolphin:        return "minecraft:dolphin";
			case mtDonkey:         return "minecraft:donkey";
			case mtDrowned:        return "minecraft:drowned";
			case mtElderGuardian:  return "minecraft:elder:guardian";
			case mtEnderDragon:    return "minecraft:ender_dragon";
			case mtEnderman:       return "minecraft:enderman";
			case mtEndermite:      return "minecraft:endermite";
			case mtEvoker:         return "minecraft:evoker";
			case mtFox:            return "minecraft:fox";
			case mtGhast:          return "minecraft:ghast";
			case mtGiant:          return "minecraft:giant";
			case mtGuardian:       return "minecraft:guardian";
			case mtHorse:          return "minecraft:horse";
			case mtHoglin:         return "minecraft:hoglin";
			case mtHusk:           return "minecraft:husk";
			case mtIllusioner:     return "minecraft:illusioner";
			case mtIronGolem:      return "minecraft:iron_golem";
			case mtLlama:          return "minecraft:llama";
			case mtMagmaCube:      return "minecraft:magma_cube";
			case mtMooshroom:      return "minecraft:mooshroom";
			case mtMule:           return "minecraft:mule";
			case mtOcelot:         return "minecraft:ocelot";
			case mtPanda:          return "minecraft:panda";
			case mtParrot:         return "minecraft:parrot";
			case mtPhantom:        return "minecraft:phantom";
			case mtPig:            return "minecraft:pig";
			case mtPiglin:         return "minecraft:piglin";
			case mtPiglinBrute:    return "minecraft:piglin_brute";
			case mtPillager:       return "minecraft:pillager";
			case mtPolarBear:      return "minecraft:polar_bear";
			case mtPufferfish:     return "minecraft:pufferfish";
			case mtRabbit:         return "minecraft:rabbit";
			case mtRavager:        return "minecraft:ravager";
			case mtSalmon:         return "minecraft:salmon";
			case mtSheep:          return "minecraft:sheep";
			case mtShulker:        return "minecraft:shulker";
			case mtSilverfish:     return "minecraft:silverfish";
			case mtSkeleton:       return "minecraft:skeleton";
			case mtSkeletonHorse:  return "minecraft:skeleton_horse";
			case mtSlime:          return "minecraft:slime";
			case mtSnowGolem:      return "minecraft:snow_golem";
			case mtSpider:         return "minecraft:spider";
			case mtSquid:          return "minecraft:squid";
			case mtStray:          return "minecraft:stray";
			case mtStrider:        return "minecraft:strider";
			case mtTraderLlama:    return "minecraft:trader_llama";
			case mtTropicalFish:   return "minecraft:tropical_fish";
			case mtTurtle:         return "minecraft:turtle";
			case mtVex:            return "minecraft:vex";
			case mtVillager:       return "minecraft:villager";
			case mtVindicator:     return "minecraft:vindicator";
			case mtWanderingTrader:return "minecraft:wandering_trader";
			case mtWitch:          return "minecraft:witch";
			case mtWither:         return "minecraft:wither";
			case mtWitherSkeleton: return "minecraft:wither_skeleton";
			case mtWolf:           return "minecraft:wolf";
			case mtZoglin:         return "minecraft:zoglin";
			case mtZombie:         return "minecraft:zombie";
			case mtZombieHorse:    return "minecraft:zombie_horse";
			case mtZombiePigman:   return "minecraft:zombified_piglin";
			case mtZombieVillager: return "minecraft:zombie_villager";
		}
		UNREACHABLE("Tried to save unknown monster type");
	}





	static const std::unordered_map<std::string_view, Statistic> CustomStatistics
	{
		{ "animals_bred",                            Statistic::AnimalsBred },
		{ "aviate_one_cm",                           Statistic::AviateOneCm },
		{ "bell_ring",                               Statistic::BellRing },
		{ "boat_one_cm",                             Statistic::BoatOneCm },
		{ "clean_armor",                             Statistic::CleanArmor },
		{ "clean_banner",                            Statistic::CleanBanner },
		{ "clean_shulker_box",                       Statistic::CleanShulkerBox },
		{ "climb_one_cm",                            Statistic::ClimbOneCm },
		{ "crouch_one_cm",                           Statistic::CrouchOneCm },
		{ "damage_absorbed",                         Statistic::DamageAbsorbed },
		{ "damage_blocked_by_shield",                Statistic::DamageBlockedByShield },
		{ "damage_dealt",                            Statistic::DamageDealt },
		{ "damage_dealt_absorbed",                   Statistic::DamageDealtAbsorbed },
		{ "damage_dealt_resisted",                   Statistic::DamageDealtResisted },
		{ "damage_resisted",                         Statistic::DamageResisted },
		{ "damage_taken",                            Statistic::DamageTaken },
		{ "deaths",                                  Statistic::Deaths },
		{ "drop",                                    Statistic::Drop },
		{ "eat_cake_slice",                          Statistic::EatCakeSlice },
		{ "enchant_item",                            Statistic::EnchantItem },
		{ "fall_one_cm",                             Statistic::FallOneCm },
		{ "fill_cauldron",                           Statistic::FillCauldron },
		{ "fish_caught",                             Statistic::FishCaught },
		{ "fly_one_cm",                              Statistic::FlyOneCm },
		{ "horse_one_cm",                            Statistic::HorseOneCm },
		{ "inspect_dispenser",                       Statistic::InspectDispenser },
		{ "inspect_dropper",                         Statistic::InspectDropper },
		{ "inspect_hopper",                          Statistic::InspectHopper },
		{ "interact_with_anvil",                     Statistic::InteractWithAnvil },
		{ "interact_with_beacon",                    Statistic::InteractWithBeacon },
		{ "interact_with_blast_furnace",             Statistic::InteractWithBlastFurnace },
		{ "interact_with_brewingstand",              Statistic::InteractWithBrewingstand },
		{ "interact_with_campfire",                  Statistic::InteractWithCampfire },
		{ "interact_with_cartography_table",         Statistic::InteractWithCartographyTable },
		{ "interact_with_crafting_table",            Statistic::InteractWithCraftingTable },
		{ "interact_with_furnace",                   Statistic::InteractWithFurnace },
		{ "interact_with_grindstone",                Statistic::InteractWithGrindstone },
		{ "interact_with_lectern",                   Statistic::InteractWithLectern },
		{ "interact_with_loom",                      Statistic::InteractWithLoom },
		{ "interact_with_smithing_table",            Statistic::InteractWithSmithingTable },
		{ "interact_with_smoker",                    Statistic::InteractWithSmoker },
		{ "interact_with_stonecutter",               Statistic::InteractWithStonecutter },
		{ "jump",                                    Statistic::Jump },
		{ "leave_game",                              Statistic::LeaveGame },
		{ "minecart_one_cm",                         Statistic::MinecartOneCm },
		{ "mob_kills",                               Statistic::MobKills },
		{ "open_barrel",                             Statistic::OpenBarrel },
		{ "open_chest",                              Statistic::OpenChest },
		{ "open_enderchest",                         Statistic::OpenEnderchest },
		{ "open_shulker_box",                        Statistic::OpenShulkerBox },
		{ "pig_one_cm",                              Statistic::PigOneCm },
		{ "play_noteblock",                          Statistic::PlayNoteblock },
		{ "play_one_minute",                         Statistic::PlayOneMinute },
		{ "play_record",                             Statistic::PlayRecord },
		{ "player_kills",                            Statistic::PlayerKills },
		{ "pot_flower",                              Statistic::PotFlower },
		{ "raid_trigger",                            Statistic::RaidTrigger },
		{ "raid_win",                                Statistic::RaidWin },
		{ "sleep_in_bed",                            Statistic::SleepInBed },
		{ "sneak_time",                              Statistic::SneakTime },
		{ "sprint_one_cm",                           Statistic::SprintOneCm },
		{ "strider_one_cm",                          Statistic::StriderOneCm },
		{ "swim_one_cm",                             Statistic::SwimOneCm },
		{ "talked_to_villager",                      Statistic::TalkedToVillager },
		{ "target_hit",                              Statistic::TargetHit },
		{ "time_since_death",                        Statistic::TimeSinceDeath },
		{ "time_since_rest",                         Statistic::TimeSinceRest },
		{ "traded_with_villager",                    Statistic::TradedWithVillager },
		{ "trigger_trapped_chest",                   Statistic::TriggerTrappedChest },
		{ "tune_noteblock",                          Statistic::TuneNoteblock },
		{ "use_cauldron",                            Statistic::UseCauldron },
		{ "walk_on_water_one_cm",                    Statistic::WalkOnWaterOneCm },
		{ "walk_one_cm",                             Statistic::WalkOneCm },
		{ "walk_under_water_one_cm",                 Statistic::WalkUnderWaterOneCm },




	Statistic ToCustomStatistic(const std::string_view ID)
	{
		static const std::unordered_map<std::string_view, Statistic> CustomStatistics
		{
			{ "animals_bred",                            Statistic::AnimalsBred },
			{ "aviate_one_cm",                           Statistic::AviateOneCm },
			{ "bell_ring",                               Statistic::BellRing },
			{ "boat_one_cm",                             Statistic::BoatOneCm },
			{ "clean_armor",                             Statistic::CleanArmor },
			{ "clean_banner",                            Statistic::CleanBanner },
			{ "clean_shulker_box",                       Statistic::CleanShulkerBox },
			{ "climb_one_cm",                            Statistic::ClimbOneCm },
			{ "crouch_one_cm",                           Statistic::CrouchOneCm },
			{ "damage_absorbed",                         Statistic::DamageAbsorbed },
			{ "damage_blocked_by_shield",                Statistic::DamageBlockedByShield },
			{ "damage_dealt",                            Statistic::DamageDealt },
			{ "damage_dealt_absorbed",                   Statistic::DamageDealtAbsorbed },
			{ "damage_dealt_resisted",                   Statistic::DamageDealtResisted },
			{ "damage_resisted",                         Statistic::DamageResisted },
			{ "damage_taken",                            Statistic::DamageTaken },
			{ "deaths",                                  Statistic::Deaths },
			{ "drop",                                    Statistic::Drop },
			{ "eat_cake_slice",                          Statistic::EatCakeSlice },
			{ "enchant_item",                            Statistic::EnchantItem },
			{ "fall_one_cm",                             Statistic::FallOneCm },
			{ "fill_cauldron",                           Statistic::FillCauldron },
			{ "fish_caught",                             Statistic::FishCaught },
			{ "fly_one_cm",                              Statistic::FlyOneCm },
			{ "horse_one_cm",                            Statistic::HorseOneCm },
			{ "inspect_dispenser",                       Statistic::InspectDispenser },
			{ "inspect_dropper",                         Statistic::InspectDropper },
			{ "inspect_hopper",                          Statistic::InspectHopper },
			{ "interact_with_anvil",                     Statistic::InteractWithAnvil },
			{ "interact_with_beacon",                    Statistic::InteractWithBeacon },
			{ "interact_with_blast_furnace",             Statistic::InteractWithBlastFurnace },
			{ "interact_with_brewingstand",              Statistic::InteractWithBrewingstand },
			{ "interact_with_campfire",                  Statistic::InteractWithCampfire },
			{ "interact_with_cartography_table",         Statistic::InteractWithCartographyTable },
			{ "interact_with_crafting_table",            Statistic::InteractWithCraftingTable },
			{ "interact_with_furnace",                   Statistic::InteractWithFurnace },
			{ "interact_with_grindstone",                Statistic::InteractWithGrindstone },
			{ "interact_with_lectern",                   Statistic::InteractWithLectern },
			{ "interact_with_loom",                      Statistic::InteractWithLoom },
			{ "interact_with_smithing_table",            Statistic::InteractWithSmithingTable },
			{ "interact_with_smoker",                    Statistic::InteractWithSmoker },
			{ "interact_with_stonecutter",               Statistic::InteractWithStonecutter },
			{ "jump",                                    Statistic::Jump },
			{ "leave_game",                              Statistic::LeaveGame },
			{ "minecart_one_cm",                         Statistic::MinecartOneCm },
			{ "mob_kills",                               Statistic::MobKills },
			{ "open_barrel",                             Statistic::OpenBarrel },
			{ "open_chest",                              Statistic::OpenChest },
			{ "open_enderchest",                         Statistic::OpenEnderchest },
			{ "open_shulker_box",                        Statistic::OpenShulkerBox },
			{ "pig_one_cm",                              Statistic::PigOneCm },
			{ "play_noteblock",                          Statistic::PlayNoteblock },
			{ "play_one_minute",                         Statistic::PlayOneMinute },
			{ "play_record",                             Statistic::PlayRecord },
			{ "player_kills",                            Statistic::PlayerKills },
			{ "pot_flower",                              Statistic::PotFlower },
			{ "raid_trigger",                            Statistic::RaidTrigger },
			{ "raid_win",                                Statistic::RaidWin },
			{ "sleep_in_bed",                            Statistic::SleepInBed },
			{ "sneak_time",                              Statistic::SneakTime },
			{ "sprint_one_cm",                           Statistic::SprintOneCm },
			{ "strider_one_cm",                          Statistic::StriderOneCm },
			{ "swim_one_cm",                             Statistic::SwimOneCm },
			{ "talked_to_villager",                      Statistic::TalkedToVillager },
			{ "target_hit",                              Statistic::TargetHit },
			{ "time_since_death",                        Statistic::TimeSinceDeath },
			{ "time_since_rest",                         Statistic::TimeSinceRest },
			{ "traded_with_villager",                    Statistic::TradedWithVillager },
			{ "trigger_trapped_chest",                   Statistic::TriggerTrappedChest },
			{ "tune_noteblock",                          Statistic::TuneNoteblock },
			{ "use_cauldron",                            Statistic::UseCauldron },
			{ "walk_on_water_one_cm",                    Statistic::WalkOnWaterOneCm },
			{ "walk_one_cm",                             Statistic::WalkOneCm },
			{ "walk_under_water_one_cm",                 Statistic::WalkUnderWaterOneCm },

			// Old ones just for compatibility
			{ "junk_fished",                             Statistic::JunkFished },
			{ "treasure_fished",                         Statistic::TreasureFished },

			// The old advancements
			{ "cuberite:achievement.openInventory",      Statistic::AchOpenInventory },
			{ "cuberite:achievement.mineWood",           Statistic::AchMineWood },
			{ "cuberite:achievement.buildWorkBench",     Statistic::AchBuildWorkBench },
			{ "cuberite:achievement.buildPickaxe",       Statistic::AchBuildPickaxe },
			{ "cuberite:achievement.buildFurnace",       Statistic::AchBuildFurnace },
			{ "cuberite:achievement.acquireIron",        Statistic::AchAcquireIron },
			{ "cuberite:achievement.buildHoe",           Statistic::AchBuildHoe },
			{ "cuberite:achievement.makeBread",          Statistic::AchMakeBread },
			{ "cuberite:achievement.bakeCake",           Statistic::AchBakeCake },
			{ "cuberite:achievement.buildBetterPickaxe", Statistic::AchBuildBetterPickaxe },
			{ "cuberite:achievement.cookFish",           Statistic::AchCookFish },
			{ "cuberite:achievement.onARail",            Statistic::AchOnARail },
			{ "cuberite:achievement.buildSword",         Statistic::AchBuildSword },
			{ "cuberite:achievement.killEnemy",          Statistic::AchKillEnemy },
			{ "cuberite:achievement.killCow",            Statistic::AchKillCow },
			{ "cuberite:achievement.flyPig",             Statistic::AchFlyPig },
			{ "cuberite:achievement.snipeSkeleton",      Statistic::AchSnipeSkeleton },
			{ "cuberite:achievement.diamonds",           Statistic::AchDiamonds },
			{ "cuberite:achievement.portal",             Statistic::AchPortal },
			{ "cuberite:achievement.ghast",              Statistic::AchGhast },
			{ "cuberite:achievement.blazeRod",           Statistic::AchBlazeRod },
			{ "cuberite:achievement.potion",             Statistic::AchPotion },
			{ "cuberite:achievement.theEnd",             Statistic::AchTheEnd },
			{ "cuberite:achievement.theEnd2",            Statistic::AchTheEnd2 },
			{ "cuberite:achievement.enchantments",       Statistic::AchEnchantments },
			{ "cuberite:achievement.overkill",           Statistic::AchOverkill },
			{ "cuberite:achievement.bookcase",           Statistic::AchBookcase },
			{ "cuberite:achievement.exploreAllBiomes",   Statistic::AchExploreAllBiomes },
			{ "cuberite:achievement.spawnWither",        Statistic::AchSpawnWither },
			{ "cuberite:achievement.killWither",         Statistic::AchKillWither },
			{ "cuberite:achievement.fullBeacon",         Statistic::AchFullBeacon },
			{ "cuberite:achievement.breedCow",           Statistic::AchBreedCow },
			{ "cuberite:achievement.diamondsToYou",      Statistic::AchDiamondsToYou}
		};

		return CustomStatistics.at(ID);
	}

	std::pair<Namespace, std::string_view> SplitNamespacedID(const std::string_view ID)
	{
		const auto NamespaceIndex = ID.find(':');
		if (NamespaceIndex == std::string_view::npos)
		{
			// No explicit namespace defaults to the Minecraft namespace:
			return { Namespace::Minecraft, ID };
		}

		const auto Namespace = ID.substr(0, NamespaceIndex);
		if (Namespace == "minecraft")
		{
			// An unprefixed ID in the vanilla Minecraft namespace.
			const auto Value = ID.substr(NamespaceIndex + 1);

			return { Namespace::Minecraft, Value };
		}

		if (Namespace == "cuberite")
		{
			return { Namespace::Cuberite, ID };
		}

		return { Namespace::Unknown, ID };
	}

	static const std::unordered_map<std::string_view, eMonsterType> MonsterTypes
	{
		{"minecraft:bat",              mtBat},
		{"minecraft:blaze",            mtBlaze},
		{"minecraft:cat",              mtCat},
		{"minecraft:cave_spider",      mtCaveSpider},
		{"minecraft:chicken",          mtChicken},
		{"minecraft:cod",              mtCod},
		{"minecraft:cow",              mtCow},
		{"minecraft:creeper",          mtCreeper},
		{"minecraft:dolphin",          mtDolphin},
		{"minecraft:donkey",           mtDonkey},
		{"minecraft:drowned",          mtDrowned},
		{"minecraft:elder_guardian",   mtElderGuardian},
		{"minecraft:ender_dragon",     mtEnderDragon},
		{"minecraft:enderman",         mtEnderman},
		{"minecraft:endermite",        mtEndermite},
		{"minecraft:evoker",           mtEvoker},
		{"minecraft:fox",              mtFox},
		{"minecraft:ghast",            mtGhast},
		{"minecraft:giant",            mtGiant},
		{"minecraft:guardian",         mtGuardian},
		{"minecraft:horse",            mtHorse},
		{"minecraft:hoglin",           mtHoglin},
		{"minecraft:husk",             mtHusk},
		{"minecraft:illusioner",       mtIllusioner},
		{"minecraft:iron_golem",       mtIronGolem},
		{"minecraft:llama",            mtLlama},
		{"minecraft:magma_cube",       mtMagmaCube},
		{"minecraft:mooshroom",        mtMooshroom},
		{"minecraft:mule",             mtMule},
		{"minecraft:ocelot",           mtOcelot},
		{"minecraft:panda",            mtPanda},
		{"minecraft:parrot",           mtParrot},
		{"minecraft:phantom",          mtPhantom},
		{"minecraft:pig",              mtPig},
		{"minecraft:piglin",           mtPiglin},
		{"minecraft:piglin_brute",     mtPiglinBrute},
		{"minecraft:pillager",         mtPillager},
		{"minecraft:polar_bear",       mtPolarBear},
		{"minecraft:pufferfish",       mtPufferfish},
		{"minecraft:rabbit",           mtRabbit},
		{"minecraft:ravager",          mtRavager},
		{"minecraft:salmon",           mtSalmon},
		{"minecraft:sheep",            mtSheep},
		{"minecraft:shulker",          mtShulker},
		{"minecraft:silverfish",       mtSilverfish},
		{"minecraft:skeleton",         mtSkeleton},
		{"minecraft:skeleton_horse",   mtSkeletonHorse},
		{"minecraft:slime",            mtSlime},
		{"minecraft:snow_golem",       mtSnowGolem},
		{"minecraft:spider",           mtSpider},
		{"minecraft:squid",            mtSquid},
		{"minecraft:stray",            mtStray},
		{"minecraft:strider",          mtStrider},
		{"minecraft:trader_llama",     mtTraderLlama},
		{"minecraft:tropical_fish",    mtTropicalFish},
		{"minecraft:turtle",           mtTurtle},
		{"minecraft:vex",              mtVex},
		{"minecraft:villager",         mtVillager},
		{"minecraft:vindicator",       mtVindicator},
		{"minecraft:wandering_trader", mtWanderingTrader},
		{"minecraft:witch",            mtWitch},
		{"minecraft:wither",           mtWither},
		{"minecraft:wither_skeleton",  mtWitherSkeleton},
		{"minecraft:wolf",             mtWolf},
		{"minecraft:zoglin",           mtZoglin},
		{"minecraft:zombie",           mtZombie},
		{"minecraft:zombie_horse",     mtZombieHorse},
		{"minecraft:zombie_pigman",    mtZombiePigman},
		{"minecraft:zombie_villager",  mtZombieVillager},

		{"minecraft:villager_golem",   mtIronGolem},
		{"minecraft:snowman",          mtSnowGolem},

		// Old names:
		{"Bat",            mtBat},
		{"Blaze",          mtBlaze},
		{"CaveSpider",     mtCaveSpider},
		{"Chicken",        mtChicken},
		{"Cow",            mtCow},
		{"Creeper",        mtCreeper},
		{"EnderDragon",    mtEnderDragon},
		{"Enderman",       mtEnderman},
		{"Ghast",          mtGhast},
		{"Giant",          mtGiant},
		{"Guardian",       mtGuardian},
		{"Horse",          mtHorse},
		{"VillagerGolem",  mtIronGolem},
		{"LavaSlime",      mtMagmaCube},
		{"MushroomCow",    mtMooshroom},
		{"Ozelot",         mtOcelot},
		{"Pig",            mtPig},
		{"Rabbit",         mtRabbit},
		{"Sheep",          mtSheep},
		{"Silverfish",     mtSilverfish},
		{"Skeleton",       mtSkeleton},
		{"Slime",          mtSlime},
		{"SnowMan",        mtSnowGolem},
		{"Spider",         mtSpider},
		{"Squid",          mtSquid},
		{"Villager",       mtVillager},
		{"Witch",          mtWitch},
		{"WitherBoss",     mtWither},
		{"WitherSkeleton", mtWitherSkeleton},
		{"Wolf",           mtWolf},
		{"Zombie",         mtZombie},
		{"PigZombie",      mtZombiePigman},
		{"ZombieVillager", mtZombieVillager}
	};

	eMonsterType ToMonsterType(std::string_view a_ID)
	{
		try
		{
			return MonsterTypes.at(a_ID);
		}
		catch (std::out_of_range)
		{
			return mtInvalidType;
		}
	}
}
