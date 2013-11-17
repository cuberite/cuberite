
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Giant.h"





cGiant::cGiant(void) :
	// 6.0 * (0.6|1.8)
	super("Giant", mtGiant, "mob.zombie.hurt", "mob.zombie.death", 3.6, 10.8)
{

}





void cGiant::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 10, 50, E_ITEM_ROTTEN_FLESH);
}




