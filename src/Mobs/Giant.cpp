
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Giant.h"





cGiant::cGiant(void) :
	Super("Giant", mtGiant, "entity.zombie.hurt", "entity.zombie.death", "entity.zombie.ambient", 3.6f, 12)
{

}





void cGiant::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 10, 50, Item::RottenFlesh);
}




