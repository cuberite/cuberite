
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Zombie.h"
#include "../World.h"
#include "../LineBlockTracer.h"





cZombie::cZombie() :
	Super("Zombie", mtZombie, "entity.zombie.hurt", "entity.zombie.death", "entity.zombie.ambient", 0.6f, 1.95f)
{
}





void cZombie::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, Item::RottenFlesh);
	cItems RareDrops;
	RareDrops.Add(cItem(Item::IronIngot));
	RareDrops.Add(cItem(Item::Carrot));
	RareDrops.Add(cItem(Item::Potato));
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
	AddRandomArmorDropItem(a_Drops, LootingLevel);
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}
