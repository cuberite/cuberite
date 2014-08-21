
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Squid.h"
#include "../Vector3.h"
#include "../Chunk.h"





cSquid::cSquid(void) :
	super("Squid", mtSquid, "", "", 0.95, 0.95)
{
}





void cSquid::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	// Drops 0-3 Ink Sacs
	int LootingLevel = 0;
	if (a_Killer != NULL)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 3 + LootingLevel, E_ITEM_DYE, E_META_DYE_BLACK);
}



