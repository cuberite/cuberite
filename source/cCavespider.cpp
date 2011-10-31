#include "cCavespider.h"

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

cCavespider::cCavespider() : m_ChaseTime(999999) {
	m_bBurnable = true;
	m_EMPersonality = AGGRESSIVE;
	m_bPassiveAggressive = true;
	//m_AttackRate = 1;
	m_MobType = 59;
	GetMonsterConfig("Cavespider");
}

cCavespider::~cCavespider()
{
}

bool cCavespider::IsA( const char* a_EntityType )
{
    //LOG("IsA( cCavespider ) : %s", a_EntityType);
	if( strcmp( a_EntityType, "cCavespider" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cCavespider::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);
	m_EMPersonality = (GetWorld()->GetWorldTime() < (12000 + 1000) )? PASSIVE:AGGRESSIVE;
}

void cCavespider::KilledBy( cEntity* a_Killer )
{
	if( (rand() % 5) == 0 )
	{
		cPickup* Pickup = new cPickup( (int)(m_Pos->x*32), (int)(m_Pos->y*32), (int)(m_Pos->z*32), cItem( E_ITEM_EGG, 1 ) );
		Pickup->Initialize( GetWorld() );
	}
	if( (rand() % 1) == 0 )
	{
		cPickup* Pickup = new cPickup( (int)(m_Pos->x*32), (int)(m_Pos->y*32), (int)(m_Pos->z*32), cItem( E_ITEM_FEATHER, 1 ) );
		Pickup->Initialize( GetWorld() );
	}
	cMonster::KilledBy( a_Killer );
}

//What to do if in Idle State
void cCavespider::InStateIdle(float a_Dt) {
	cMonster::InStateIdle(a_Dt);	
}

//What to do if in Chasing State
void cCavespider::InStateChasing(float a_Dt) {
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

void cCavespider::InStateEscaping(float a_Dt) {
	cMonster::InStateEscaping(a_Dt);
}

void cCavespider::GetMonsterConfig(const char* pm_name) {
	LOG("I am gettin my attributes: %s", pm_name);
	cRoot::Get()->GetMonsterConfig()->Get()->AssignAttributes(this,pm_name);
}
