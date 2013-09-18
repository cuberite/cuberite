
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SnowGolem.h"





cSnowGolem::cSnowGolem(void) :
	super("SnowGolem", 97, "", "", 0.4, 1.8)
{
}





void cSnowGolem::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 5, E_ITEM_SNOWBALL);
}




