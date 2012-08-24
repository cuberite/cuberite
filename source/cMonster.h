
#pragma once

#include "cPawn.h"
#include "Defines.h"
#include "cWorld.h"
#include "BlockID.h"
#include "cItem.h"
#include "BlockID.h"





class Vector3f;
class cClientHandle;




class cMonster :	public cPawn											//tolua_export
{																			//tolua_export
public:

	cMonster();
	virtual ~cMonster();

	virtual bool IsA( const char* a_EntityType );

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	virtual void Tick(float a_Dt) override;
	
	virtual void HandlePhysics(float a_Dt);
	virtual void ReplicateMovement(void);

	virtual void TakeDamage(int a_Damage, cEntity *  a_Instigator) override;
	virtual void KilledBy(cEntity * a_Killer) override;

	virtual void MoveToPosition(const Vector3f & a_Position);
	virtual bool ReachedDestination(void);
	
	char GetMobType(void) const {return m_MobType; }

	const char * GetState();
	void SetState(const AString & str);
	
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
	
	virtual void Attack(float a_Dt);
	int GetMobType() {return m_MobType;}
	int GetAttackRate(){return (int)m_AttackRate;}
	void SetAttackRate(int ar);
	void SetAttackRange(float ar);
	void SetAttackDamage(float ad);
	void SetSightDistance(float sd);
	
	enum MState{ATTACKING, IDLE, CHASING, ESCAPING} m_EMState;
	enum MPersonality{PASSIVE,AGGRESSIVE,COWARDLY} m_EMPersonality;
	
protected:
	
	cEntity* m_Target;
	float m_AttackRate;
	float idle_interval;

	Vector3f m_Destination;
	bool m_bMovingToDestination;
	bool m_bPassiveAggressive;

	Vector3f m_Speed;
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

	void AddRandomDropItem(cItems & a_Drops, unsigned int a_Min, unsigned int a_Max, short a_Item, short a_ItemHealth = 0);
}; //tolua_export




