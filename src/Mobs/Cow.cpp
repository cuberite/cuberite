
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cow.h"
#include "../Entities/Player.h"



cCow::cCow(void) :
	super("Cow", mtCow, "entity.cow.hurt", "entity.cow.death", 0.9, 1.3),
	m_BehaviorItemReplacer(E_ITEM_BUCKET, E_ITEM_MILK)
{
	m_EMPersonality = PASSIVE;
	m_BehaviorBreeder.AttachToMonster(*this);
	m_BehaviorCoward.AttachToMonster(*this);
	m_BehaviorItemFollower.AttachToMonster(*this);
	m_BehaviorWanderer.AttachToMonster(*this);
	m_BehaviorItemReplacer.AttachToMonster(*this);
}





void cCow::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3 + LootingLevel, IsOnFire() ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}





cBehaviorBreeder * cCow::GetBehaviorBreeder()
{
	return &m_BehaviorBreeder;
}





const cBehaviorBreeder * cCow::GetBehaviorBreeder() const
{
	return static_cast<const cBehaviorBreeder *>(&m_BehaviorBreeder);
}
