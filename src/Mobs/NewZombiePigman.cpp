#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewZombiePigman.h"
#include "../World.h"





cNewZombiePigman::cNewZombiePigman(void) :
	super("ZombiePigman", mtZombiePigman, "mob.zombiepig.zpighurt", "mob.zombiepig.zpigdeath", 0.6, 1.8)
{
}





void cNewZombiePigman::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_ROTTEN_FLESH);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GOLD_NUGGET);
	
	cItems RareDrops;
	RareDrops.Add(cItem(E_ITEM_GOLD));
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}




