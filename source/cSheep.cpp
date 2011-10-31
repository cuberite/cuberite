#include "cSheep.h"

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



cSheep::cSheep()
	: m_ChaseTime( 999999 )

{
	//LOG("SPAWNING A Sheep!!!!!!!!!!!!!!!!!!!!!");
	m_EMPersonality = PASSIVE;
	m_MobType = 91;
	GetMonsterConfig("Sheep");
}

cSheep::~cSheep()
{
}

bool cSheep::IsA( const char* a_EntityType )
{
    //LOG("IsA( cSheep ) : %s", a_EntityType);
	if( strcmp( a_EntityType, "cSheep" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cSheep::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);
}

void cSheep::KilledBy( cEntity* a_Killer )
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
void cSheep::InStateIdle(float a_Dt) {
	cMonster::InStateIdle(a_Dt);
}

//What to do if in Chasing State
void cSheep::InStateChasing(float a_Dt) {
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

void cSheep::InStateEscaping(float a_Dt) {
	cMonster::InStateEscaping(a_Dt);
}
