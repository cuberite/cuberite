
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Blaze.h"





cBlaze::cBlaze(void) :
	// TODO: The size is only a guesstimate, measure in vanilla and fix the size values here
	super("Blaze", 61, "mob.blaze.hit", "mob.blaze.death", 0.7, 1.8)
{
}





void cBlaze::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 1, E_ITEM_BLAZE_ROD);
}




