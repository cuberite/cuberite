
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewSnowGolem.h"
#include "../World.h"





cNewSnowGolem::cNewSnowGolem(void) :
	super("SnowGolem", mtSnowGolem, "", "", 0.4, 1.8)
{
}





void cNewSnowGolem::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	AddRandomDropItem(a_Drops, 0, 15, E_ITEM_SNOWBALL);
}
