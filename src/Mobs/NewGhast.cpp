
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewGhast.h"
#include "../World.h"
#include "../Entities/GhastFireballEntity.h"




cNewGhast::cNewGhast(void) :
	super("Ghast", mtGhast, "mob.ghast.scream", "mob.ghast.death", 4, 4)
{
}





void cNewGhast::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_GUNPOWDER);
	AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_GHAST_TEAR);
}



