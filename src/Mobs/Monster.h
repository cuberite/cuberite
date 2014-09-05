
#pragma once

#include "../Entities/Pawn.h"
#include "../Defines.h"
#include "../BlockID.h"
#include "../Item.h"
#include "../Enchantments.h"
#include "Components/AllComponents.h"





class cClientHandle;
class cWorld;


class cMonster : public cPawn {
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

	enum eFamily
	{
		mfHostile  = 0,  // Spider, Zombies ...
		mfPassive  = 1,  // Cows, Pigs
		mfAmbient  = 2,  // Bats
		mfWater    = 3,  // Squid

		mfNoSpawn,
		mfUnhandled,  // Nothing. Be sure this is the last and the others are in order
	} ;


protected:
	eType  m_MobType;

	cAIComponent * m_AI;
	cAttackComponent * m_Attack;
	cEnvironmentComponent * m_Environment;
	cMovementComponent * m_Movement;
public:
	cMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);
	~cMonster() { delete m_AI; delete m_Attack; delete m_Environment; delete m_Movement;}
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual void Tick(float a_Dt, cChunk & a_Chunk);

	// Type Function
	virtual bool IsBaby    (void) const { return false; }
	virtual bool IsTame    (void) const { return false; }
	virtual bool IsUndead  (void) const { return false; }

	// Get Functions
	cAIComponent * GetAIComponent() { return m_AI; }
	cAttackComponent * GetAttackComponent() { return m_Attack; }
	cEnvironmentComponent * GetEnvironmentComponent() { return m_Environment; }
	cMovementComponent * GetMovementComponent() { return m_Movement; }

	// Get Functions - Temporary

	AString GetOwnerName  (void) const { return m_OwnerName; }
	AString GetOwnerUUID  (void) const { return m_OwnerUUID; }
	float   GetDropChanceBoots() { return m_DropChanceBoots; }
	float   GetDropChanceHelmet() { return m_DropChanceHelmet; }
	float   GetDropChanceChestplate() { return m_DropChanceChestplate; }
	float   GetDropChanceLeggings() { return m_DropChanceLeggings; }
	float   GetDropChanceWeapon() { return m_DropChanceWeapon; }

	eType  GetMobType() const { return m_MobType; }
	eFamily GetMobFamily() { return mfPassive; }


	// Set Functions - Temporary
	void    SetDropChanceBoots(float a_Chance) {  m_DropChanceBoots = a_Chance; }
	void    SetDropChanceHelmet(float a_Chance) {  m_DropChanceHelmet = a_Chance; }
	void    SetDropChanceChestplate(float a_Chance) {  m_DropChanceChestplate = a_Chance; }
	void    SetDropChanceLeggings(float a_Chance) {  m_DropChanceLeggings = a_Chance; }
	void    SetDropChanceWeapon(float a_Chance) { m_DropChanceWeapon = a_Chance; }
	void    SetIsTame(bool m_Tame) {}
	void    SetOwner(AString a_Name, AString a_UUID) { m_OwnerName = a_Name; m_OwnerUUID = a_UUID; }

	// Ability Functions
	bool CanPickUpLoot() { return false; }
	void SetCanPickUpLoot(bool a_Looting) {}

	// Static Functions
	
	/// Translates MobType enum to a string, empty string if unknown
	static AString MobTypeToString(eType a_MobType);
	
	/// Translates MobType string to the enum, mtInvalidType if not recognized
	static eType StringToMobType(const AString & a_MobTypeName);
	
	/// Returns the mob family based on the type
	static eFamily FamilyFromType(eType a_MobType);

	/// Returns the spawn delay (number of game ticks between spawn attempts) for the given mob family
	static int GetSpawnDelay(cMonster::eFamily a_MobFamily);
	
	/** Creates a new object of the specified mob.
	a_MobType is the type of the mob to be created
	Asserts and returns null if mob type is not specified
	*/
	static cMonster * NewMonsterFromType(eType a_MobType);
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

	// Temporary placement till I figure out where to put it
	float m_DropChanceWeapon;
	float m_DropChanceHelmet;
	float m_DropChanceChestplate;
	float m_DropChanceLeggings;
	float m_DropChanceBoots;


	AString m_OwnerName;
	AString m_OwnerUUID;

	
	float m_DestroyTimer;
};
