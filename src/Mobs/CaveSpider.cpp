#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "CaveSpider.h"
#include "../World.h"
#include "Mobs/Behaviors/BehaviorAttackerMelee.h"


void CaveSpiderPostAttack(cBehaviorAttackerMelee & a_Behavior, cMonster & a_Attacker, cPawn & a_Attacked)
{
	UNUSED(a_Behavior);
	UNUSED(a_Attacker);
	// TODO: Easy = no poison, Medium = 7 seconds, Hard = 15 seconds
	a_Attacked.AddEntityEffect(cEntityEffect::effPoison, 7 * 20, 0);
}





cCaveSpider::cCaveSpider(void)
	: super(mtCaveSpider, "entity.spider.hurt", "entity.spider.death", 0.7, 0.5)
	, m_BehaviorAttackerMelee(CaveSpiderPostAttack)
{
		m_EMPersonality = AGGRESSIVE;
		m_BehaviorAttackerMelee.AttachToMonster(*this);
		m_BehaviorWanderer.AttachToMonster(*this);
		m_BehaviorAggressive.AttachToMonster(*this);
		GetMonsterConfig("CaveSpider");
}





void cCaveSpider::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	m_EMPersonality = (GetWorld()->GetTimeOfDay() < (12000 + 1000)) ? PASSIVE : AGGRESSIVE;
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




