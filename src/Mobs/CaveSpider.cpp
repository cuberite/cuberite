#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "CaveSpider.h"
#include "../World.h"





cCaveSpider::cCaveSpider(void) :
	Super("CaveSpider", mtCaveSpider, "entity.spider.hurt", "entity.spider.death", "entity.spider.ambient", 0.7, 0.5)
{
}





void cCaveSpider::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	m_EMPersonality = (GetWorld()->GetTimeOfDay() < (12000 + 1000)) ? PASSIVE : AGGRESSIVE;
}





bool cCaveSpider::Attack(std::chrono::milliseconds a_Dt)
{
	if (!Super::Attack(a_Dt))
	{
		return false;
	}

	if (GetTarget()->IsPawn())
	{
		// TODO: Easy = no poison, Medium = 7 seconds, Hard = 15 seconds
		static_cast<cPawn *>(GetTarget())->AddEntityEffect(cEntityEffect::effPoison, 7 * 20, 0);
	}
	return true;
}





void cCaveSpider::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_STRING);
	if ((a_Killer != nullptr) && (a_Killer->IsPlayer() || a_Killer->IsA("cWolf")))
	{
		AddRandomUncommonDropItem(a_Drops, 33.0f, E_ITEM_SPIDER_EYE);
	}
}




