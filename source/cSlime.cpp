
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSlime.h"

//TODO Implement sized slimes





cSlime::cSlime()
{
	m_MobType = 55;
	GetMonsterConfig("Slime");
}

cSlime::~cSlime()
{
}

bool cSlime::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cSlime" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cSlime::KilledBy( cEntity* a_Killer )
{
	//TODO: only when tiny
	cMonster::RandomDropItem(E_ITEM_SLIMEBALL, 0, 2);

	cMonster::KilledBy( a_Killer );
}