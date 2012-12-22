
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Magmacube.h"





cMagmacube::cMagmacube(void) :
	super("Magmacube", 62, "mob.magmacube.big", "mob.magmacube.big")
{
}





void cMagmacube::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_MAGMA_CREAM);
}




