
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Spider.h"





cSpider::cSpider()
{
	m_MobType = 52;
	GetMonsterConfig("Spider");
}





bool cSpider::IsA(const char * a_EntityType)
{
	return ((strcmp(a_EntityType, "cSpider") == 0) || super::IsA(a_EntityType));
}





void cSpider::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_STRING);
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_SPIDER_EYE);
}




