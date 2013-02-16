
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Pig.h"





cPig::cPig(void) :
	super("Pig", 90, "mob.pig.say", "mob.pig.death")
{
}





void cPig::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	AddRandomDropItem(a_Drops, 1, 3, (GetMetaData() == BURNING) ? E_ITEM_COOKED_PORKCHOP : E_ITEM_RAW_PORKCHOP);
}




