#include "cCow.h"

#include "Vector3f.h"
#include "Vector3d.h"

#include "Defines.h"

#include "cRoot.h"
#include "cWorld.h"
#include "cPickup.h"
#include "cItem.h"

#include "cMCLogger.h"

#ifndef _WIN32
#include <stdlib.h> // rand()
#include <cstring>
#endif

#include <string>

//TODO: Milk Cow

cCow::cCow()
	: m_ChaseTime( 999999 )

{
	//LOG("SPAWNING A Cow!!!!!!!!!!!!!!!!!!!!!");
	m_EMPersonality = PASSIVE;
	m_MobType = 92;
	GetMonsterConfig("Cow");
}

cCow::~cCow()
{
}

bool cCow::IsA( const char* a_EntityType )
{
    //LOG("IsA( cCow ) : %s", a_EntityType);
	if( strcmp( a_EntityType, "cCow" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cCow::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);
}

void cCow::KilledBy( cEntity* a_Killer )
{
	//Drops 0-2 Lether
	cMonster::RandomDropItem(E_ITEM_LEATHER, 0, 2);

	//Drops 1-3 Raw beefs
	cMonster::RandomDropItem(E_ITEM_LEATHER, 1, 3);
	
	//TODO: Check wheather burning and drop 1-3 steak instead

	cMonster::KilledBy( a_Killer );
}

//What to do if in Idle State
void cCow::InStateIdle(float a_Dt) {
	cMonster::InStateIdle(a_Dt);
}

//What to do if in Chasing State
void cCow::InStateChasing(float a_Dt) {
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

void cCow::InStateEscaping(float a_Dt) {
	cMonster::InStateEscaping(a_Dt);
}
