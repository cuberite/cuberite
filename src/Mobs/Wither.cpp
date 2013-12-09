
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wither.h"





cWither::cWither(void) :
	super("Wither", mtWither, "mob.wither.hurt", "mob.wither.death", 0.9, 4.0)
{
}





void cWither::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 1, 1, E_ITEM_NETHER_STAR);
}




