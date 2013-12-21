
#pragma once

#include "../Entities/Pawn.h"
#include "../Defines.h"
#include "../BlockID.h"
#include "../Item.h"





class Vector3f;
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
		mfHostile  = 0, // Spider, Zombies ...
		mfPassive  = 1, // Cows, Pigs
		mfAmbient  = 2, // Bats
		mfWater    = 3, // Squid

		mfMaxplusone, // Nothing. Be sure this is the last and the others are in order
	} ;
	
	// tolua_end
	
	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	enum MPersonality{PASSIVE,AGGRESSIVE,COWARDLY} m_EMPersonality;
	
	float m_SightDistance;
	
	/** Creates the mob object.
	* If a_ConfigName is not empty, the configuration is loaded using GetMonsterConfig()
	* a_MobType is the type of the mob (also used in the protocol ( http://wiki.vg/Entities#Mobs , 2012_12_22))
	* a_SoundHurt and a_SoundDeath are assigned into m_SoundHurt and m_SoundDeath, respectively
	*/
	cMonster(const AString & a_ConfigName, eType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	CLASS_PROTODEF(cMonster);
	
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	
	virtual void KilledBy(cEntity * a_Killer) override;

	virtual void MoveToPosition(const Vector3f & a_Position);
	virtual bool ReachedDestination(void);
	
	// tolua_begin
	eType GetMobType(void) const {return m_MobType; }
	eFamily GetMobFamily(void) const;
	// tolua_end


	const char * GetState();
	void SetState(const AString & str);
	
	virtual void CheckEventSeePlayer(void);
	virtual void EventSeePlayer(cEntity * a_Player);
	virtual cPlayer * FindClosestPlayer();		// non static is easier. also virtual so other mobs can implement their own searching algo 
	
	/// Reads the monster configuration for the specified monster name and assigns it to this object.
	void GetMonsterConfig(const AString & a_Name);
	
	virtual void EventLosePlayer(void);
	virtual void CheckEventLostPlayer(void);
	
	virtual void InStateIdle    (float a_Dt);
	virtual void InStateChasing (float a_Dt);
	virtual void InStateEscaping(float a_Dt);
	
	virtual void Attack(float a_Dt);
	
	int GetAttackRate(){return (int)m_AttackRate;}
	void SetAttackRate(int ar);
	void SetAttackRange(float ar);
	void SetAttackDamage(float ad);
	void SetSightDistance(float sd);
	
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
	
	cEntity * m_Target;
	float m_AttackRate;
	float m_IdleInterval;

	Vector3f m_Destination;
	bool m_bMovingToDestination;
	bool m_bPassiveAggressive;

	float m_DestinationTime;

	float m_DestroyTimer;
	float m_Jump;

	eType m_MobType;

	AString m_SoundHurt;
	AString m_SoundDeath;

	float m_SeePlayerInterval;
	float m_AttackDamage;
	float m_AttackRange;
	float m_AttackInterval;
	
	bool m_BurnsInDaylight;

	void AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth = 0);
	
	void HandleDaylightBurning(cChunk & a_Chunk);

} ; // tolua_export




