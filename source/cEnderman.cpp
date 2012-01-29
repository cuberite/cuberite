
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cEnderman.h"

cEnderman::cEnderman()
{
	m_MobType = 58;
	GetMonsterConfig("Enderman");
}

cEnderman::~cEnderman()
{
}

bool cEnderman::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cEnderman" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cEnderman::Tick(float a_Dt)
{
	cMonster::Tick(a_Dt);

	//TODO Same as stated in cSkeleton
	if (GetWorld()->GetWorldTime() < (12000 + 1000) && GetMetaData() != BURNING) { //if daylight
		SetMetaData(BURNING); // BURN, BABY, BURN!  >:D
	}
}

void cEnderman::KilledBy( cEntity* a_Killer )
{
	//Drops 0-1 Enderpearl
	cMonster::RandomDropItem(E_ITEM_ENDER_PEARL, 0, 1);

	cMonster::KilledBy( a_Killer );
}
