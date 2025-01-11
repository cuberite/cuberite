
#pragma once





// tolua_begin
/** Identifies individual monster type. */
enum eMonsterType
{
	mtInvalidType = -1,

	mtAllay,	                    // Added in 1.18
	mtArmadillo,                    // Added in 1.20.5
	mtAxolotl,	                    // Added in 1.17
	mtBee,                          // Added in 1.15
	mtBat,
	mtBlaze,
	mtBogged,                       // Added in 1.21
	mtBreeze,                       // Added in 1.21
	mtCreaking,	                    // Added in 1.21.2
	mtCamel,                        // Added in 1.20
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
	mtFrog,		                    // Added in 1.18
	mtGhast,
	mtGiant,
	mtGlowSquid,	                // Added in 1.17
	mtGoat,	                        // Added in 1.17
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
	mtSniffer,                      // Added in 1.20
	mtSnowGolem,
	mtSpider,
	mtSquid,
	mtStray,                        // Added in 1.10
	mtStrider,                      // Added in 1.16
	mtTadpole,		                // Added in 1.18
	mtTraderLlama,                  // Added in 1.11
	mtTropicalFish,                 // Added in 1.13
	mtTurtle,                       // Added in 1.13
	mtVex,                          // Added in 1.11
	mtVillager,
	mtVindicator,                   // Added in 1.11
	mtWanderingTrader,              // Added in 1.14
	mtWarden,			            // Added in 1.18
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
