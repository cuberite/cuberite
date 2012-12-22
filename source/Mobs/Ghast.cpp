
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Ghast.h"





cGhast::cGhast(void) :
	super("Ghast", 56, "mob.ghast.scream", "mob.ghast.death")
{
}





void cGhast::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_GUNPOWDER);
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_GHAST_TEAR);
}




