
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IronGolem.h"





cIronGolem::cIronGolem(void) :
	super("IronGolem", mtIronGolem, "mob.IronGolem.hit", "mob.IronGolem.death", 1.4, 2.9)
{
}





void cIronGolem::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 5, E_ITEM_IRON);
	AddRandomDropItem(a_Drops, 0, 2, E_BLOCK_FLOWER);
}




