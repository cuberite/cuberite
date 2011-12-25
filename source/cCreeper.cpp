#include "cCreeper.h"

cCreeper::cCreeper()
{
	m_MobType = 50;
	GetMonsterConfig("Creeper");
}

cCreeper::~cCreeper()
{
}

bool cCreeper::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cCreeper" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}


void cCreeper::KilledBy( cEntity* a_Killer )
{
	cMonster::RandomDropItem(E_ITEM_GUNPOWDER, 0, 2);

	//TODO Check if killed by a skeleton then drop random music disk

	cMonster::KilledBy( a_Killer );
}