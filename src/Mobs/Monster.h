
#pragma once

#include "../Entities/Pawn.h"
#include "../Defines.h"
#include "../BlockID.h"
#include "../Item.h"
#include "../Enchantments.h"





class cClientHandle;
class cWorld;




// tolua_begin
class cMonster :
	public cPawn
{
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
	} ;

	enum eFamily
	{
		mfHostile  = 0,  // Spider, Zombies ...
		mfPassive  = 1,  // Cows, Pigs
		mfAmbient  = 2,  // Bats
		mfWater    = 3,  // Squid

		mfNoSpawn,
		mfUnhandled,  // Nothing. Be sure this is the last and the others are in order
	} ;
	
	// tolua_end
	
	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	enum MPersonality{PASSIVE, AGGRESSIVE, COWARDLY} m_EMPersonality;
	
	/** Creates the mob object.
	If a_ConfigName is not empty, the configuration is loaded using GetMonsterConfig()
	a_MobType is the type of the mob (also used in the protocol ( http://wiki.vg/Entities#Mobs 2012_12_22))
	a_SoundHurt and a_SoundDeath are assigned into m_SoundHurt and m_SoundDeath, respectively
	*/
	cMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	CLASS_PROTODEF(cMonster);
	
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	
	virtual void KilledBy(TakeDamageInfo & a_TDI) override;

	virtual void MoveToPosition(const Vector3d & a_Position);  // tolua_export
	virtual bool ReachedDestination(void);
	
	// tolua_begin
	eType GetMobType(void) const {return m_MobType; }
	eFamily GetMobFamily(void) const;
	// tolua_end
	
	virtual void CheckEventSeePlayer(void);
	virtual void EventSeePlayer(cEntity * a_Player);
	
	/// Reads the monster configuration for the specified monster name and assigns it to this object.
	void GetMonsterConfig(const AString & a_Name);
	
	/** Returns whether this mob is undead (skeleton, zombie, etc.) */
	virtual bool IsUndead(void);
	
	virtual void EventLosePlayer(void);
	virtual void CheckEventLostPlayer(void);
	
	virtual void InStateIdle    (float a_Dt);
	virtual void InStateChasing (float a_Dt);
	virtual void InStateEscaping(float a_Dt);
	
	int GetAttackRate() { return (int)m_AttackRate; }
	void SetAttackRate(float a_AttackRate) { m_AttackRate = a_AttackRate; }
	void SetAttackRange(int a_AttackRange) { m_AttackRange = a_AttackRange; }
	void SetAttackDamage(int a_AttackDamage) { m_AttackDamage = a_AttackDamage; }
	void SetSightDistance(int a_SightDistance) { m_SightDistance = a_SightDistance; }
	
	float GetDropChanceWeapon() { return m_DropChanceWeapon; }
	float GetDropChanceHelmet() { return m_DropChanceHelmet; }
	float GetDropChanceChestplate() { return m_DropChanceChestplate; }
	float GetDropChanceLeggings() { return m_DropChanceLeggings; }
	float GetDropChanceBoots() { return m_DropChanceBoots; }
	bool CanPickUpLoot() { return m_CanPickUpLoot; }
	void SetDropChanceWeapon(float a_DropChanceWeapon) { m_DropChanceWeapon = a_DropChanceWeapon; }
	void SetDropChanceHelmet(float a_DropChanceHelmet) { m_DropChanceHelmet = a_DropChanceHelmet; }
	void SetDropChanceChestplate(float a_DropChanceChestplate) { m_DropChanceChestplate = a_DropChanceChestplate; }
	void SetDropChanceLeggings(float a_DropChanceLeggings) { m_DropChanceLeggings = a_DropChanceLeggings; }
	void SetDropChanceBoots(float a_DropChanceBoots) { m_DropChanceBoots = a_DropChanceBoots; }
	void SetCanPickUpLoot(bool a_CanPickUpLoot) { m_CanPickUpLoot = a_CanPickUpLoot; }
	
	/// Sets whether the mob burns in daylight. Only evaluated at next burn-decision tick
	void SetBurnsInDaylight(bool a_BurnsInDaylight) { m_BurnsInDaylight = a_BurnsInDaylight; }

	// Overridables to handle ageable mobs
	virtual bool IsBaby    (void) const { return false; }
	virtual bool IsTame    (void) const { return false; }
	virtual bool IsSitting (void) const { return false; }
	
	// tolua_begin
	
	/// Translates MobType enum to a string, empty string if unknown
	static AString MobTypeToString(eType a_MobType);
	
	/// Translates MobType string to the enum, mtInvalidType if not recognized
	static eType StringToMobType(const AString & a_MobTypeName);
	
	/// Returns the mob family based on the type
	static eFamily FamilyFromType(eType a_MobType);

	/// Returns the spawn delay (number of game ticks between spawn attempts) for the given mob family
	static int GetSpawnDelay(cMonster::eFamily a_MobFamily);

	// tolua_end
	
	/** Creates a new object of the specified mob.
	a_MobType is the type of the mob to be created
	Asserts and returns null if mob type is not specified
	*/
	static cMonster * NewMonsterFromType(eType a_MobType);

protected:
	
	/* ======= PATHFINDING ======= */

	/** A pointer to the entity this mobile is aiming to reach */
	cEntity * m_Target;
	/** Coordinates of the next position that should be reached */
	Vector3d m_Destination;
	/** Coordinates for the ultimate, final destination. */
	Vector3d m_FinalDestination;
	/** Returns if the ultimate, final destination has been reached */
	bool ReachedFinalDestination(void);

	/** Stores if mobile is currently moving towards the ultimate, final destination */
	bool m_bMovingToDestination;
	
	/** Finds the first non-air block position (not the highest, as cWorld::GetHeight does)
		If current Y is nonsolid, goes down to try to find a solid block, then returns that + 1
		If current Y is solid, goes up to find first nonsolid block, and returns that */
	int FindFirstNonAirBlockPosition(double a_PosX, double a_PosZ);
	/** Returns if a monster can actually reach a given height by jumping or walking */
	inline bool IsNextYPosReachable(int a_PosY)
	{
		return (
			(a_PosY <= POSY_TOINT) ||
			DoesPosYRequireJump(a_PosY)
			);
	}
	/** Returns if a monster can reach a given height by jumping */
	inline bool DoesPosYRequireJump(int a_PosY)
	{
		return ((a_PosY > POSY_TOINT) && (a_PosY == POSY_TOINT + 1));
	}

	/** A semi-temporary list to store the traversed coordinates during active pathfinding so we don't visit them again */
	std::vector<Vector3i> m_TraversedCoordinates;
	/** Returns if coordinate is in the traversed list */
	bool IsCoordinateInTraversedList(Vector3i a_Coords);

	/** Finds the next place to go
		This is based on the ultimate, final destination and the current position, as well as the traversed coordinates, and any environmental hazards */
	void TickPathFinding(void);
	/** Finishes a pathfinding task, be it due to failure or something else */
	inline void FinishPathFinding(void)
	{
		m_TraversedCoordinates.clear();
		m_bMovingToDestination = false;
	}
	/** Sets the body yaw and head yaw/pitch based on next/ultimate destinations */
	void SetPitchAndYawFromDestination(void);

	/* =========================== */
	/* ========= FALLING ========= */

	virtual void HandleFalling(void);
	int m_LastGroundHeight;

	/* =========================== */

	float m_IdleInterval;
	float m_DestroyTimer;

	eType m_MobType;

	AString m_SoundHurt;
	AString m_SoundDeath;

	float m_AttackRate;
	int m_AttackDamage;
	int m_AttackRange;
	float m_AttackInterval;
	int m_SightDistance;
	
	float m_DropChanceWeapon;
	float m_DropChanceHelmet;
	float m_DropChanceChestplate;
	float m_DropChanceLeggings;
	float m_DropChanceBoots;
	bool m_CanPickUpLoot;
	
	void HandleDaylightBurning(cChunk & a_Chunk);
	bool m_BurnsInDaylight;

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
	

} ;  // tolua_export




