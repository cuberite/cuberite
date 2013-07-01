
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Mooshroom.h"





// TODO: Milk Cow





cMooshroom::cMooshroom(void) :
	super("Mooshroom", 96, "mob.cow.hurt", "mob.cow.hurt", 0.9, 1.3)
{
}





void cMooshroom::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_LEATHER);
	AddRandomDropItem(a_Drops, 1, 3, IsOnFire() ? E_ITEM_STEAK : E_ITEM_RAW_BEEF);
}




