
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"





cEnderman::cEnderman(void) :
	super("Enderman", mtEnderman, "mob.endermen.hit", "mob.endermen.death", 0.5, 2.9),
	m_bIsScreaming(false),
	CarriedBlock(E_BLOCK_AIR),
	CarriedMeta(0)
{
}





void cEnderman::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_ENDER_PEARL);
}




