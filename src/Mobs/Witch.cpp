
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Witch.h"





cWitch::cWitch(void) :
	super("Witch", mtWitch, "", "", 0.6, 1.8)
{
}





void cWitch::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(E_ENCHANTMENT_LOOTING);
	}
	MTRand r1;
	int DropTypeCount = (r1.randInt() % 3) + 1;
	for (int i = 0; i < DropTypeCount; i++)
	{
		int DropType = r1.randInt() % 7;
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
	cItems RareDrops;
	if (!GetEquippedWeapon().IsEmpty()) RareDrops.Add(GetEquippedWeapon());
	AddRandomRareDropItem(a_Drops, RareDrops, LootingLevel);
}




