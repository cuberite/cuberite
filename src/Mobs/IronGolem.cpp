
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "IronGolem.h"





cIronGolem::cIronGolem(void) :
	Super("IronGolem", mtIronGolem, "entity.irongolem.hurt", "entity.irongolem.death", "entity.irongolem.ambient", 1.4, 2.9)
{
}





void cIronGolem::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	UNUSED(a_Killer);
	AddRandomDropItem(a_Drops, 0, 5, E_ITEM_IRON);
	AddRandomDropItem(a_Drops, 0, 2, E_BLOCK_FLOWER);
}




