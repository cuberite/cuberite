
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Witch.h"





cWitch::cWitch(void) :
	super("Witch", mtWitch, "", "", 0.6, 1.8)
{
}





void cWitch::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 6, E_ITEM_GLASS_BOTTLE);
	AddRandomDropItem(a_Drops, 0, 6, E_ITEM_GLOWSTONE_DUST);
	AddRandomDropItem(a_Drops, 0, 6, E_ITEM_GUNPOWDER);
	AddRandomDropItem(a_Drops, 0, 6, E_ITEM_REDSTONE_DUST);
	AddRandomDropItem(a_Drops, 0, 6, E_ITEM_SPIDER_EYE);
	AddRandomDropItem(a_Drops, 0, 6, E_ITEM_STICK);
	AddRandomDropItem(a_Drops, 0, 6, E_ITEM_SUGAR);
}




