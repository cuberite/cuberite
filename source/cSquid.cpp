#include "cSquid.h"

cSquid::cSquid()
{
	m_MobType = 94;
	GetMonsterConfig("Squid");
}

cSquid::~cSquid()
{
}

bool cSquid::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cSquid" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cSquid::KilledBy( cEntity* a_Killer )
{
	//Drops 0-3 Ink Sacs
	cMonster::RandomDropItem(E_ITEM_DYE, 0, 3);

	cMonster::KilledBy( a_Killer );
}
