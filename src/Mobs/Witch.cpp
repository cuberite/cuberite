
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Witch.h"
#include "../FastRandom.h"





cWitch::cWitch(void) :
	Super("Witch", mtWitch, "entity.witch.hurt", "entity.witch.death", "entity.witch.ambient", 0.6, 1.8)
{
}





void cWitch::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	auto & r1 = GetRandomProvider();
	int DropTypeCount = r1.RandInt(1, 3);
	for (int i = 0; i < DropTypeCount; i++)
	{
		int DropType = r1.RandInt(6);
		switch (DropType)
		{
			case 0: AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GLASS_BOTTLE); break;
			case 1: AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GLOWSTONE_DUST); break;
			case 2: AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GUNPOWDER); break;
			case 3: AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_REDSTONE_DUST); break;
			case 4: AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_SPIDER_EYE); break;
			case 5: AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_STICK); break;
			case 6: AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_SUGAR); break;
		}
	}
	AddRandomWeaponDropItem(a_Drops, LootingLevel);
}




