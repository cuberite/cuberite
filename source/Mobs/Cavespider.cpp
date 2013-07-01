
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cavespider.h"





cCavespider::cCavespider(void) :
	// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
	super("Cavespider", 59, "mob.spider.say", "mob.spider.death", 0.9, 0.6)
{
}





void cCavespider::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	// TODO: Check vanilla if cavespiders really get passive during the day / in daylight
	m_EMPersonality = (GetWorld()->GetTimeOfDay() < (12000 + 1000)) ? PASSIVE : AGGRESSIVE;
}





void cCavespider::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_STRING);
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_SPIDER_EYE);
}




