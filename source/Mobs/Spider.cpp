
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Spider.h"





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
	cItems Drops;
	AddRandomDropItem(Drops, 0, 2, E_ITEM_STRING);
	AddRandomDropItem(Drops, 0, 1, E_ITEM_SPIDER_EYE);
	m_World->SpawnItemPickups(Drops, m_Pos.x, m_Pos.y, m_Pos.z);

	cMonster::KilledBy( a_Killer );
}




