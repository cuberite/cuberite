
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombiepigman.h"
#include "../World.h"





cZombiePigman::cZombiePigman(void) :
	super("ZombiePigman", mtZombiePigman, "mob.zombiepig.zpighurt", "mob.zombiepig.zpigdeath", 0.6, 1.8)
{
}





void cZombiePigman::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(E_ENCHANTMENT_LOOTING);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_ROTTEN_FLESH);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GOLD_NUGGET);
	
	cItems RareDrops;
	RareDrops.Add(cItem(E_ITEM_GOLD));
	if (!GetEquippedWeapon().IsEmpty()) RareDrops.Add(GetEquippedWeapon());
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
}





void cZombiePigman::KilledBy(cEntity * a_Killer)
{
	super::KilledBy(a_Killer);

	if ((a_Killer != NULL) && (a_Killer->IsPlayer()))
	{
		// TODO: Anger all nearby zombie pigmen
		// TODO: In vanilla, if one player angers ZPs, do they attack any nearby player, or only that one attacker?
	}
}




