
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewBlaze.h"
#include "../World.h"
#include "../Entities/FireChargeEntity.h"




cNewBlaze::cNewBlaze(void) :
	super("Blaze", mtBlaze, "mob.blaze.hit", "mob.blaze.death", 0.6, 1.8)
{
}





void cNewBlaze::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if ((a_Killer != NULL) && (a_Killer->IsPlayer() || a_Killer->IsA("cWolf")))
	{
		int LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
		AddRandomDropItem(a_Drops, 0, 1 + LootingLevel, E_ITEM_BLAZE_ROD);
	}
}
