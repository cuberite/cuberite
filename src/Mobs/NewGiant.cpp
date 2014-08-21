
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "NewGiant.h"





cNewGiant::cNewGiant(void) :
	super("Giant", mtGiant, "mob.zombie.hurt", "mob.zombie.death", 3.6, 10.8)
{

}





void cNewGiant::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 10, 50, E_ITEM_ROTTEN_FLESH);
}




