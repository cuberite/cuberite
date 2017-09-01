
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Spider.h"

#include "../World.h"
#include "../Entities/Player.h"
#include "../Chunk.h"


cSpider::cSpider(void) :
	super(mtSpider, "entity.spider.hurt", "entity.spider.death", 1.4, 0.9)
{
	m_EMPersonality = AGGRESSIVE;
	GetMonsterConfig("Spider");
}





void cSpider::GetDrops(cItems & a_Drops, cEntity * a_Killer)
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





bool cSpider::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	/* mobTodo
	// If the source of the damage is not from an pawn entity, switch to idle
	if ((a_TDI.Attacker == nullptr) || !a_TDI.Attacker->IsPawn())
	{
		m_EMState = IDLE;
	}
	else
	{
		// If the source of the damage is from a pawn entity, chase that entity
		m_EMState = CHASING;
	}*/

	return true;
}
