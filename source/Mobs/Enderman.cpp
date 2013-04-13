
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"





cEnderman::cEnderman(void) :
	super("Enderman", 58, "mob.endermen.hit", "mob.endermen.death")
{
}





void cEnderman::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_ENDER_PEARL);
}




