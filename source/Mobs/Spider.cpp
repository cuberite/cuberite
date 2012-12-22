
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Spider.h"





cSpider::cSpider(void) :
	super("Spider", 52, "mob.spider.say", "mob.spider.death")
{
}





void cSpider::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_STRING);
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_SPIDER_EYE);
}




