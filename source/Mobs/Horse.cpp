
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Horse.h"





cHorse::cHorse(void) :
	super("Horse", 100, "mob.horse.hit", "mob.horse.death", 1.4, 1.6)
{
}





void cHorse::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_LEATHER);
}




