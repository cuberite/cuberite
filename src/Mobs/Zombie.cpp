
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombie.h"
#include "../World.h"
#include "../LineBlockTracer.h"





cZombie::cZombie() :
	Super("Zombie", mtZombie, "entity.zombie.hurt", "entity.zombie.death", "entity.zombie.ambient", 0.6, 1.8)
{
}





void cZombie::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
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
