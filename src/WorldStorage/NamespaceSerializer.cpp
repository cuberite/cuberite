#include "Globals.h"

#include "NamespaceSerializer.h"





unsigned NamespaceSerializer::DataVersion()
{
	return 2566;
}





std::string_view NamespaceSerializer::From(const CustomStatistic a_ID)
{
	switch (a_ID)
	{
		case CustomStatistic::AnimalsBred:                  return "animals_bred";
		case CustomStatistic::AviateOneCm:                  return "aviate_one_cm";
		case CustomStatistic::BellRing:                     return "bell_ring";
		case CustomStatistic::BoatOneCm:                    return "boat_one_cm";
		case CustomStatistic::CleanArmor:                   return "clean_armor";
		case CustomStatistic::CleanBanner:                  return "clean_banner";
		case CustomStatistic::CleanShulkerBox:              return "clean_shulker_box";
		case CustomStatistic::ClimbOneCm:                   return "climb_one_cm";
		case CustomStatistic::CrouchOneCm:                  return "crouch_one_cm";
		case CustomStatistic::DamageAbsorbed:               return "damage_absorbed";
		case CustomStatistic::DamageBlockedByShield:        return "damage_blocked_by_shield";
		case CustomStatistic::DamageDealt:                  return "damage_dealt";
		case CustomStatistic::DamageDealtAbsorbed:          return "damage_dealt_absorbed";
		case CustomStatistic::DamageDealtResisted:          return "damage_dealt_resisted";
		case CustomStatistic::DamageResisted:               return "damage_resisted";
		case CustomStatistic::DamageTaken:                  return "damage_taken";
		case CustomStatistic::Deaths:                       return "deaths";
		case CustomStatistic::Drop:                         return "drop";
		case CustomStatistic::EatCakeSlice:                 return "eat_cake_slice";
		case CustomStatistic::EnchantItem:                  return "enchant_item";
		case CustomStatistic::FallOneCm:                    return "fall_one_cm";
		case CustomStatistic::FillCauldron:                 return "fill_cauldron";
		case CustomStatistic::FishCaught:                   return "fish_caught";
		case CustomStatistic::FlyOneCm:                     return "fly_one_cm";
		case CustomStatistic::HorseOneCm:                   return "horse_one_cm";
		case CustomStatistic::InspectDispenser:             return "inspect_dispenser";
		case CustomStatistic::InspectDropper:               return "inspect_dropper";
		case CustomStatistic::InspectHopper:                return "inspect_hopper";
		case CustomStatistic::InteractWithAnvil:            return "interact_with_anvil";
		case CustomStatistic::InteractWithBeacon:           return "interact_with_beacon";
		case CustomStatistic::InteractWithBlastFurnace:     return "interact_with_blast_furnace";
		case CustomStatistic::InteractWithBrewingstand:     return "interact_with_brewingstand";
		case CustomStatistic::InteractWithCampfire:         return "interact_with_campfire";
		case CustomStatistic::InteractWithCartographyTable: return "interact_with_cartography_table";
		case CustomStatistic::InteractWithCraftingTable:    return "interact_with_crafting_table";
		case CustomStatistic::InteractWithFurnace:          return "interact_with_furnace";
		case CustomStatistic::InteractWithGrindstone:       return "interact_with_grindstone";
		case CustomStatistic::InteractWithLectern:          return "interact_with_lectern";
		case CustomStatistic::InteractWithLoom:             return "interact_with_loom";
		case CustomStatistic::InteractWithSmithingTable:    return "interact_with_smithing_table";
		case CustomStatistic::InteractWithSmoker:           return "interact_with_smoker";
		case CustomStatistic::InteractWithStonecutter:      return "interact_with_stonecutter";
		case CustomStatistic::Jump:                         return "jump";
		case CustomStatistic::LeaveGame:                    return "leave_game";
		case CustomStatistic::MinecartOneCm:                return "minecart_one_cm";
		case CustomStatistic::MobKills:                     return "mob_kills";
		case CustomStatistic::OpenBarrel:                   return "open_barrel";
		case CustomStatistic::OpenChest:                    return "open_chest";
		case CustomStatistic::OpenEnderchest:               return "open_enderchest";
		case CustomStatistic::OpenShulkerBox:               return "open_shulker_box";
		case CustomStatistic::PigOneCm:                     return "pig_one_cm";
		case CustomStatistic::PlayNoteblock:                return "play_noteblock";
		case CustomStatistic::PlayOneMinute:                return "play_one_minute";
		case CustomStatistic::PlayRecord:                   return "play_record";
		case CustomStatistic::PlayerKills:                  return "player_kills";
		case CustomStatistic::PotFlower:                    return "pot_flower";
		case CustomStatistic::RaidTrigger:                  return "raid_trigger";
		case CustomStatistic::RaidWin:                      return "raid_win";
		case CustomStatistic::SleepInBed:                   return "sleep_in_bed";
		case CustomStatistic::SneakTime:                    return "sneak_time";
		case CustomStatistic::SprintOneCm:                  return "sprint_one_cm";
		case CustomStatistic::StriderOneCm:                 return "strider_one_cm";
		case CustomStatistic::SwimOneCm:                    return "swim_one_cm";
		case CustomStatistic::TalkedToVillager:             return "talked_to_villager";
		case CustomStatistic::TargetHit:                    return "target_hit";
		case CustomStatistic::TimeSinceDeath:               return "time_since_death";
		case CustomStatistic::TimeSinceRest:                return "time_since_rest";
		case CustomStatistic::TradedWithVillager:           return "traded_with_villager";
		case CustomStatistic::TriggerTrappedChest:          return "trigger_trapped_chest";
		case CustomStatistic::TuneNoteblock:                return "tune_noteblock";
		case CustomStatistic::UseCauldron:                  return "use_cauldron";
		case CustomStatistic::WalkOnWaterOneCm:             return "walk_on_water_one_cm";
		case CustomStatistic::WalkOneCm:                    return "walk_one_cm";
		case CustomStatistic::WalkUnderWaterOneCm:          return "walk_under_water_one_cm";

		// Old ones just for compatibility
		case CustomStatistic::JunkFished:                   return "junk_fished";
		case CustomStatistic::TreasureFished:               return "treasure_fished";

		// The old advancements
		case CustomStatistic::AchOpenInventory:             return "cuberite:achievement.openInventory";
		case CustomStatistic::AchMineWood:                  return "cuberite:achievement.mineWood";
		case CustomStatistic::AchBuildWorkBench:            return "cuberite:achievement.buildWorkBench";
		case CustomStatistic::AchBuildPickaxe:              return "cuberite:achievement.buildPickaxe";
		case CustomStatistic::AchBuildFurnace:              return "cuberite:achievement.buildFurnace";
		case CustomStatistic::AchAcquireIron:               return "cuberite:achievement.acquireIron";
		case CustomStatistic::AchBuildHoe:                  return "cuberite:achievement.buildHoe";
		case CustomStatistic::AchMakeBread:                 return "cuberite:achievement.makeBread";
		case CustomStatistic::AchBakeCake:                  return "cuberite:achievement.bakeCake";
		case CustomStatistic::AchBuildBetterPickaxe:        return "cuberite:achievement.buildBetterPickaxe";
		case CustomStatistic::AchCookFish:                  return "cuberite:achievement.cookFish";
		case CustomStatistic::AchOnARail:                   return "cuberite:achievement.onARail";
		case CustomStatistic::AchBuildSword:                return "cuberite:achievement.buildSword";
		case CustomStatistic::AchKillEnemy:                 return "cuberite:achievement.killEnemy";
		case CustomStatistic::AchKillCow:                   return "cuberite:achievement.killCow";
		case CustomStatistic::AchFlyPig:                    return "cuberite:achievement.flyPig";
		case CustomStatistic::AchSnipeSkeleton:             return "cuberite:achievement.snipeSkeleton";
		case CustomStatistic::AchDiamonds:                  return "cuberite:achievement.diamonds";
		case CustomStatistic::AchPortal:                    return "cuberite:achievement.portal";
		case CustomStatistic::AchGhast:                     return "cuberite:achievement.ghast";
		case CustomStatistic::AchBlazeRod:                  return "cuberite:achievement.blazeRod";
		case CustomStatistic::AchPotion:                    return "cuberite:achievement.potion";
		case CustomStatistic::AchTheEnd:                    return "cuberite:achievement.theEnd";
		case CustomStatistic::AchTheEnd2:                   return "cuberite:achievement.theEnd2";
		case CustomStatistic::AchEnchantments:              return "cuberite:achievement.enchantments";
		case CustomStatistic::AchOverkill:                  return "cuberite:achievement.overkill";
		case CustomStatistic::AchBookcase:                  return "cuberite:achievement.bookcase";
		case CustomStatistic::AchExploreAllBiomes:          return "cuberite:achievement.exploreAllBiomes";
		case CustomStatistic::AchSpawnWither:               return "cuberite:achievement.spawnWither";
		case CustomStatistic::AchKillWither:                return "cuberite:achievement.killWither";
		case CustomStatistic::AchFullBeacon:                return "cuberite:achievement.fullBeacon";
		case CustomStatistic::AchBreedCow:                  return "cuberite:achievement.breedCow";
		case CustomStatistic::AchDiamondsToYou:             return "cuberite:achievement.diamondsToYou";
	}
	UNREACHABLE("Tried to save unhandled statistic");
}





std::string_view NamespaceSerializer::From(const eMonsterType a_ID)
{
	switch (a_ID)
	{
		case mtBat:            return "bat";
		case mtBlaze:          return "blaze";
		case mtCat:            return "cat";
		case mtCaveSpider:     return "cave_spider";
		case mtChicken:        return "chicken";
		case mtCod:            return "cod";
		case mtCow:            return "cow";
		case mtCreeper:        return "creeper";
		case mtDolphin:        return "dolphin";
		case mtDonkey:         return "donkey";
		case mtDrowned:        return "drowned";
		case mtElderGuardian:  return "elder_guardian";
		case mtEnderDragon:    return "ender_dragon";
		case mtEnderman:       return "enderman";
		case mtEndermite:      return "endermite";
		case mtEvoker:         return "evoker";
		case mtFox:            return "fox";
		case mtGhast:          return "ghast";
		case mtGiant:          return "giant";
		case mtGuardian:       return "guardian";
		case mtHorse:          return "horse";
		case mtHoglin:         return "hoglin";
		case mtHusk:           return "husk";
		case mtIllusioner:     return "illusioner";
		case mtIronGolem:      return "iron_golem";
		case mtLlama:          return "llama";
		case mtMagmaCube:      return "magma_cube";
		case mtMooshroom:      return "mooshroom";
		case mtMule:           return "mule";
		case mtOcelot:         return "ocelot";
		case mtPanda:          return "panda";
		case mtParrot:         return "parrot";
		case mtPhantom:        return "phantom";
		case mtPig:            return "pig";
		case mtPiglin:         return "piglin";
		case mtPiglinBrute:    return "piglin_brute";
		case mtPillager:       return "pillager";
		case mtPolarBear:      return "polar_bear";
		case mtPufferfish:     return "pufferfish";
		case mtRabbit:         return "rabbit";
		case mtRavager:        return "ravager";
		case mtSalmon:         return "salmon";
		case mtSheep:          return "sheep";
		case mtShulker:        return "shulker";
		case mtSilverfish:     return "silverfish";
		case mtSkeleton:       return "skeleton";
		case mtSkeletonHorse:  return "skeleton_horse";
		case mtSlime:          return "slime";
		case mtSnowGolem:      return "snow_golem";
		case mtSpider:         return "spider";
		case mtSquid:          return "squid";
		case mtStray:          return "stray";
		case mtStrider:        return "strider";
		case mtTraderLlama:    return "trader_llama";
		case mtTropicalFish:   return "tropical_fish";
		case mtTurtle:         return "turtle";
		case mtVex:            return "vex";
		case mtVillager:       return "villager";
		case mtVindicator:     return "vindicator";
		case mtWanderingTrader:return "wandering_trader";
		case mtWitch:          return "witch";
		case mtWither:         return "wither";
		case mtWitherSkeleton: return "wither_skeleton";
		case mtWolf:           return "wolf";
		case mtZoglin:         return "zoglin";
		case mtZombie:         return "zombie";
		case mtZombieHorse:    return "zombie_horse";
		case mtZombiePigman:   return "zombified_piglin";
		case mtZombieVillager: return "zombie_villager";
		case mtInvalidType:    break;
	}
	UNREACHABLE("Tried to save unknown monster type");
}





std::string_view NamespaceSerializer::From(const BannerPattern a_Pattern)
{
	switch (a_Pattern)
	{
		case BannerPattern::BottomStripe: return "bs";
		case BannerPattern::TopStripe: return "ts";
		case BannerPattern::LeftStripe: return "ls";
		case BannerPattern::RightStripe: return "rs";
		case BannerPattern::CenterStripeVertical: return "cs";
		case BannerPattern::MiddleStripeHorizontal: return "ms";
		case BannerPattern::DownRightStripe: return "drs";
		case BannerPattern::DownLeftStripe: return "dls";
		case BannerPattern::SmallVerticalStripes: return "ss";
		case BannerPattern::DiagonalCross: return "cr";
		case BannerPattern::SquareCross: return "sc";
		case BannerPattern::LeftOfDiagonal: return "ld";
		case BannerPattern::RightOfUpsideDownDiagonal: return "rud";
		case BannerPattern::LeftOfUpsideDownDiagonal: return "lud";
		case BannerPattern::RightOfDiagonal: return "rd";
		case BannerPattern::VerticalHalfLeft: return "vh";
		case BannerPattern::VerticalHalfRight: return "vhr";
		case BannerPattern::HorizontalHalfTop: return "hh";
		case BannerPattern::HorizontalHalfBottom: return "hhb";
		case BannerPattern::BottomLeftCorner: return "bl";
		case BannerPattern::BottomRightCorner: return "br";
		case BannerPattern::TopLeftCorner: return "tl";
		case BannerPattern::TopRightCorner: return "tr";
		case BannerPattern::BottomTriangle: return "bt";
		case BannerPattern::TopTriangle: return "tt";
		case BannerPattern::BottomTriangleSawtooth: return "bts";
		case BannerPattern::TopTriangleSawtooth: return "tts";
		case BannerPattern::MiddleCircle: return "mc";
		case BannerPattern::MiddleRhombus: return "mr";
		case BannerPattern::Border: return "bo";
		case BannerPattern::CurlyBorder: return "cbo";
		case BannerPattern::Brick: return "bri";
		case BannerPattern::Gradient: return "gra";
		case BannerPattern::GradientUpsideDown: return "gru";
		case BannerPattern::Creeper: return "cre";
		case BannerPattern::Skull: return "sku";
		case BannerPattern::Flower: return "flo";
		case BannerPattern::Mojang: return "moj";
		case BannerPattern::Globe: return "glb";
		case BannerPattern::Piglin: return "pig";
	}
	UNREACHABLE("Unsupported banner pattern");
}





CustomStatistic NamespaceSerializer::ToCustomStatistic(const std::string_view ID)
{
	static const std::unordered_map<std::string_view, CustomStatistic> CustomStatistics
	{
		{ "animals_bred",                            CustomStatistic::AnimalsBred },
		{ "aviate_one_cm",                           CustomStatistic::AviateOneCm },
		{ "bell_ring",                               CustomStatistic::BellRing },
		{ "boat_one_cm",                             CustomStatistic::BoatOneCm },
		{ "clean_armor",                             CustomStatistic::CleanArmor },
		{ "clean_banner",                            CustomStatistic::CleanBanner },
		{ "clean_shulker_box",                       CustomStatistic::CleanShulkerBox },
		{ "climb_one_cm",                            CustomStatistic::ClimbOneCm },
		{ "crouch_one_cm",                           CustomStatistic::CrouchOneCm },
		{ "damage_absorbed",                         CustomStatistic::DamageAbsorbed },
		{ "damage_blocked_by_shield",                CustomStatistic::DamageBlockedByShield },
		{ "damage_dealt",                            CustomStatistic::DamageDealt },
		{ "damage_dealt_absorbed",                   CustomStatistic::DamageDealtAbsorbed },
		{ "damage_dealt_resisted",                   CustomStatistic::DamageDealtResisted },
		{ "damage_resisted",                         CustomStatistic::DamageResisted },
		{ "damage_taken",                            CustomStatistic::DamageTaken },
		{ "deaths",                                  CustomStatistic::Deaths },
		{ "drop",                                    CustomStatistic::Drop },
		{ "eat_cake_slice",                          CustomStatistic::EatCakeSlice },
		{ "enchant_item",                            CustomStatistic::EnchantItem },
		{ "fall_one_cm",                             CustomStatistic::FallOneCm },
		{ "fill_cauldron",                           CustomStatistic::FillCauldron },
		{ "fish_caught",                             CustomStatistic::FishCaught },
		{ "fly_one_cm",                              CustomStatistic::FlyOneCm },
		{ "horse_one_cm",                            CustomStatistic::HorseOneCm },
		{ "inspect_dispenser",                       CustomStatistic::InspectDispenser },
		{ "inspect_dropper",                         CustomStatistic::InspectDropper },
		{ "inspect_hopper",                          CustomStatistic::InspectHopper },
		{ "interact_with_anvil",                     CustomStatistic::InteractWithAnvil },
		{ "interact_with_beacon",                    CustomStatistic::InteractWithBeacon },
		{ "interact_with_blast_furnace",             CustomStatistic::InteractWithBlastFurnace },
		{ "interact_with_brewingstand",              CustomStatistic::InteractWithBrewingstand },
		{ "interact_with_campfire",                  CustomStatistic::InteractWithCampfire },
		{ "interact_with_cartography_table",         CustomStatistic::InteractWithCartographyTable },
		{ "interact_with_crafting_table",            CustomStatistic::InteractWithCraftingTable },
		{ "interact_with_furnace",                   CustomStatistic::InteractWithFurnace },
		{ "interact_with_grindstone",                CustomStatistic::InteractWithGrindstone },
		{ "interact_with_lectern",                   CustomStatistic::InteractWithLectern },
		{ "interact_with_loom",                      CustomStatistic::InteractWithLoom },
		{ "interact_with_smithing_table",            CustomStatistic::InteractWithSmithingTable },
		{ "interact_with_smoker",                    CustomStatistic::InteractWithSmoker },
		{ "interact_with_stonecutter",               CustomStatistic::InteractWithStonecutter },
		{ "jump",                                    CustomStatistic::Jump },
		{ "leave_game",                              CustomStatistic::LeaveGame },
		{ "minecart_one_cm",                         CustomStatistic::MinecartOneCm },
		{ "mob_kills",                               CustomStatistic::MobKills },
		{ "open_barrel",                             CustomStatistic::OpenBarrel },
		{ "open_chest",                              CustomStatistic::OpenChest },
		{ "open_enderchest",                         CustomStatistic::OpenEnderchest },
		{ "open_shulker_box",                        CustomStatistic::OpenShulkerBox },
		{ "pig_one_cm",                              CustomStatistic::PigOneCm },
		{ "play_noteblock",                          CustomStatistic::PlayNoteblock },
		{ "play_one_minute",                         CustomStatistic::PlayOneMinute },
		{ "play_record",                             CustomStatistic::PlayRecord },
		{ "player_kills",                            CustomStatistic::PlayerKills },
		{ "pot_flower",                              CustomStatistic::PotFlower },
		{ "raid_trigger",                            CustomStatistic::RaidTrigger },
		{ "raid_win",                                CustomStatistic::RaidWin },
		{ "sleep_in_bed",                            CustomStatistic::SleepInBed },
		{ "sneak_time",                              CustomStatistic::SneakTime },
		{ "sprint_one_cm",                           CustomStatistic::SprintOneCm },
		{ "strider_one_cm",                          CustomStatistic::StriderOneCm },
		{ "swim_one_cm",                             CustomStatistic::SwimOneCm },
		{ "talked_to_villager",                      CustomStatistic::TalkedToVillager },
		{ "target_hit",                              CustomStatistic::TargetHit },
		{ "time_since_death",                        CustomStatistic::TimeSinceDeath },
		{ "time_since_rest",                         CustomStatistic::TimeSinceRest },
		{ "traded_with_villager",                    CustomStatistic::TradedWithVillager },
		{ "trigger_trapped_chest",                   CustomStatistic::TriggerTrappedChest },
		{ "tune_noteblock",                          CustomStatistic::TuneNoteblock },
		{ "use_cauldron",                            CustomStatistic::UseCauldron },
		{ "walk_on_water_one_cm",                    CustomStatistic::WalkOnWaterOneCm },
		{ "walk_one_cm",                             CustomStatistic::WalkOneCm },
		{ "walk_under_water_one_cm",                 CustomStatistic::WalkUnderWaterOneCm },

		// Old ones just for compatibility
		{ "junk_fished",                             CustomStatistic::JunkFished },
		{ "treasure_fished",                         CustomStatistic::TreasureFished },

		// The old advancements
		{ "cuberite:achievement.openInventory",      CustomStatistic::AchOpenInventory },
		{ "cuberite:achievement.mineWood",           CustomStatistic::AchMineWood },
		{ "cuberite:achievement.buildWorkBench",     CustomStatistic::AchBuildWorkBench },
		{ "cuberite:achievement.buildPickaxe",       CustomStatistic::AchBuildPickaxe },
		{ "cuberite:achievement.buildFurnace",       CustomStatistic::AchBuildFurnace },
		{ "cuberite:achievement.acquireIron",        CustomStatistic::AchAcquireIron },
		{ "cuberite:achievement.buildHoe",           CustomStatistic::AchBuildHoe },
		{ "cuberite:achievement.makeBread",          CustomStatistic::AchMakeBread },
		{ "cuberite:achievement.bakeCake",           CustomStatistic::AchBakeCake },
		{ "cuberite:achievement.buildBetterPickaxe", CustomStatistic::AchBuildBetterPickaxe },
		{ "cuberite:achievement.cookFish",           CustomStatistic::AchCookFish },
		{ "cuberite:achievement.onARail",            CustomStatistic::AchOnARail },
		{ "cuberite:achievement.buildSword",         CustomStatistic::AchBuildSword },
		{ "cuberite:achievement.killEnemy",          CustomStatistic::AchKillEnemy },
		{ "cuberite:achievement.killCow",            CustomStatistic::AchKillCow },
		{ "cuberite:achievement.flyPig",             CustomStatistic::AchFlyPig },
		{ "cuberite:achievement.snipeSkeleton",      CustomStatistic::AchSnipeSkeleton },
		{ "cuberite:achievement.diamonds",           CustomStatistic::AchDiamonds },
		{ "cuberite:achievement.portal",             CustomStatistic::AchPortal },
		{ "cuberite:achievement.ghast",              CustomStatistic::AchGhast },
		{ "cuberite:achievement.blazeRod",           CustomStatistic::AchBlazeRod },
		{ "cuberite:achievement.potion",             CustomStatistic::AchPotion },
		{ "cuberite:achievement.theEnd",             CustomStatistic::AchTheEnd },
		{ "cuberite:achievement.theEnd2",            CustomStatistic::AchTheEnd2 },
		{ "cuberite:achievement.enchantments",       CustomStatistic::AchEnchantments },
		{ "cuberite:achievement.overkill",           CustomStatistic::AchOverkill },
		{ "cuberite:achievement.bookcase",           CustomStatistic::AchBookcase },
		{ "cuberite:achievement.exploreAllBiomes",   CustomStatistic::AchExploreAllBiomes },
		{ "cuberite:achievement.spawnWither",        CustomStatistic::AchSpawnWither },
		{ "cuberite:achievement.killWither",         CustomStatistic::AchKillWither },
		{ "cuberite:achievement.fullBeacon",         CustomStatistic::AchFullBeacon },
		{ "cuberite:achievement.breedCow",           CustomStatistic::AchBreedCow },
		{ "cuberite:achievement.diamondsToYou",      CustomStatistic::AchDiamondsToYou}
	};

	return CustomStatistics.at(ID);
}





std::pair<NamespaceSerializer::Namespace, std::string_view> NamespaceSerializer::SplitNamespacedID(const std::string_view ID)
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





eMonsterType NamespaceSerializer::ToMonsterType(const std::string_view a_ID)
{
	static const std::unordered_map<std::string_view, eMonsterType> MonsterTypes
	{
		{ "bat",              mtBat },
		{ "blaze",            mtBlaze },
		{ "cat",              mtCat },
		{ "cave_spider",      mtCaveSpider },
		{ "chicken",          mtChicken },
		{ "cod",              mtCod },
		{ "cow",              mtCow },
		{ "creeper",          mtCreeper },
		{ "dolphin",          mtDolphin },
		{ "donkey",           mtDonkey },
		{ "drowned",          mtDrowned },
		{ "elder_guardian",   mtElderGuardian },
		{ "ender_dragon",     mtEnderDragon },
		{ "enderman",         mtEnderman },
		{ "endermite",        mtEndermite },
		{ "evoker",           mtEvoker },
		{ "fox",              mtFox },
		{ "ghast",            mtGhast },
		{ "giant",            mtGiant },
		{ "guardian",         mtGuardian },
		{ "horse",            mtHorse },
		{ "hoglin",           mtHoglin },
		{ "husk",             mtHusk },
		{ "illusioner",       mtIllusioner },
		{ "iron_golem",       mtIronGolem },
		{ "llama",            mtLlama },
		{ "magma_cube",       mtMagmaCube },
		{ "mooshroom",        mtMooshroom },
		{ "mule",             mtMule },
		{ "ocelot",           mtOcelot },
		{ "panda",            mtPanda },
		{ "parrot",           mtParrot },
		{ "phantom",          mtPhantom },
		{ "pig",              mtPig },
		{ "piglin",           mtPiglin },
		{ "piglin_brute",     mtPiglinBrute },
		{ "pillager",         mtPillager },
		{ "polar_bear",       mtPolarBear },
		{ "pufferfish",       mtPufferfish },
		{ "rabbit",           mtRabbit },
		{ "ravager",          mtRavager },
		{ "salmon",           mtSalmon },
		{ "sheep",            mtSheep },
		{ "shulker",          mtShulker },
		{ "silverfish",       mtSilverfish },
		{ "skeleton",         mtSkeleton },
		{ "skeleton_horse",   mtSkeletonHorse },
		{ "slime",            mtSlime },
		{ "snow_golem",       mtSnowGolem },
		{ "spider",           mtSpider },
		{ "squid",            mtSquid },
		{ "stray",            mtStray },
		{ "strider",          mtStrider },
		{ "trader_llama",     mtTraderLlama },
		{ "tropical_fish",    mtTropicalFish },
		{ "turtle",           mtTurtle },
		{ "vex",              mtVex },
		{ "villager",         mtVillager },
		{ "vindicator",       mtVindicator },
		{ "wandering_trader", mtWanderingTrader },
		{ "witch",            mtWitch },
		{ "wither",           mtWither },
		{ "wither_skeleton",  mtWitherSkeleton },
		{ "wolf",             mtWolf },
		{ "zoglin",           mtZoglin },
		{ "zombie",           mtZombie },
		{ "zombie_horse",     mtZombieHorse },
		{ "zombified_piglin", mtZombiePigman },
		{ "zombie_villager",  mtZombieVillager },

		{ "snowman",          mtSnowGolem },
		{ "villager_golem",   mtIronGolem },
		{ "zombie_pigman",    mtZombiePigman },

		// Old names:
		{ "Bat",            mtBat },
		{ "Blaze",          mtBlaze },
		{ "CaveSpider",     mtCaveSpider },
		{ "Chicken",        mtChicken },
		{ "Cow",            mtCow },
		{ "Creeper",        mtCreeper },
		{ "EnderDragon",    mtEnderDragon },
		{ "Enderman",       mtEnderman },
		{ "Ghast",          mtGhast },
		{ "Giant",          mtGiant },
		{ "Guardian",       mtGuardian },
		{ "Horse",          mtHorse },
		{ "VillagerGolem",  mtIronGolem },
		{ "LavaSlime",      mtMagmaCube },
		{ "MushroomCow",    mtMooshroom },
		{ "Ozelot",         mtOcelot },
		{ "Pig",            mtPig },
		{ "Rabbit",         mtRabbit },
		{ "Sheep",          mtSheep },
		{ "Silverfish",     mtSilverfish },
		{ "Skeleton",       mtSkeleton },
		{ "Slime",          mtSlime },
		{ "SnowMan",        mtSnowGolem },
		{ "Spider",         mtSpider },
		{ "Squid",          mtSquid },
		{ "Villager",       mtVillager },
		{ "Witch",          mtWitch },
		{ "WitherBoss",     mtWither },
		{ "WitherSkeleton", mtWitherSkeleton },
		{ "Wolf",           mtWolf },
		{ "Zombie",         mtZombie },
		{ "PigZombie",      mtZombiePigman },
		{ "ZombieVillager", mtZombieVillager }
	};

	return MonsterTypes.at(a_ID);
}
