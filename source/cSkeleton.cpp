#include "cSkeleton.h"

cSkeleton::cSkeleton()
{
	m_MobType = 51;
	GetMonsterConfig("Skeleton");
}

cSkeleton::~cSkeleton()
{
}

bool cSkeleton::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cSkeleton" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cSkeleton::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	//TODO Outsource
	//TODO should do lightcheck, not daylight -> mobs in the dark don´t burn 
	if (GetWorld()->GetWorldTime() < (12000 + 1000) ) { //if daylight
		m_EMMetaState = BURNING; // BURN, BABY, BURN!  >:D
	}
}

void cSkeleton::KilledBy( cEntity* a_Killer )
{
	cMonster::RandomDropItem(E_ITEM_ARROW, 0, 2);

	cMonster::RandomDropItem(E_ITEM_BONE, 0, 2);

	cMonster::KilledBy( a_Killer );
}
