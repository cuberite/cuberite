
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Spider.h"





cSpider::cSpider()
{
	m_MobType = 52;
	m_SoundHurt = "mob.spider.say";
	m_SoundDeath = "mob.spider.death";
	GetMonsterConfig("Spider");
}





void cSpider::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_STRING);
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_SPIDER_EYE);
}




