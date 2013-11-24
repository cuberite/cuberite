
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Giant.h"





cGiant::cGiant(void) :
	// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
	super("Giant", mtGiant, "mob.zombie.hurt", "mob.zombie.death", 2.0, 13.5)
{
}





void cGiant::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 10, 50, E_ITEM_ROTTEN_FLESH);
}




