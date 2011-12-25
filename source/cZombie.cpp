#include "cZombie.h"

cZombie::cZombie()
{
	m_MobType = 54;
	GetMonsterConfig("Zombie");
}

cZombie::~cZombie()
{
}

bool cZombie::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cZombie" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cZombie::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	//TODO Same as in cSkeleton :D
	if (GetWorld()->GetWorldTime() < (12000 + 1000) ) { //if daylight
		m_EMMetaState = BURNING; // BURN, BABY, BURN!  >:D
	}
}

void cZombie::KilledBy( cEntity* a_Killer )
{
	cMonster::RandomDropItem(E_ITEM_ROTTEN_FLESH, 0, 2);

	cMonster::KilledBy( a_Killer );
}
