
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cSpider.h"





cSpider::cSpider()
{
	m_MobType = 52;
	GetMonsterConfig("Spider");
}

cSpider::~cSpider()
{
}

bool cSpider::IsA( const char* a_EntityType )
{
	if( strcmp( a_EntityType, "cSpider" ) == 0 ) return true;
	return cMonster::IsA( a_EntityType );
}

void cSpider::KilledBy( cEntity* a_Killer )
{
	cMonster::RandomDropItem(E_ITEM_STRING, 0, 2);

	cMonster::RandomDropItem(E_ITEM_SPIDER_EYE, 0, 1);

	cMonster::KilledBy( a_Killer );
}
