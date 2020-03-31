
#pragma once





// tolua_begin
/** Identifies individual monster type. */
enum eMonsterType
{
	mtInvalidType = -1,

	mtBat,
	mtBlaze,
	mtCaveSpider,
	mtChicken,
	mtCow,
	mtCreeper,
	mtEnderDragon,
	mtEnderman,
	mtGhast,
	mtGiant,
	mtGuardian,
	mtHorse,
	mtIronGolem,
	mtMagmaCube,
	mtMooshroom,
	mtOcelot,
	mtPig,
	mtRabbit,
	mtSheep,
	mtSilverfish,
	mtSkeleton,
	mtSlime,
	mtSnowGolem,
	mtSpider,
	mtSquid,
	mtVillager,
	mtWitch,
	mtWither,
	mtWolf,
	mtZombie,
	mtZombiePigman,

	mtMax,  // The maximum value of the mob type, used for ToLua range-checks
} ;

// tolua_end
