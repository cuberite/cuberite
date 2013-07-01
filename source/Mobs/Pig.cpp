
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pig.h"





cPig::cPig(void) :
	super("Pig", 90, "mob.pig.say", "mob.pig.death", 0.9, 0.9)
{
}





void cPig::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 1, 3, IsOnFire() ? E_ITEM_COOKED_PORKCHOP : E_ITEM_RAW_PORKCHOP);
}




