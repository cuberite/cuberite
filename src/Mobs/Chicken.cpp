#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Chicken.h"
#include "../World.h"







cChicken::cChicken(void) :
	super("Chicken", mtChicken, "mob.chicken.hurt", "mob.chicken.hurt", 0.3, 0.4),
	m_EggDropTimer(0)
{
}




void cChicken::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if ((m_EggDropTimer == 6000) && (m_World->GetTickRandomNumber(1) == 0))
	{
		cItems Drops;
		m_EggDropTimer = 0;
		Drops.push_back(cItem(E_ITEM_EGG, 1));
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
	}
	else if (m_EggDropTimer == 12000)
	{
		cItems Drops;
		m_EggDropTimer = 0;
		Drops.push_back(cItem(E_ITEM_EGG, 1));
		m_World->SpawnItemPickups(Drops, GetPosX(), GetPosY(), GetPosZ(), 10);
	}
	else
	{
		m_EggDropTimer++;
	}
}





void cChicken::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(E_ENCHANTMENT_LOOTING);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_FEATHER);
	AddRandomDropItem(a_Drops, 1, 1, IsOnFire() ? E_ITEM_COOKED_CHICKEN : E_ITEM_RAW_CHICKEN);
}








