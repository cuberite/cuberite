
#pragma once

#include "../Entities/Pawn.h"
#include "../Defines.h"
#include "../BlockID.h"
#include "../Item.h"
#include "../Enchantments.h"
#include "Components/AllComponents.h"





class cClientHandle;
class cWorld;

class cNewMonster : public cPawn {
	typedef cPawn super;
public:
	/// This identifies individual monster type, as well as their network type-ID
	enum eType
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
		mtHorse        = E_META_SPAWN_EGG_HORSE,
		mtIronGolem    = E_META_SPAWN_EGG_IRON_GOLEM,
		mtMagmaCube    = E_META_SPAWN_EGG_MAGMA_CUBE,
		mtMooshroom    = E_META_SPAWN_EGG_MOOSHROOM,
		mtOcelot       = E_META_SPAWN_EGG_OCELOT,
		mtPig          = E_META_SPAWN_EGG_PIG,
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
	};

	cNewMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);
protected:
	eType m_MobType;

	AString m_SoundHurt;
	AString m_SoundDeath;
};
