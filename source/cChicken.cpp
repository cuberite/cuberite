#include "cChicken.h"

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


// TODO: Drop egg every 5-10 minutes


cChicken::cChicken()
	: m_ChaseTime( 999999 )

{
	//LOG("SPAWNING A CHICKEN!!!!!!!!!!!!!!!!!!!!!");
	m_EMPersonality = PASSIVE;
	m_MobType = 93;
	GetMonsterConfig("Chicken");
}

cChicken::~cChicken()
{
}

bool cChicken::IsA( const char* a_EntityType )
{
    //LOG("IsA( cChicken ) : %s", a_EntityType);
	if( strcmp( a_EntityType, "cChicken" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cChicken::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);
}

void cChicken::KilledBy( cEntity* a_Killer )
{
	//Drops 0-2 Feathers
	cMonster::RandomDropItem(E_ITEM_FEATHER, 0, 2);

	// Raw Chicken 
	// TODO: (Check wheather it is burning to drop cooked Chicken)
	//Drops 0-2 Lether
	cMonster::DropItem(E_ITEM_RAW_CHICKEN, 1);


	cMonster::KilledBy( a_Killer );
}

//What to do if in Idle State
void cChicken::InStateIdle(float a_Dt) {
	cMonster::InStateIdle(a_Dt);
}

//What to do if in Chasing State
void cChicken::InStateChasing(float a_Dt) {
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

void cChicken::InStateEscaping(float a_Dt) {
	cMonster::InStateEscaping(a_Dt);
}
