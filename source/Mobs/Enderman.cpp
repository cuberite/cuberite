
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Enderman.h"





cEnderman::cEnderman(void) :
	// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
	super("Enderman", 58, "mob.endermen.hit", "mob.endermen.death", 0.5, 2.5)
{
}





void cEnderman::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_ENDER_PEARL);
}




