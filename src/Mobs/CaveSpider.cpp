#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "CaveSpider.h"
#include "../World.h"





cCaveSpider::cCaveSpider(void) :
	super("CaveSpider", mtCaveSpider, "mob.spider.say", "mob.spider.death", 0.7, 0.5)
{
}





void cCaveSpider::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	m_EMPersonality = (GetWorld()->GetTimeOfDay() < (12000 + 1000)) ? PASSIVE : AGGRESSIVE;
}





void cCaveSpider::Attack(float a_Dt)
{
	super::Attack(a_Dt);
	
	if (m_Target->IsPawn())
	{
		// TODO: Easy = no poison, Medium = 7 seconds, Hard = 15 seconds
		((cPawn *) m_Target)->AddEntityEffect(cEntityEffect::effPoison, 7 * 20, 0);
	}
}





void cCaveSpider::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_STRING);
	if ((a_Killer != NULL) && (a_Killer->IsPlayer() || a_Killer->IsA("cWolf")))
	{
		AddRandomUncommonDropItem(a_Drops, 33.0f, E_ITEM_SPIDER_EYE);
	}
}




