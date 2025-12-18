
#pragma once





// tolua_begin
/** Identifies individual monster type. */
enum eMonsterType
{
	mtInvalidType = -1,

	mtBat,
	mtBlaze,
	mtCat,                          // Added in 1.14
	mtCaveSpider,
	mtChicken,
	mtCod,                          // Added in 1.13
	mtCow,
	mtCreeper,
	mtDolphin,                      // Added in 1.13
	mtDonkey,                       // Added in 1.6
	mtDrowned,                      // Added in 1.13
	mtElderGuardian,                // Added in 1.13
	mtEnderDragon,
	mtEnderman,
	mtEndermite,                    // Added in 1.8
	mtEvoker,                       // Added in 1.11
	mtFox,                          // Added in 1.14
	mtGhast,
	mtGiant,
	mtGuardian,
	mtHorse,
	mtHoglin,                       // Added in 1.16
	mtHusk,                         // Added in 1.10
	mtIllusioner,                   // Added in 1.12
	mtIronGolem,
	mtLlama,                        // Added in 1.11
	mtMagmaCube,
	mtMooshroom,
	mtMule,                         // Added in 1.6
	mtOcelot,
	mtPanda,                        // Added in 1.14
	mtParrot,                       // Added in 1.12
	mtPhantom,                      // Added in 1.13
	mtPig,
	mtPiglin,                       // Added in 1.16
	mtPiglinBrute,                  // Added in 1.16
	mtPillager,                     // Added in 1.14
	mtPolarBear,                    // Added in 1.10
	mtPufferfish,                   // Added in 1.13
	mtRabbit,
	mtRavager,                      // Added in 1.14
	mtSalmon,                       // Added in 1.13
	mtSheep,
	mtShulker,                      // Added in 1.9
	mtSilverfish,
	mtSkeleton,
	mtSkeletonHorse,
	mtSlime,
	mtSnowGolem,
	mtSpider,
	mtSquid,
	mtStray,                        // Added in 1.10
	mtStrider,                      // Added in 1.16
	mtTraderLlama,                  // Added in 1.11
	mtTropicalFish,                 // Added in 1.13
	mtTurtle,                       // Added in 1.13
	mtVex,                          // Added in 1.11
	mtVillager,
	mtVindicator,                   // Added in 1.11
	mtWanderingTrader,              // Added in 1.14
	mtWitch,
	mtWither,
	mtWitherSkeleton,
	mtWolf,
	mtZoglin,                       // Added in 1.16
	mtZombie,
	mtZombieHorse,                  // Added in 1.6
	mtZombifiedPiglin,
	mtZombieVillager,

	// Synonyms:
	mtZombiePigman = mtZombifiedPiglin,
} ;

// tolua_end
