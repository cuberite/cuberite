
#pragma once





// tolua_begin
/** Identifies individual monster type, as well as their network type-ID. */
enum eMonsterType
{
	mtInvalidType = -1,

	mtBat          = E_META_SPAWN_EGG_BAT,
	mtBlaze        = E_META_SPAWN_EGG_BLAZE,
	mtCaveSpider   = E_META_SPAWN_EGG_CAVE_SPIDER,
	mtChicken      = E_META_SPAWN_EGG_CHICKEN,
	mtCow          = E_META_SPAWN_EGG_COW,
	mtCreeper      = E_META_SPAWN_EGG_CREEPER,
	mtEnderDragon  = E_META_SPAWN_EGG_ENDER_DRAGON,
	mtEnderman     = E_META_SPAWN_EGG_ENDERMAN,
	mtGhast        = E_META_SPAWN_EGG_GHAST,
	mtGiant        = E_META_SPAWN_EGG_GIANT,
	mtGuardian     = E_META_SPAWN_EGG_GUARDIAN,
	mtHorse        = E_META_SPAWN_EGG_HORSE,
	mtIronGolem    = E_META_SPAWN_EGG_IRON_GOLEM,
	mtMagmaCube    = E_META_SPAWN_EGG_MAGMA_CUBE,
	mtMooshroom    = E_META_SPAWN_EGG_MOOSHROOM,
	mtOcelot       = E_META_SPAWN_EGG_OCELOT,
	mtPig          = E_META_SPAWN_EGG_PIG,
	mtRabbit       = E_META_SPAWN_EGG_RABBIT,
	mtSheep        = E_META_SPAWN_EGG_SHEEP,
	mtSilverfish   = E_META_SPAWN_EGG_SILVERFISH,
	mtSkeleton     = E_META_SPAWN_EGG_SKELETON,
	mtSlime        = E_META_SPAWN_EGG_SLIME,
	mtSnowGolem    = E_META_SPAWN_EGG_SNOW_GOLEM,
	mtSpider       = E_META_SPAWN_EGG_SPIDER,
	mtSquid        = E_META_SPAWN_EGG_SQUID,
	mtVillager     = E_META_SPAWN_EGG_VILLAGER,
	mtWitch        = E_META_SPAWN_EGG_WITCH,
	mtWither       = E_META_SPAWN_EGG_WITHER,
	mtWolf         = E_META_SPAWN_EGG_WOLF,
	mtZombie       = E_META_SPAWN_EGG_ZOMBIE,
	mtZombiePigman = E_META_SPAWN_EGG_ZOMBIE_PIGMAN,
	mtMax          = 120,  // This is just a hotfix for https://forum.cuberite.org/thread-1616.html. Tolua is too bad to find the highest value, so this is needed.
} ;

// tolua_end




