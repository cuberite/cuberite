#pragma once
#include "cPawn.h"
#include "Defines.h"
#include "cWorld.h"
#include "BlockID.h"
#include <string.h>

class Vector3f;
class cClientHandle;
class cMonster :	public cPawn											//tolua_export
{																			//tolua_export
public:

	cMonster();
	virtual ~cMonster();

	virtual bool IsA( const char* a_EntityType );

	virtual void SpawnOn( cClientHandle* a_Target );

	virtual void Tick(float a_Dt);
	virtual void HandlePhysics(float a_Dt);
	virtual void ReplicateMovement();

	virtual void TakeDamage( int a_Damage, cEntity* a_Instigator );
	virtual void KilledBy( cEntity* a_Killer );

	virtual void MoveToPosition( const Vector3f & a_Position );
	virtual bool ReachedDestination();

	const char *GetState();
	void SetState(const char* str);
	static void ListMonsters();
	
	virtual void CheckEventSeePlayer();
	virtual void EventSeePlayer(cEntity *);
	float m_SightDistance;
	virtual cPlayer *FindClosestPlayer();		//non static is easier. also virtual so other mobs can implement their own searching algo 
	virtual void GetMonsterConfig(const char* pm_name);
	virtual void EventLosePlayer();
	virtual void CheckEventLostPlayer();
	
	virtual void InStateIdle(float a_Dt);
	virtual void InStateChasing(float a_Dt);
	virtual void InStateEscaping(float a_Dt);
	virtual void InStateBurning(float a_Dt);
	
	virtual void Attack(float a_Dt);
	int GetMobType() {return m_MobType;}
	int GetAttackRate(){return (int)m_AttackRate;}
	void SetAttackRate(int ar);
	void SetAttackRange(float ar);
	void SetAttackDamage(float ad);
	void SetSightDistance(float sd);
	virtual void CheckMetaDataBurn();
	
	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	enum MPersonality{PASSIVE,AGGRESSIVE,COWARDLY} m_EMPersonality;
	enum MMetaState{NORMAL,BURNING,CROUCHED,RIDING} m_EMMetaState;
	
protected:
	
	cEntity* m_Target;
	float m_AttackRate;
	float idle_interval;

	Vector3f* m_Destination;
	bool m_bMovingToDestination;
	bool m_bPassiveAggressive;
	bool m_bBurnable;

	Vector3f* m_Speed;
	float m_DestinationTime;

	float m_Gravity;
	bool m_bOnGround;

	float m_DestroyTimer;
	float m_Jump;

	char m_MobType;

	float m_SeePlayerInterval;
	float m_AttackDamage;
	float m_AttackRange;
	float m_AttackInterval;
	float m_FireDamageInterval;
	float m_BurnPeriod;

	void DropItem(ENUM_ITEM_ID a_Item, unsigned int a_Count);
	void RandomDropItem(ENUM_ITEM_ID a_Item, unsigned int a_Min, unsigned int a_Max);

}; //tolua_export
