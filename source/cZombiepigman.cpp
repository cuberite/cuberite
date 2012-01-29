
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cZombiepigman.h"





cZombiepigman::cZombiepigman()
{
	m_MobType = 57;
	GetMonsterConfig("Zombiepigman");
}

cZombiepigman::~cZombiepigman()
{
}

bool cZombiepigman::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cZombiepigman" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cZombiepigman::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	//TODO Same as noticed in cSkeleton AND Do they really burn by sun?? :D In the neather is no sun :D
	if (GetWorld()->GetWorldTime() < (12000 + 1000) && GetMetaData() != BURNING ) { //if daylight
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}

void cZombiepigman::KilledBy( cEntity* a_Killer )
{
	cMonster::RandomDropItem(E_ITEM_ROTTEN_FLESH, 0, 1);
	cMonster::RandomDropItem(E_ITEM_GOLD_NUGGET, 0, 1);

	cMonster::KilledBy( a_Killer );
}
