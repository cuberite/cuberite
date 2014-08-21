
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewZombie.h"
#include "../World.h"
#include "../LineBlockTracer.h"





cNewZombie::cNewZombie(bool a_IsVillagerZombie) :
	super("Zombie", mtZombie, "mob.zombie.hurt", "mob.zombie.death", 0.6, 1.8),
	m_IsVillagerZombie(a_IsVillagerZombie),
	m_IsConverting(false)
{
}





void cNewZombie::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_ROTTEN_FLESH);
	cItems RareDrops;
	RareDrops.Add(cItem(E_ITEM_IRON));
	RareDrops.Add(cItem(E_ITEM_CARROT));
	RareDrops.Add(cItem(E_ITEM_POTATO));
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}




