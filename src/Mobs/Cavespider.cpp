
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cavespider.h"
#include "../World.h"





cCavespider::cCavespider(void) :
	super("Cavespider", mtCaveSpider, "mob.spider.say", "mob.spider.death", 0.7, 0.5)
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
	int LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(21);
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_STRING);
	if (a_Killer->IsA("cPlayer") || a_Killer->IsA("cWolf"))
	{
		AddRandomUncommonDropItem(a_Drops, 33.0f, E_ITEM_SPIDER_EYE);
	}
}




