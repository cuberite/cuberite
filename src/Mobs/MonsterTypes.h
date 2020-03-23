
#pragma once





// tolua_begin
/** Identifies individual monster type, as well as their network type-ID. */
enum eMonsterType
{
	mtInvalidType = -1,

	mtBat          = 3,
	mtBlaze        = 4,
	mtCaveSpider   = 6,
	mtChicken      = 7,
	mtCow          = 9,
	mtCreeper      = 10,
	mtEnderDragon  = 17,
	mtEnderman     = 18,
	mtGhast        = 26,
	mtGiant        = 27,
	mtGuardian     = 28,
	mtHorse        = 29,
	mtIronGolem    = 80,
	mtMagmaCube    = 38,
	mtMooshroom    = 47,
	mtOcelot       = 48,
	mtPig          = 51,
	mtRabbit       = 56,
	mtSheep        = 58,
	mtSilverfish   = 61,
	mtSkeleton     = 62,
	mtSlime        = 64,
	mtSnowGolem    = 66,
	mtSpider       = 69,
	mtSquid        = 70,
	mtVillager     = 79,
	mtWitch        = 82,
	mtWither       = 83,
	mtWolf         = 86,
	mtZombie       = 87,
	mtZombiePigman = 53,
	mtMax          = 120,  // This is just a hotfix for https://forum.cuberite.org/thread-1616.html. Tolua is too bad to find the highest value, so this is needed.
} ;

// tolua_end




