#include "cZombiepigman.h"

#include "Vector3f.h"
#include "Vector3d.h"

#include "Defines.h"

#include "cRoot.h"
#include "cWorld.h"
#include "cPickup.h"
#include "cItem.h"
#include "cMonsterConfig.h"

#include "cMCLogger.h"

#ifndef _WIN32
#include <stdlib.h> // rand()
#include <cstring>
#endif

cZombiepigman::cZombiepigman() : m_ChaseTime(999999) {
	m_bBurnable = true;
	m_EMPersonality = PASSIVE;
	m_bPassiveAggressive = true;
	//m_AttackRate = 1;
	m_MobType = 57;
	GetMonsterConfig("Zombiepigman");
}

cZombiepigman::~cZombiepigman()
{
}

bool cZombiepigman::IsA( const char* a_EntityType )
{
    //LOG("IsA( cZombiepigman ) : %s", a_EntityType);
	if( strcmp( a_EntityType, "cZombiepigman" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cZombiepigman::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);
	if (GetWorld()->GetWorldTime() < (12000 + 1000) ) { //if daylight
		m_EMMetaState = BURNING; // BURN, BABY, BURN!  >:D
	}
}

void cZombiepigman::KilledBy( cEntity* a_Killer )
{
	//Drops 0-1 Rottenflesh
	cMonster::RandomDropItem(E_ITEM_ROTTEN_FLESH, 0, 2);

	//Drops 0-1 gold nuggets
	cMonster::RandomDropItem(E_ITEM_GOLD_NUGGET, 0, 2);

	cMonster::KilledBy( a_Killer );
}

//What to do if in Idle State
void cZombiepigman::InStateIdle(float a_Dt) {
	cMonster::InStateIdle(a_Dt);	
}

//What to do if in Chasing State
void cZombiepigman::InStateChasing(float a_Dt) {
	cMonster::InStateChasing(a_Dt);
	m_ChaseTime += a_Dt;
	if( m_Target )
	{
		Vector3f Pos = Vector3f( m_Pos );
		Vector3f Their = Vector3f( m_Target->GetPosition() );
		if( (Their - Pos).Length() <= m_AttackRange) {
			cMonster::Attack(a_Dt);
		}
		MoveToPosition( Their + Vector3f(0, 0.65f, 0) );
	} else if( m_ChaseTime > 5.f ) {
		m_ChaseTime = 0;
		m_EMState = IDLE;
	}	
} 

void cZombiepigman::InStateEscaping(float a_Dt) {
	cMonster::InStateEscaping(a_Dt);
}

void cZombiepigman::GetMonsterConfig(const char* pm_name) {
	LOG("I am gettin my attributes: %s", pm_name);
	cRoot::Get()->GetMonsterConfig()->Get()->AssignAttributes(this,pm_name);
}
