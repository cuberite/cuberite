
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Slime.h"

// TODO: Implement sized slimes





cSlime::cSlime(void) :
	super("Slime", 55, "mob.slime.attack", "mob.slime.attack")
{
}





void cSlime::GetDrops(cItems & a_Drops, cPawn * a_Killer)
{
	// TODO: only when tiny
	AddRandomDropItem(a_Drops, 0, 2, E_ITEM_SLIMEBALL);
}




