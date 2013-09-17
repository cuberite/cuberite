
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
		mtCreeper      = E_META_SPAWN_EGG_CREEPER,
		mtSkeleton     = E_META_SPAWN_EGG_SKELETON,
		mtSpider       = E_META_SPAWN_EGG_SPIDER,
		mtGiant        = E_META_SPAWN_EGG_GIANT,
		mtZombie       = E_META_SPAWN_EGG_ZOMBIE,
		mtSlime        = E_META_SPAWN_EGG_SLIME,
		mtGhast        = E_META_SPAWN_EGG_GHAST,
		mtZombiePigman = E_META_SPAWN_EGG_ZOMBIE_PIGMAN,
		mtEnderman     = E_META_SPAWN_EGG_ENDERMAN,
		mtCaveSpider   = E_META_SPAWN_EGG_CAVE_SPIDER,
		mtSilverfish   = E_META_SPAWN_EGG_SILVERFISH,
		mtBlaze        = E_META_SPAWN_EGG_BLAZE,
		mtMagmaCube    = E_META_SPAWN_EGG_MAGMA_CUBE,
		mtEnderDragon  = E_META_SPAWN_EGG_ENDER_DRAGON,
		mtWither       = E_META_SPAWN_EGG_WITHER,
		mtBat          = E_META_SPAWN_EGG_BAT,
		mtWitch        = E_META_SPAWN_EGG_WITCH,
		mtPig          = E_META_SPAWN_EGG_PIG,
		mtSheep        = E_META_SPAWN_EGG_SHEEP,
		mtCow          = E_META_SPAWN_EGG_COW,
		mtChicken      = E_META_SPAWN_EGG_CHICKEN,
		mtSquid        = E_META_SPAWN_EGG_SQUID,
		mtWolf         = E_META_SPAWN_EGG_WOLF,
		mtMooshroom    = E_META_SPAWN_EGG_MOOSHROOM,
		mtSnowGolem    = E_META_SPAWN_EGG_SNOW_GOLEM,
		mtOcelot       = E_META_SPAWN_EGG_OCELOT,
		mtIronGolem    = E_META_SPAWN_EGG_IRON_GOLEM,
		mtVillager     = E_META_SPAWN_EGG_VILLAGER,
	} ;
	
	// tolua_end
	
	float m_SightDistance;
	
	/** Creates the mob object.
	* If a_ConfigName is not empty, the configuration is loaded using GetMonsterConfig()
	* a_ProtocolMobType is the ID of the mob used in the protocol ( http://wiki.vg/Entities#Mobs , 2012_12_22)
	* a_SoundHurt and a_SoundDeath are assigned into m_SoundHurt and m_SoundDeath, respectively
	*/
	cMonster(const AString & a_ConfigName, char a_ProtocolMobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height);

	CLASS_PROTODEF(cMonster);
	
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	virtual void DoTakeDamage(TakeDamageInfo & a_TDI) override;
	
	virtual void KilledBy(cEntity * a_Killer) override;

	virtual void MoveToPosition(const Vector3f & a_Position);
	virtual bool ReachedDestination(void);
	
	char GetMobType(void) const {return m_MobType; }

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
	
	int GetMobType() { return m_MobType; }  // tolua_export
	
	int GetAttackRate(){return (int)m_AttackRate;}
	void SetAttackRate(int ar);
	void SetAttackRange(float ar);
	void SetAttackDamage(float ad);
	void SetSightDistance(float sd);
	
	/// Sets whether the mob burns in daylight. Only evaluated at next burn-decision tick
	void SetBurnsInDaylight(bool a_BurnsInDaylight) { m_BurnsInDaylight = a_BurnsInDaylight; }
	
	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	enum MPersonality{PASSIVE,AGGRESSIVE,COWARDLY} m_EMPersonality;
	
protected:
	
	cEntity * m_Target;
	float m_AttackRate;
	float idle_interval;

	Vector3f m_Destination;
	bool m_bMovingToDestination;
	bool m_bPassiveAggressive;

	float m_DestinationTime;

	float m_DestroyTimer;
	float m_Jump;

	char m_MobType;

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




