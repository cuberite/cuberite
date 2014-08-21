
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

protected:
	eType  m_MobType;
public:
	cNewMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	eType  GetMobType() const { return m_MobType; }
	virtual bool IsBaby    (void) const { return false; }
	virtual bool IsTame    (void) const { return false; }
protected:



	/** Adds a random number of a_Item between a_Min and a_Max to itemdrops a_Drops*/
	void AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth = 0);
	
	/** Adds a item a_Item with the chance of a_Chance (in percent) to itemdrops a_Drops*/
	void AddRandomUncommonDropItem(cItems & a_Drops, float a_Chance, short a_Item, short a_ItemHealth = 0);
	
	/** Adds one rare item out of the list of rare items a_Items modified by the looting level a_LootingLevel(I-III or custom) to the itemdrop a_Drops*/
	void AddRandomRareDropItem(cItems & a_Drops, cItems & a_Items, short a_LootingLevel);
	
	/** Adds armor that is equipped with the chance saved in m_DropChance[...] (this will be greter than 1 if piccked up or 0.085 + (0.01 per LootingLevel) if born with) to the drop*/
	void AddRandomArmorDropItem(cItems & a_Drops, short a_LootingLevel);
	
	/** Adds weapon that is equipped with the chance saved in m_DropChance[...] (this will be greter than 1 if piccked up or 0.085 + (0.01 per LootingLevel) if born with) to the drop*/
	void AddRandomWeaponDropItem(cItems & a_Drops, short a_LootingLevel);

	AString m_SoundHurt;
	AString m_SoundDeath;

	cAIComponent * m_AI;
	cAttackComponent * m_Attack;
	cEnvironmentComponent * m_Environment;
	cMovementComponent * m_Movement;

	// Temporary placement till I figure out where to put it
	float m_DropChanceWeapon;
	float m_DropChanceHelmet;
	float m_DropChanceChestplate;
	float m_DropChanceLeggings;
	float m_DropChanceBoots;
};
